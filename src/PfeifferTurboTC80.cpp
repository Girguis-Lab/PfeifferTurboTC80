#include "PfeifferTurboTC80.h"

/**
 * @file PfeifferTurboTC80.cpp
 *
 * Default arguments live on the declarations in the header; repeating them here
 * would be a redefinition.
 */

using namespace PfeifferVacProtocol;

bool pfeifferDefaultDelay(unsigned long ms)
{
    delay(ms);
    return false; // never aborts
}

// ===================================================================
// Construction and configuration
// ===================================================================

PfeifferSerialTC80::PfeifferSerialTC80(RS485Serial_Base &turboSerial,
                                       const uint16_t address,
                                       DelayFunc delayFunc)
    : _turboSerial(turboSerial), _delayFunc(delayFunc)
{
    // Seed a valid address before setAddress(), which rejects out-of-range
    // values and would otherwise leave the buffer uninitialised.
    strcpy(_address, "001");
    setAddress(address);
}

PfeifferSerialTC80::PfeifferSerialTC80(RS485Serial_Base &turboSerial,
                                       const uint16_t address,
                                       Stream &debug,
                                       DelayFunc delayFunc)
    : _turboSerial(turboSerial), _delayFunc(delayFunc)
{
    strcpy(_address, "001");
    setAddress(address);
    PfeifferVacProtocol::setLogStream(debug);
}

void PfeifferSerialTC80::begin(unsigned long baud, uint32_t config)
{
    _turboSerial.begin(baud, config); // RS485Serial class handles pin setup and initialization
}

void PfeifferSerialTC80::setAddress(const uint16_t address)
{
    // The address field is three characters wide. The old constructor used
    // sprintf() straight into this 4-byte buffer, so any address of 1000 or
    // more overran it.
    if (address > PfeifferVacProtocol::PARAMETER_MAX)
        return;
    snprintf(_address, sizeof(_address), "%03u", (unsigned)address);
}

// ===================================================================
// Sending
// ===================================================================

void PfeifferSerialTC80::sendTelegramRaw(const char *telegram)
{
    if (telegram == nullptr)
        return;

    // Send the body and its terminating CR as one write. Each write() call
    // asserts DE, transmits, waits for the shift register to drain and then
    // releases the bus, so writing the CR separately let the line go idle
    // mid-telegram and cost an extra turnaround.
    const size_t bodyLength = strlen(telegram);
    String frame;
    frame.reserve(bodyLength + 1);
    frame += telegram;
    frame += '\r';

    _turboSerial.write(reinterpret_cast<const uint8_t *>(frame.c_str()), frame.length());
    // The RS485Serial class automatically handles direction switching.
}

void PfeifferSerialTC80::printOutgoingTelegram(const char action, const uint16_t parameter, const char *data)
{
    String telegramStr = encodeTelegram(_address, action, parameter, data);
    PfeifferTelegram telegram = decodeTelegram(telegramStr.c_str());
    printTelegramHumanReadable(telegram, true);
    PFTC80_LOG_TRACE("[Raw Telegram: '");
    PFTC80_LOG_TRACE(telegramStr.c_str());
    PFTC80_LOG_TRACE("']\n");
}

void PfeifferSerialTC80::sendTelegram(const char action, const uint16_t parameter, const char *data, const bool debugPrint)
{
    if (debugPrint || PFTC80_LOG_LEVEL_AT_LEAST(PFTC80_LOG_LVL_DEBUG))
    {
        printOutgoingTelegram(action, parameter, data);
    }

    String telegramStr = encodeTelegram(_address, action, parameter, data);
    if (telegramStr.length() == 0)
    {
        PFTC80_LOG_WARN(F("!WARN: Refusing to send malformed telegram for parameter "), parameter, "\n");
        return;
    }
    sendTelegramRaw(telegramStr.c_str());
}

void PfeifferSerialTC80::sendQuery(const uint16_t parameter, const bool debugPrint)
{
    sendTelegram((char)Action::Query, parameter, "=?", debugPrint);
}

