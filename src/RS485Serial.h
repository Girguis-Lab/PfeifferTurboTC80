#pragma once

/**
 * @file RS485Serial.h
 * @brief Half-duplex RS-485 transport used by PfeifferSerialTC80.
 *
 * RS485Serial_Base is the abstract interface the pump driver talks to; derive
 * from it to drive a transceiver this library does not cover (a SoftwareSerial
 * link, a USB-RS485 bridge, a test double). RS485HardwareSerial is the stock
 * implementation, wrapping a HardwareSerial plus DE/RE direction pins.
 *
 * The base derives from Print, so print()/println() and their formatting
 * overloads work through a RS485Serial_Base& for free. Only write() needs
 * implementing.
 */

#include <Arduino.h>
#include <RingBufCPP.h>

/// Passed for a RS485 converter Driver Enable (DE) or Receiver Enable (RE) pin that is not wired. Pin 0 is a real pin on most
/// boards, so it cannot double as the "absent" sentinel.
#define RS485_PIN_UNUSED 255

/**
 * @brief Abstract half-duplex serial interface with explicit direction control.
 *
 * Every method is pure virtual: the vtable is emitted in each translation unit
 * that needs it, and there is no key function to leave undefined.
 */
class RS485Serial_Base : public Print
{
public:
    RS485Serial_Base() {}

    /// Required so deleting a derived object through a base pointer is defined.
    virtual ~RS485Serial_Base() {}

    // Setup / teardown
    virtual void begin(unsigned long baud = 9600, uint32_t config = SERIAL_8N1) = 0;
    virtual void end() = 0;

    // Direction control (explicit override if needed)
    virtual void enterReceiveMode() = 0;
    virtual void enterSendMode() = 0;

    /**
     * @brief Cooperative task function. Call frequently (e.g. each loop()) to move
     *        incoming bytes out of the RS485 converter serial port UART and into the software RX buffer.
     */
    virtual void task() = 0;

    // --- RX API ---
    virtual int available() = 0;
    virtual int availableForWrite() = 0;
    virtual int read() = 0;

    // --- Write API (blocking: drives DE, writes, waits for the shift register
    //     to drain, then releases the bus) ---
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    virtual size_t write(uint8_t b) = 0;

    // Print declares write(const char*) and friends; naming write() above
    // would otherwise hide them.
    using Print::write;
};

/**
 * @brief RS-485 transport over a HardwareSerial with DE/RE direction pins.
 *
 * @tparam RxBufferSize Bytes of RAM reserved for the receive FIFO. The longest
 *         Pfeiffer telegram is 30 bytes, so the 64-byte default holds two full
 *         frames. Raise it if your loop() calls task() infrequently; check
 *         rxOverflowCount() if replies look truncated.
 */
template <size_t RxBufferSize = 64>
class RS485HardwareSerial : public RS485Serial_Base
{
public:
    /**
     * @param serial               Underlying arduino UART.
     * @param pin485SendEnable     DE (Driver Enable) pin, active high. Defaults to RS485_PIN_UNUSED if absent.
     * @param pin485ReceiveDisable RE (Reciver Enable) pin, Both DE and RE are driven high/low together matching the style of a combined DE/RE
     *                             RS485 converter. Defaults to RS485_PIN_UNUSED if absent.
     * @param readTimeoutMs        Upper bound on how long one ingest pass may
     *                             spend draining the arduino UART, so a flooded bus
     *                             cannot stall the caller indefinitely.
     */
    RS485HardwareSerial(
        HardwareSerial &serial,
        uint8_t pin485SendEnable,
        uint8_t pin485ReceiveDisable,
        unsigned long readTimeoutMs = 6)
        : serial(serial),
          _pin485SendEnable(pin485SendEnable),
          _pin485ReceiveDisable(pin485ReceiveDisable),
          _readTimeoutMs(readTimeoutMs),
          _rxOverflowCount(0),
          _sendMode(false)
    {
    }

    // === Setup / teardown ===

    void begin(unsigned long baud = 9600, uint32_t config = SERIAL_8N1) override
    {
        if (_pin485SendEnable != RS485_PIN_UNUSED)
            pinMode(_pin485SendEnable, OUTPUT);
        if (_pin485ReceiveDisable != RS485_PIN_UNUSED)
            pinMode(_pin485ReceiveDisable, OUTPUT);
        enterReceiveMode();         // start listening, never driving the bus
        serial.begin(baud, config); // hardware serial with config
    }

