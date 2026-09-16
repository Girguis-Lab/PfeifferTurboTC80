#include "PfeifferLog.h"
#include "PfeifferVacProtocolFuncs.h"

/**
 * @file PfeifferVacProtocolFuncs.cpp
 * @brief Telegram encoding, decoding, and human-readable rendering.
 *
 * Default arguments live on the declarations in the header; repeating them here
 * would be a redefinition.
 */

namespace PfeifferVacProtocol
{
    // Helper: Encode a telegram from action, parameter, and data. Returns an empty string if the telegram cannot be encoded (invalid param, etc..)
    String encodeTelegram(const char *address, const char action, const uint16_t parameter, const char *data)
    {
        // The parameter field is three digits wide, and the data length field
        // two, so anything larger would be silently truncated into a valid
        // looking but wrong telegram. Refuse instead.
        if (address == nullptr || parameter > PARAMETER_MAX)
            return String("");
        const size_t dataLen = (data != nullptr) ? strlen(data) : 0;
        if (dataLen > 99)
            return String("");

        // Telegrams starts with the 3-digit address
        String frame = String(address);
        // Then the action character either a 1 for set or a 0 for query
        frame += action;
        frame += '0'; // 0 character as per protocol
        char paramStr[4];
        snprintf(paramStr, sizeof(paramStr), "%03u", (unsigned)parameter);
        frame += paramStr;
        if (data != nullptr)
        {
            char dataLength[3];
            snprintf(dataLength, sizeof(dataLength), "%02u", (unsigned)dataLen);
            frame += dataLength;
            frame += data;
        }
        else
        {
            frame += "00"; // no data
        }
        // Calculate checksum
        uint16_t checksum = 0;

        for (size_t i = 0; i < frame.length(); ++i)
            checksum += (uint16_t)frame[i];
        checksum = checksum % 256;
        char checksumStr[4];
        snprintf(checksumStr, sizeof(checksumStr), "%03u", (unsigned)checksum);
        frame += checksumStr;
        return frame;
    }

    // Helper: Decode a telegram into its fields (returns PfeifferResponse)
    PfeifferTelegram decodeTelegram(const char *telegram)
    {
        PfeifferTelegram resp;
        if (telegram == nullptr)
        {
            resp.error = TelegramError::InvalidChecksum;
            return resp;
        }

        String telegramStr = String(telegram);
        size_t len = strlen(telegram);

        // A telegram shorter than the minimum has no checksum field to read.
        // The old bound of 8 let substring(len - 3, len) overlap the parameter
        // field, so a runt frame was scored against its own parameter digits.
        // Address and parameter stay empty here, which is how callers tell
        // "nothing arrived" from "something arrived and was bad".
        if (len < TELEGRAM_MIN_LENGTH)
        {
            resp.error = TelegramError::InvalidChecksum;
            return resp;
        }

        resp.address = telegramStr.substring(0, 3);
        resp.action = telegram[3];
        // there is always a '0' at index 4
        resp.parameter = telegramStr.substring(5, 8);

        // Data is between param and checksum. The declared payload length must
        // account for exactly the bytes between the header and the 3-digit
        // checksum; otherwise the frame is truncated or corrupt and could
        // still pass on a coincidental checksum match.
        int dataLen = atoi(telegramStr.substring(8, 10).c_str());
        if (dataLen < 0 || (size_t)(10 + dataLen + 3) != len)
        {
            resp.error = TelegramError::InvalidChecksum;
            return resp;
        }
        resp.data = telegramStr.substring(10, 10 + dataLen);

        // handle special error cases based on data content
        if (resp.data == "NO_DEF")
        {
            resp.error = TelegramError::InvalidParameter;
        }
        else if (resp.data == "_RANGE")
        {
            resp.error = TelegramError::OutOfRange;
        }
        else if (resp.data == "_LOGIC")
        {
            resp.error = TelegramError::LogicError;
        }

        uint16_t telegramChecksum = (uint16_t)atoi(telegramStr.substring(len - 3, len).c_str());
        // Calculate checksum from every char except the 3 checksum digits
        uint16_t calculatedChecksum = 0;
        for (size_t i = 0; i < len - 3; ++i)
            calculatedChecksum += (uint16_t)telegram[i];
        calculatedChecksum = calculatedChecksum % 256;
        if (telegramChecksum != calculatedChecksum)
        {
            resp.error = TelegramError::InvalidChecksum;
        }
        return resp;
    }

