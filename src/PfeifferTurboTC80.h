#pragma once

/**
 * @file PfeifferTurboTC80.h
 * @brief Driver for Pfeiffer turbo pumps behind a TC80 electronic drive unit.
 *
 * Talks the Pfeiffer Vacuum Protocol over RS-485. Construct it with an
 * RS485Serial_Base (normally an RS485HardwareSerial wrapping a UART plus DE/RE
 * pins), call begin(), then pair a sendQuery()/sendCommand() with the matching
 * receive*() for the parameter's datatype.
 *
 * Logging is configured in PfeifferLog.h; the human-readable parameter tables
 * are a compile-time option described in PfeifferVacProtocolConsts.h.
 */

#include <Arduino.h>
#include "PfeifferLog.h"
#include "PfeifferVacProtocolConsts.h"
#include "PfeifferVacProtocolFuncs.h"
#include "PfeifferVacProtocolDatatypes.h"
#include "RS485Serial.h"

/**
 * @brief Default delay used between receive retries: Arduino delay(), blocking.
 *
 * Supply your own DelayFunc to keep servicing the rest of the sketch while the
 * driver waits. Returning true from it aborts the operation in progress, at the
 * cost of possibly abandoning a partly-received message.
 */
bool pfeifferDefaultDelay(unsigned long ms);

class PfeifferSerialTC80
{
public:
    /// Delay callback. Returns true to abort the operation in progress.
    typedef bool (*DelayFunc)(unsigned long);

    /// Pause between retries while waiting for a response, in milliseconds.
    static const unsigned long RETRY_DELAY_MS = 50;

    /// A receive*() call splits its budget across several read attempts so a
    /// garbled frame does not consume the whole timeout in one wait.
    static const unsigned long RECEIVE_ATTEMPTS_PER_TIMEOUT = 4;

    /**
     * @param turboSerial RS-485 transport, already constructed.
     * @param address     Pump bus address, 0-999. Default 1.
     * @param delayFunc   Called to wait between retries.
     */
    explicit PfeifferSerialTC80(RS485Serial_Base &turboSerial,
                                const uint16_t address = 1,
                                DelayFunc delayFunc = pfeifferDefaultDelay);

    /**
     * @brief As above, and routes library logging to @p debug.
     *
     * Equivalent to calling PfeifferVacProtocol::setLogStream(debug), which
     * only has an effect in a PFTC80_LOG_TO_STREAM build. The sink is
     * library-wide, not per instance: with several pumps, the last one
     * constructed wins.
     */
    PfeifferSerialTC80(RS485Serial_Base &turboSerial,
                       const uint16_t address,
                       Stream &debug,
                       DelayFunc delayFunc = pfeifferDefaultDelay);

    /// Opens the underlying port. RS485Serial handles pin setup and direction.
    void begin(unsigned long baud = 9600, uint32_t config = SERIAL_8N1);

    /// Sets the pump bus address. Values above 999 do not fit the 3-digit
    /// address field and are rejected, leaving the current address in place.
    void setAddress(const uint16_t address);

    /// The address as it appears on the wire, e.g. "001".
    const char *getAddress() const { return _address; }

    // === Sending ===

    /// Write a pre-built telegram, appending the terminating carriage return.
    void sendTelegramRaw(const char *telegram);

    /// Render a telegram to the log without sending it.
    void printOutgoingTelegram(const char action, const uint16_t parameter, const char *data = nullptr);

    /// Build and send a telegram from action, parameter, and data.
    void sendTelegram(const char action, const uint16_t parameter, const char *data = nullptr, const bool debugPrint = false);

    /// Ask the pump for the value of a parameter.
    void sendQuery(const uint16_t parameter, const bool debugPrint = false);

    /// Set a parameter from an already-encoded value payload.
    void sendCommand(const uint16_t parameter, const char *data, const bool debugPrint = false);

    // One overload per protocol datatype; each encodes and sends its value/data payload.
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::BooleanOld data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::UInteger data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::UReal data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::String6 data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::BooleanNew data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::UShortInt data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::String16 data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::String8 data, const bool debugPrint = false);
    void sendCommand(const uint16_t parameter, const PfeifferVacProtocol::UExpoNew data, const bool debugPrint = false);

    // === Receiving ===

    /**
     * @brief Read one carriage-return-terminated frame.
     *
     * Returns as soon as a CR arrives, so @p timeout is an upper bound rather
     * than a fixed wait. The default allows for a full 30-character telegram at
     * 9600 baud (~31 ms) plus turnaround;
     *
     * @return The frame without its CR, or an empty String on timeout.
     */
    String receiveTelegramRaw(unsigned long timeout = 200);

    /// Read and decode one telegram. Check the result's `error` field.
    PfeifferVacProtocol::PfeifferTelegram receiveTelegram(bool debugPrint = false, unsigned long timeout = 5000);

    /// True when the telegram is a clean reply from this pump for @p expectedParameter.
    bool isValidResponse(const PfeifferVacProtocol::PfeifferTelegram &telegram, uint16_t expectedParameter) const;

    /**
     * Typed reads. Each waits up to @p timeout for a valid response to
     * @p expectedParameter, sets @p isValid, and returns a zero/empty value if
     * none arrived. Pair each with the sendQuery() for the same parameter.
     */
    boolean receiveBooleanOld(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    unsigned long receiveUInteger(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    float receiveUReal(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    boolean receiveBooleanNew(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    uint16_t receiveUShortInt(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    float receiveUExpoNew(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    String receiveString6(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    String receiveString16(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);
    String receiveString8(uint16_t expectedParameter, bool &isValid, bool debugPrint = true, unsigned long timeout = 1000);

    // === Error reporting ===

    /**
     * @brief Query the pump's current error code and translate it.
     * @return Human-readable text, or an empty String if there is no error, the
     *         code is unknown, or the query failed. Always empty when built
     *         with PFEIFFER_VAC_PROTOCOL_INCLUDE_DEBUG at 0.
     */
    String queryLatestError(bool &isValid, bool debugPrint = false, unsigned long timeout = 1000);

    /**
     * @brief Walk the pump's error history (ErrHist1..ErrHist10).
     * @return Comma-separated codes, each followed by its description in a
     *         debug build. Stops at the first "000000" entry.
     */
    String queryErrorHistory(bool &isValid, bool debugPrint = false, unsigned long timeout = 1000);

private:
    /**
     * @brief Shared retry loop behind every typed receive*().
     *
     * Waits for a response to @p expectedParameter whose payload is exactly
     * @p expectedDataLength characters, retrying until @p timeout expires or
     * the delay callback asks to abort.
     *
     * @return The payload, or an empty String when @p isValid comes back false.
     */
    String receiveValidatedPayload(uint16_t expectedParameter, unsigned int expectedDataLength,
                                   bool &isValid, bool debugPrint, unsigned long timeout);

    /// Explain, at debug level, why a response was rejected.
    void printInvalidResponseDebug(const PfeifferVacProtocol::PfeifferTelegram &telegram, uint16_t expectedParameter) const;

    RS485Serial_Base &_turboSerial;
    DelayFunc _delayFunc;
    char _address[4]; // 3 digits + null terminator address of the turbo pump
};