void PfeifferSerialTC80::sendCommand(const uint16_t parameter, const char *data, const bool debugPrint)
{
    sendTelegram((char)Action::Command, parameter, data, debugPrint);
}

// One overload per datatype. Each encodes its payload and hands it to the
// const char* overload above.
#define PFTC80_DEFINE_SEND_COMMAND(TYPE)                                                                 \
    void PfeifferSerialTC80::sendCommand(const uint16_t parameter, const PfeifferVacProtocol::TYPE data, \
                                         const bool debugPrint)                                          \
    {                                                                                                    \
        sendTelegram((char)Action::Command, parameter, data.encode().c_str(), debugPrint);               \
    }

PFTC80_DEFINE_SEND_COMMAND(BooleanOld) // 0: boolean_old
PFTC80_DEFINE_SEND_COMMAND(UInteger)   // 1: u_integer
PFTC80_DEFINE_SEND_COMMAND(UReal)      // 2: u_real
PFTC80_DEFINE_SEND_COMMAND(String6)    // 4: string (6 chars)
PFTC80_DEFINE_SEND_COMMAND(BooleanNew) // 6: boolean_new
PFTC80_DEFINE_SEND_COMMAND(UShortInt)  // 7: u_short_int
PFTC80_DEFINE_SEND_COMMAND(UExpoNew)   // 10: u_expo_new
PFTC80_DEFINE_SEND_COMMAND(String16)   // 11: string16
PFTC80_DEFINE_SEND_COMMAND(String8)    // 12: string8

#undef PFTC80_DEFINE_SEND_COMMAND

// ===================================================================
// Receiving
// ===================================================================

String PfeifferSerialTC80::receiveTelegramRaw(unsigned long timeout)
{
    String frame = "";
    frame.reserve(TELEGRAM_MAX_LENGTH);
    unsigned long start = millis();
    while (millis() - start < timeout)
    {
        _turboSerial.task(); // Call task() to handle buffering and direction control
        if (_turboSerial.available())
        {
            int c = _turboSerial.read();
            if (c < 0)
                continue;
            if (c == '\r')
                break;
            // Cap the frame instead of letting line noise grow it for the whole
            // timeout window; a telegram can never exceed this length.
            if (frame.length() >= TELEGRAM_MAX_LENGTH)
            {
                PFTC80_LOG_DEBUG(F("!WARN: Oversized turbo frame discarded.\n"));
                frame = "";
                continue; // resync on the next CR
            }
            frame += (char)c;
        }
    }
    return frame;
}

PfeifferTelegram PfeifferSerialTC80::receiveTelegram(bool debugPrint, unsigned long timeout)
{
    String raw = receiveTelegramRaw(timeout);
    PfeifferTelegram telegram = decodeTelegram(raw.c_str());
    if (debugPrint || PFTC80_LOG_LEVEL_AT_LEAST(PFTC80_LOG_LVL_DEBUG))
    {
        printTelegramHumanReadable(telegram, false);
        PFTC80_LOG_TRACE("\n[RAW Received Telegram is '");
        PFTC80_LOG_TRACE(raw.c_str());
        PFTC80_LOG_TRACE("'] ");
        PFTC80_LOG_INFO("\n");
    }
    return telegram;
}

bool PfeifferSerialTC80::isValidResponse(const PfeifferTelegram &telegram, uint16_t expectedParameter) const
{
    // A standard successful response has the same source address and no Errors
    return strcmp(telegram.address.c_str(), _address) == 0 &&
           telegram.error == TelegramError::None &&
           (uint16_t)telegram.parameter.toInt() == expectedParameter;
}