    void end() override
    {
        serial.end();
    }

    // === Data flow direction control (allows explicit override if needed) ===

    void enterReceiveMode() override
    {
        // Block until the last outgoing byte has left the shift register,
        // otherwise dropping DE truncates it mid-character.
        serial.flush();

        _sendMode = false;
        if (_pin485SendEnable != RS485_PIN_UNUSED)
            digitalWrite(_pin485SendEnable, LOW);
        if (_pin485ReceiveDisable != RS485_PIN_UNUSED)
            digitalWrite(_pin485ReceiveDisable, LOW);
    }

    void enterSendMode() override
    {
        // Capture anything still in flight before we take over the bus.
        ingestRxBytes();

        _sendMode = true;
        if (_pin485SendEnable != RS485_PIN_UNUSED)
            digitalWrite(_pin485SendEnable, HIGH);
        if (_pin485ReceiveDisable != RS485_PIN_UNUSED)
            digitalWrite(_pin485ReceiveDisable, HIGH);
    }

    // === Cooperative task function ===

    void task() override
    {
        // Only drain while listening; during a send the Arduino UART's RX side sees
        // our own transmission on a half-duplex pair.
        if (!_sendMode)
            ingestRxBytes();
    }

    // === RX methods ===

    /**
     * @brief Bytes available to read().
     *
     * Ingests first so the count reflects what read() can actually return;
     * without that, bytes still sitting in the UART would be counted even
     * though read() drains only the ring buffer.
     */
    int available() override
    {
        ingestRxBytes();
        return static_cast<int>(rxBuffer.numElements());
    }

    int availableForWrite() override
    {
        return serial.availableForWrite();
    }

    /**
     * @brief Remove and return the oldest buffered byte.
     * @return The byte (0-255), or -1 when nothing is buffered.
     */
    int read() override
    {
        ingestRxBytes();
        uint8_t readByte = 0;
        if (!rxBuffer.pull(&readByte))
            return -1; // empty: must be -1, not a truncated 0xFF
        return static_cast<int>(readByte);
    }

    /** @brief Discard buffered input, e.g. to resync after a malformed frame. */
    void flushInput()
    {
        ingestRxBytes();
        uint8_t discard = 0;
        while (rxBuffer.pull(&discard))
        {
        }
    }

    // === TX methods ===

    /// Multiple byte write (blocking). Holds the bus for the whole buffer.
    size_t write(const uint8_t *bytes, size_t size) override
    {
        enterSendMode();
        size_t written = serial.write(bytes, size);
        enterReceiveMode();
        return written;
    }

    /// Single byte write (blocking). Prefer the buffer overload for a telegram:
    /// one call per byte releases the bus between characters.
    size_t write(uint8_t b) override
    {
        enterSendMode();
        size_t written = serial.write(b);
        enterReceiveMode();
        return written;
    }

    using RS485Serial_Base::write;

    // === Diagnostics ===

    /**
     * @brief Bytes discarded because the RX buffer was full.
     *
     * Non-zero means task() is not being called often enough, or RxBufferSize
     * is too small for your loop timing. Dropped bytes corrupt telegrams.
     */
    unsigned long rxOverflowCount() const { return _rxOverflowCount; }

    /**
     * @brief Move bytes from the Arduino UART into the software RX buffer.
     *
     * Bounded by readTimeoutMs so a continuously flooded bus cannot spin here
     * forever; leftover bytes are picked up on the next call.
     *
     * @return Number of bytes moved.
     */
    size_t ingestRxBytes()
    {
        unsigned long startTime = millis();
        size_t bytesRead = 0;
        while (serial.available())
        {
            int c = serial.read();
            if (c < 0)
                break;
            if (!rxBuffer.add(static_cast<uint8_t>(c)))
                _rxOverflowCount++; // buffer was full: this byte is lost
            bytesRead++;
            if (millis() - startTime > _readTimeoutMs)
                break;
        }
        return bytesRead;
    }

    HardwareSerial &serial;

private:
    uint8_t _pin485SendEnable;
    uint8_t _pin485ReceiveDisable;
    unsigned long _readTimeoutMs;
    unsigned long _rxOverflowCount;
    RingBufCPP<uint8_t, RxBufferSize> rxBuffer;

    // RS485 state
    bool _sendMode;
};
