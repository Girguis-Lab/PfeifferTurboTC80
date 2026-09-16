#pragma once

/**
 * @file PfeifferVacProtocolFuncs.h
 * @brief Telegram encoding, decoding, and human-readable rendering.
 */

#include <Arduino.h>
#include "PfeifferVacProtocolConsts.h"
#include "PfeifferVacProtocolDatatypes.h"
#include "PfeifferLog.h"

namespace PfeifferVacProtocol
{
    /// Shortest legal telegram: 3 address + 1 action + 1 '0' + 3 parameter
    /// + 2 data length + 3 checksum, with an empty payload.
    constexpr size_t TELEGRAM_MIN_LENGTH = 13;

    /// Longest telegram this library can produce or parse: the minimum above
    /// plus a 16-character string16 payload. Excludes the terminating CR.
    constexpr size_t TELEGRAM_MAX_LENGTH = TELEGRAM_MIN_LENGTH + 16;

    /// Highest parameter number the 3-digit parameter field can represent.
    constexpr uint16_t PARAMETER_MAX = 999;

    struct PfeifferTelegram
    {
        String address;
        char action;
        String parameter;
        String data;
        TelegramError error;

        PfeifferTelegram() : address(""), action('\0'), parameter(""), data(""), error(TelegramError::None)
        {
        }
    };

    /**
     * @brief Build a telegram from its fields, appending the checksum.
     *
     * @param address   3-digit pump address as a C string, e.g. "001".
     * @param action    '0' to query, '1' to command (see Action).
     * @param parameter Parameter number, 0 to PARAMETER_MAX.
     * @param data      Payload, or nullptr for none.
     * @return The telegram without its terminating carriage return, or an empty
     *         String if @p address is null or @p parameter is out of range.
     */
    String encodeTelegram(const char *address, const char action, const uint16_t parameter, const char *data = nullptr);

    /**
     * @brief Parse a received telegram.
     *
     * The returned telegram's `error` field reports what went wrong:
     * TelegramError::InvalidChecksum for a malformed, truncated or corrupted
     * frame, and InvalidParameter / OutOfRange / LogicError for the pump's own
     * "NO_DEF" / "_RANGE" / "_LOGIC" replies.
     *
     * A telegram that was never received at all (an empty input) comes back
     * with empty address and parameter fields; callers use that to tell
     * "nothing arrived" from "something arrived and was bad".
     *
     * @param telegram Null-terminated frame with the carriage return removed.
     */
    PfeifferTelegram decodeTelegram(const char *telegram);

    /**
     * @brief Log a telegram with its parameter name and decoded value.
     *
     * Output goes through the PFTC80_LOG_* macros, so it is subject to both the
     * logging backend selected in PfeifferLog.h and the runtime level. When
     * PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG is 0 the name and value tables do not
     * exist, and this prints the raw parameter number and payload instead.
     *
     * @param outgoing true to render as a sent telegram, false as a received one.
     */
    void printTelegramHumanReadable(const PfeifferTelegram &telegram, bool outgoing = false);
}