void PfeifferSerialTC80::printInvalidResponseDebug(const PfeifferTelegram &telegram, uint16_t expectedParameter) const
{
    if (telegram.address.length() == 0 && telegram.parameter.length() == 0)
    {
        return; // no message was received, ignore.
    }
    else if (strcmp(telegram.address.c_str(), _address) != 0)
    {
        PFTC80_LOG_DEBUG(F("!WARN: Address mismatch in Turbo response. Expected: "), _address, ", Received: ", telegram.address.c_str(), "\n");
    }
    else if (telegram.error == TelegramError::LogicError)
    {
        PFTC80_LOG_DEBUG(F("!WARN: Turbo pump internal logic error.\n"));
    }
    else if (telegram.error == TelegramError::OutOfRange)
    {
        PFTC80_LOG_DEBUG(F("!WARN: Given command value was out of range.\n"));
    }
    else if (telegram.error == TelegramError::InvalidParameter)
    {
        PFTC80_LOG_DEBUG(F("!WARN: Parameter "));
        PFTC80_LOG_DEBUG(telegram.parameter.c_str());
        PFTC80_LOG_DEBUG(F(" is not a valid parameter.\n"));
    }
    else if (telegram.error == TelegramError::InvalidChecksum)
    {
        PFTC80_LOG_DEBUG(F("!WARN: Checksum invalid or partial Turbo response.\n"));
    }
    // uint16_t, not uint8_t: every StatusRequest (300-399) and
    // ReferenceValueInput (700-797) parameter is above 255, so a uint8_t cast
    // made this comparison meaningless and misreported the reason.
    else if ((uint16_t)telegram.parameter.toInt() != expectedParameter)
    {
        PFTC80_LOG_DEBUG(F("!WARN: Unexpected parameter in Turbo response: "));
        PFTC80_LOG_DEBUG(telegram.parameter.c_str());
        PFTC80_LOG_DEBUG(F(" != "));
        PFTC80_LOG_DEBUG(expectedParameter, "\n");
    }
    else
    {
        PFTC80_LOG_DEBUG(F("!WARN: Data length mismatch or unexpected content in Turbo response.\n"));
    }
}

String PfeifferSerialTC80::receiveValidatedPayload(uint16_t expectedParameter, unsigned int expectedDataLength,
                                                   bool &isValid, bool debugPrint, unsigned long timeout)
{
    const unsigned long attemptTimeout = timeout / RECEIVE_ATTEMPTS_PER_TIMEOUT;
    unsigned long startTime = millis();
    while (millis() - startTime < timeout)
    {
        const PfeifferTelegram telegram = receiveTelegram(debugPrint, attemptTimeout);
        if (isValidResponse(telegram, expectedParameter) && telegram.data.length() == expectedDataLength)
        {
            isValid = true;
            return telegram.data;
        }

        isValid = false;
        printInvalidResponseDebug(telegram, expectedParameter);
        // Small delay before retrying. Break out if something happened during
        // the delay that requires exit, such as a received user command.
        if (_delayFunc(RETRY_DELAY_MS))
            break;
    }
    return String("");
}

// Each typed read validates the payload length for its datatype, then decodes.

boolean PfeifferSerialTC80::receiveBooleanOld(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 6, isValid, debugPrint, timeout);
    return isValid ? BooleanOld(payload.c_str()).decode() : false;
}

unsigned long PfeifferSerialTC80::receiveUInteger(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 6, isValid, debugPrint, timeout);
    return isValid ? UInteger(payload.c_str()).decode() : 0;
}

float PfeifferSerialTC80::receiveUReal(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 6, isValid, debugPrint, timeout);
    return isValid ? UReal(payload.c_str()).decode() : 0.0f;
}

boolean PfeifferSerialTC80::receiveBooleanNew(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 1, isValid, debugPrint, timeout);
    return isValid ? BooleanNew(payload.c_str()).decode() : false;
}

uint16_t PfeifferSerialTC80::receiveUShortInt(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 3, isValid, debugPrint, timeout);
    return isValid ? UShortInt(payload.c_str()).decode() : 0;
}

