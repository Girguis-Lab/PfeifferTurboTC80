#pragma once

/**
 * @file PfeifferVacProtocolDatatypes.h
 * @brief The nine Pfeiffer Vacuum Protocol payload types.
 *
 * Each class wraps one wire format. Construct from a native value to send, or
 * from the 1/3/6/8/16-character ASCII field of a received telegram to read;
 * encode() gives the wire form, decode() the native value.
 */

#include <Arduino.h> // Used for String, boolean, etc.

namespace PfeifferVacProtocol
{

    // ---- Data types -----
    enum class DataType
    {
        BooleanOld = 0, // boolean_old
        UInteger = 1,   // u_integer
        UReal = 2,      // u_real
        String6 = 4,    // string (6 chars)
        BooleanNew = 6, // boolean_new
        UShortInt = 7,  // u_short_int
        String16 = 11,  // string16
        String8 = 12,   // string8
        UExpoNew = 10   // u_expo_new
    };

    // ---- Data Type Classes ----

    /**
     * @brief boolean_old (0) - Logical value (false/true)
     * Length: 6. Example: 000000=false; 111111=true
     */
    class BooleanOld
    {
    public:
        // Constructor from native type
        explicit BooleanOld(const bool val) : _value(val) {}

        // Constructor from ASCII string representation (6 chars)
        explicit BooleanOld(const char *str)
        {
            // Compares the first 6 characters to "111111"
            _value = (strncmp(str, "111111", 6) == 0);
        }

        // returns the ASCII representation of a BooleanOld (6 chars)
        String encode() const
        {
            return _value ? "111111" : "000000";
        }

        // returns a bool from the BooleanOld
        bool decode() const
        {
            return _value;
        }

    private:
        boolean _value;
    };

    /**
     * @brief u_integer (1) - Positive whole number
     * Length: 6. Example: 000000 to 999999
     */
    class UInteger
    {
    public:
        // Constructor from native type
        explicit UInteger(unsigned long val) : _value(val) {}

        // Constructor from ASCII string representation (6 chars)
        explicit UInteger(const char *str)
        {
            _value = (unsigned long)strtoul(str, NULL, 10);
        }

        // returns the ASCII representation of an UInteger (6 chars)
        String encode() const
        {
            // Pad with leading zeros to 6 digits
            char buf[7];
            snprintf(buf, sizeof(buf), "%06lu", _value);
            return String(buf);
        }

        // returns an unsigned long from the UInteger
        unsigned long decode() const
        {
            return _value;
        }

    private:
        unsigned long _value;
    };

    /**
     * @brief u_real (2) - Fixed point number (unsigned)
     * Length: 6. Example: 001571 corresponds with 15.71. Implies a fixed two-decimal-place structure.
     */
    class UReal
    {
    public:
        // Constructor from native type (float, representing the actual value)
        explicit UReal(float val) : _value(val) {}

        // Constructor from ASCII string representation (6 chars)
        explicit UReal(const char *str)
        {
            // Read as an integer (e.g., 001571 -> 1571) and divide by 100.0
            unsigned long temp_val = strtoul(str, NULL, 10);
            _value = (float)temp_val / 100.0f;
        }

        // returns the ASCII representation of an UReal (6 chars)
        String encode() const
        {
            // Multiply by 100 (e.g., 15.71 -> 1571) and format as 6-digit integer
            unsigned long temp_val = (unsigned long)(_value * 100.0f + 0.5f); // +0.5 for rounding
            char buf[7];
            snprintf(buf, sizeof(buf), "%06lu", temp_val);
            return String(buf);
        }

        // returns a float from the UReal
        float decode() const
        {
            return _value;
        }

    private:
        float _value;
    };

    /**
     * @brief string (4) - Any character string
     * Length: 6. ASCII codes between 32 and 127.
     */
    class String6
    {
    public:
        // Constructor from native type
        explicit String6(const char *val)
        {
            strncpy(_value, val, 6);
            _value[6] = '\0'; // Ensure null termination
        }

        // returns the ASCII representation of a String6 (6 chars)
        String encode() const
        {
            // Returns the string, up to 6 chars
            char buf[7] = {'\0'}; // Initialize with null terminators
            strncpy(buf, _value, 6);
            return String(buf);
        }

        // returns a C-string from the String6
        const char *decode() const
        {
            return _value;
        }

    private:
        char _value[7]; // 6 chars + null terminator
    };

    /**
     * @brief boolean_new (6) - Logical value (false/true)
     * Length: 1. Example: 0=false; 1=true
     */
    class BooleanNew
    {
    public:
        // Constructor from native type
        explicit BooleanNew(bool val) : _value(val) {}

        // Constructor from ASCII string representation (1 char)
        explicit BooleanNew(const char *str)
        {
            _value = (str[0] == '1');
        }

        // returns the ASCII representation of a BooleanNew (1 char)
        String encode() const
        {
            return _value ? "1" : "0";
        }

        // returns a bool from the BooleanNew
        bool decode() const
        {
            return _value;
        }

    private:
        boolean _value;
    };