    // Print a human-readable version of a PfeifferTelegram
    void printTelegramHumanReadable(const PfeifferTelegram &telegram, bool outgoing)
    {
        // print details
        if (outgoing)
        {
            PFTC80_LOG_INFO("-> Turbo ");
        }
        else
        {
            PFTC80_LOG_INFO("<- Turbo ");
        }
        // return early for empty telegram
        if (telegram.address.length() == 0 || telegram.parameter.length() == 0)
        {
            PFTC80_LOG_INFO("Nothing ");
            return;
        }

        PFTC80_LOG_TRACE("(Addr: ", telegram.address.c_str(), ")");
        if (outgoing)
        {
            if (telegram.action == (char)Action::Query)
                PFTC80_LOG_INFO("Query ");
            else if (telegram.action == (char)Action::Command)
                PFTC80_LOG_INFO("Command set ");
            else
                PFTC80_LOG_WARN("Unknown action '", telegram.action, "' ");
        }

#if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
        // Find the command entry by parameter number. The lookup tables only
        // exist in a debug build; without them every parameter is "unknown"
        // and we fall through to printing the raw number and payload.
        ParameterDebugEntry debugEntry = {};
        int paramNum = telegram.parameter.toInt();
        const bool debugEntryFound = getParameterDebugEntry(paramNum, &debugEntry);

        if (debugEntryFound)
        {
            PFTC80_LOG_INFO(debugEntry.description);
        }
#endif
        PFTC80_LOG_DEBUG(" (p", telegram.parameter.c_str(), ")");

        if (telegram.error == TelegramError::InvalidChecksum)
        {
            PFTC80_LOG_INFO(" checksum error!\n");
            return; // For errors return early
        }
        else if (telegram.error == TelegramError::InvalidParameter)
        {
            PFTC80_LOG_INFO(" invalid parameter!\n");
            return; // For errors return early
        }
        if (telegram.error == TelegramError::LogicError)
        {
            PFTC80_LOG_INFO(" turbo pump logic error!\n");
            return; // For errors return early
        }
        if (telegram.error == TelegramError::OutOfRange)
        {
            PFTC80_LOG_INFO(" value out of range!\n");
            return; // For errors return early
        }

        if (outgoing && telegram.action == (char)Action::Query)
        {
            PFTC80_LOG_INFO("\n");
            return; // For outgoing queries, we don't have data to interpret, so we can return early after printing the parameter description
        }

#if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
        if (debugEntryFound)
        {
            if (telegram.data.length() > 0)
            {
                if (outgoing && telegram.action == (char)Action::Command)
                    PFTC80_LOG_INFO(" to ");
                else if (!outgoing)
                    PFTC80_LOG_INFO(" is ");
                const char *ascii = telegram.data.c_str();
                (void)ascii; // only read by the log macros, which may compile out, void here gets around unused variable compiler warnings
                switch (debugEntry.datatype)
                {
                case DataType::BooleanOld:
                    PFTC80_LOG_INFO((debugEntry.lookupFuncFunction ? String(debugEntry.lookupFuncFunction(BooleanOld(ascii).decode() ? 1 : 0)) : String(BooleanOld(ascii).decode())).c_str());
                    PFTC80_LOG_TRACE(" (BooleanOld ", ascii, ")");
                    break;
                case DataType::BooleanNew:
                    PFTC80_LOG_INFO((debugEntry.lookupFuncFunction ? String(debugEntry.lookupFuncFunction(BooleanNew(ascii).decode() ? 1 : 0)) : String(BooleanNew(ascii).decode())).c_str());
                    PFTC80_LOG_TRACE(" (BooleanNew ", ascii, ")");
                    break;
                case DataType::UShortInt:
                    PFTC80_LOG_INFO((debugEntry.lookupFuncFunction ? String(debugEntry.lookupFuncFunction(UShortInt(ascii).decode())) : String(UShortInt(ascii).decode())).c_str());
                    PFTC80_LOG_TRACE(" (UShortInt ", ascii, ")");
                    break;
                case DataType::UInteger:
                    PFTC80_LOG_INFO(UInteger(ascii).decode());
                    PFTC80_LOG_TRACE(" (UInteger ", ascii, ")");
                    break;
                case DataType::UReal:
                    PFTC80_LOG_INFO(UReal(ascii).decode());
                    PFTC80_LOG_TRACE(" (UReal ", ascii, ")");
                    break;
                case DataType::String6:
                    PFTC80_LOG_INFO(ascii);
                    break;
                case DataType::String16:
                    PFTC80_LOG_INFO(ascii);
                    break;
                case DataType::String8:
                    PFTC80_LOG_INFO(ascii);
                    break;
                default:
                    PFTC80_LOG_INFO(ascii);
                    PFTC80_LOG_DEBUG(" (Unknown Datatype");
                    PFTC80_LOG_DEBUG(static_cast<int>(debugEntry.datatype));
                    PFTC80_LOG_DEBUG(")");
                    break;
                }
            }
            else
            {
                PFTC80_LOG_INFO("(no data)");
            }
        }
        else
#endif // PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
        {
            PFTC80_LOG_INFO(" Unknown Param ", telegram.parameter.c_str());
            PFTC80_LOG_DEBUG("RawData: ", telegram.data.length() > 0 ? telegram.data.c_str() : "(no data)");
        }
        PFTC80_LOG_INFO("\n");
    }
}