float PfeifferSerialTC80::receiveUExpoNew(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 6, isValid, debugPrint, timeout);
    return isValid ? UExpoNew(payload.c_str()).decode() : 0.0f;
}

String PfeifferSerialTC80::receiveString6(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 6, isValid, debugPrint, timeout);
    return isValid ? String(String6(payload.c_str()).decode()) : String("");
}

String PfeifferSerialTC80::receiveString16(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 16, isValid, debugPrint, timeout);
    return isValid ? String(String16(payload.c_str()).decode()) : String("");
}

String PfeifferSerialTC80::receiveString8(uint16_t expectedParameter, bool &isValid, bool debugPrint, unsigned long timeout)
{
    String payload = receiveValidatedPayload(expectedParameter, 8, isValid, debugPrint, timeout);
    return isValid ? String(String8(payload.c_str()).decode()) : String("");
}

// ===================================================================
// Combined query and receive
// ===================================================================

// One overload per datatype. Each sends the query, gives the pump a moment to
// turn the bus around, and decodes the reply with the matching receive*().
#define PFTC80_DEFINE_QUERY(TYPE, RETURN_TYPE)                                                \
    RETURN_TYPE PfeifferSerialTC80::query##TYPE(const uint16_t parameter, bool &isValid,       \
                                                const bool debugPrint, unsigned long timeout) \
    {                                                                                         \
        sendQuery(parameter, debugPrint);                                                     \
        _delayFunc(1); /* delay to avoid backtalk; honours the caller's delay hook */         \
        return receive##TYPE(parameter, isValid, debugPrint, timeout);                        \
    }

PFTC80_DEFINE_QUERY(BooleanOld, boolean)      // 0: boolean_old
PFTC80_DEFINE_QUERY(UInteger, unsigned long)  // 1: u_integer
PFTC80_DEFINE_QUERY(UReal, float)             // 2: u_real
PFTC80_DEFINE_QUERY(String6, String)          // 4: string (6 chars)
PFTC80_DEFINE_QUERY(BooleanNew, boolean)      // 6: boolean_new
PFTC80_DEFINE_QUERY(UShortInt, uint16_t)      // 7: u_short_int
PFTC80_DEFINE_QUERY(UExpoNew, float)          // 10: u_expo_new
PFTC80_DEFINE_QUERY(String16, String)         // 11: string16
PFTC80_DEFINE_QUERY(String8, String)          // 12: string8

#undef PFTC80_DEFINE_QUERY

// ===================================================================
// Error reporting
// ===================================================================

String PfeifferSerialTC80::queryLatestError(bool &isValid, bool debugPrint, unsigned long timeout)
{
    String errMsg = queryString6(StatusRequest::ErrorCode, isValid, debugPrint,
                                 timeout / RECEIVE_ATTEMPTS_PER_TIMEOUT);
    if (!isValid)
        return String("");

#if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
    const __FlashStringHelper *problem = getHumanReadableErrorMessage(errMsg.c_str());
    if (problem != nullptr)
        return String(problem);
#endif
    return errMsg;
}

String PfeifferSerialTC80::queryErrorHistory(bool &isValid, bool debugPrint, unsigned long timeout)
{
    String errorMessages = "";
    for (int p = StatusRequest::ErrHist1; p <= StatusRequest::ErrHist10; p++)
    {
        String errCode = queryString6(p, isValid, debugPrint, timeout / RECEIVE_ATTEMPTS_PER_TIMEOUT);
        if (!isValid)
        {
            errorMessages += "failed to query turbo error history,";
            break;
        }
        else if (errCode == String("000000"))
        {
            break; // we've reached the end of the error/warning history
        }
        else
        {
            errorMessages += errCode;
#if PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
            const __FlashStringHelper *problem = getHumanReadableErrorMessage(errCode.c_str());
            if (problem != nullptr)
            {
                errorMessages += String(" ") + String(problem);
            }
#endif // PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG
            errorMessages += ",";
        }
    }
    return errorMessages;
}