    /**
     * @brief u_short_int (7) - Positive whole number
     * Length: 3. Example: 000 to 999
     */
    class UShortInt
    {
    public:
        // Constructor from native type
        explicit UShortInt(uint16_t val) : _value(val) {}

        // Constructor from ASCII string representation (3 chars)
        explicit UShortInt(const char *str)
        {
            _value = (uint16_t)strtoul(str, NULL, 10);
        }

        // returns the ASCII representation of an UShortInt (3 chars)
        String encode() const
        {
            // Pad with leading zeros to 3 digits
            char buf[4] = {'\0'};
            snprintf(buf, 4, "%03u", _value);
            return String(buf);
        }

        // returns an unsigned short from the UShortInt
        uint16_t decode() const
        {
            return _value;
        }

    private:
        uint16_t _value;
    };

    /**
     * @brief string16 (11) - Any character string
     * Length: 16. ASCII codes between 32 and 127.
     */
    class String16
    {
    public:
        // Constructor from native type
        explicit String16(const char *val)
        {
            strncpy(_value, val, 16);
            _value[16] = '\0'; // Ensure null termination
        }

        // returns the ASCII representation of a String16 (16 chars)
        String encode() const
        {
            // Returns the string, padded with spaces if necessary, up to 16 chars
            char buf[17] = {'\0'}; // Initialize with null terminators
            strncpy(buf, _value, 16);
            return String(buf);
        }

        // returns a C-string from the String16
        const char *decode() const
        {
            return _value;
        }

    private:
        char _value[17]; // 16 chars + null terminator
    };

    /**
     * @brief string8 (12) - Any character string
     * Length: 8. ASCII codes between 32 and 127.
     */
    class String8
    {
    public:
        // Constructor from native type
        explicit String8(const char *val)
        {
            strncpy(_value, val, 8);
            _value[8] = '\0'; // Ensure null termination
        }

        // returns the ASCII representation of a String8 (8 chars)
        String encode() const
        {
            // Returns the string, up to 8 chars
            char buf[9] = {'\0'}; // Initialize with null terminators
            strncpy(buf, _value, 8);
            return String(buf);
        }

        // returns a C-string from the String8
        const char *decode() const
        {
            return _value;
        }

    private:
        char _value[9]; // 8 chars + null terminator
    };

    /**
     * @brief u_expo_new (10) - Positive exponential number
     * Length: 6. Last two digits are the exponent with a deduction of 20.
     * Example: 100023 = 1.0 * 10^(23-20) = 1.0 * 10^3; 100000 = 1.0 * 10^(0-20) = 1.0 * 10^-20
     * Format: MMMMXX, where MMMM is the mantissa  (fixed point, 3 decimals) and XX is the exponent + 20.
     */
    class UExpoNew
    {
    public:
        // Constructor from native type (float, representing the actual value)
        explicit UExpoNew(float val) : _value(val) {}

        // Constructor from ASCII string representation (6 chars)
        explicit UExpoNew(const char *str)
        {
            // MMMM: str[0] to str[3] - Mantissa (with three decimals, e.g., 1000 -> 1.000)
            char mantissa_str[5];
            strncpy(mantissa_str, str, 4);
            mantissa_str[4] = '\0';
            float mantissa = (float)strtoul(mantissa_str, NULL, 10) / 1000.0f;

            // XX: str[4] to str[5] - Exponent + 20
            char exponent_str[3];
            strncpy(exponent_str, str + 4, 2);
            exponent_str[2] = '\0';
            int exponent_plus_20 = (int)strtoul(exponent_str, NULL, 10);
            int actual_exponent = exponent_plus_20 - 20;

            // Calculate value: mantissa * 10^actual_exponent
            _value = mantissa * pow(10.0f, actual_exponent);
        }

        // returns the ASCII representation of an UExpoNew (6 chars)
        String encode() const
        {
            // NOTE: Encoding a float back into this specific format can be complex
            // and may lose precision. A simplified approach is used here.
            // This implementation is a placeholder; a robust implementation would
            // require more complex math to find the best MMMM and XX.

            if (_value == 0.0f)
                return "000000"; // Special case for zero

            // Find an exponent 'exp' such that 1.00 <= mantissa < 10.00
            int actual_exponent = 0;
            float mantissa = _value;
            while (mantissa < 1.0f && actual_exponent > -20)
            {
                mantissa *= 10.0f;
                actual_exponent--;
            }
            while (mantissa >= 10.0f && actual_exponent < 79) // Limit to max practical exp (79=99-20)
            {
                mantissa /= 10.0f;
                actual_exponent++;
            }

            // MMMM is the mantissa * 1000 (rounded)
            uint16_t mantissa_mm = (uint16_t)(mantissa * 1000.0f + 0.5f);

            // XX is the actual_exponent + 20
            uint8_t exponent_xx = (uint8_t)(actual_exponent + 20);

            // Format: MMMM (4 digits) + XX (2 digits)
            char buf[7];
            snprintf(buf, sizeof(buf), "%04u%02u", mantissa_mm, exponent_xx);
            return String(buf);
        }

        // returns a float from the UExpoNew
        float decode() const
        {
            return _value;
        }

    private:
        float _value;
    };

} // namespace PfeifferVacProtocol
