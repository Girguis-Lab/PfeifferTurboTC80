/*
  Host-side tests for the Pfeiffer Vacuum Protocol layer.

  Covers telegram encoding, decoding, and the payload datatypes - all pure
  logic, no hardware. Run with:  pio test -e native

  The transport and driver are not covered here; they need a real UART and are
  exercised by the example builds in CI.
*/

#include <unity.h>
#include "PfeifferVacProtocolFuncs.h"
#include "PfeifferVacProtocolDatatypes.h"

using namespace PfeifferVacProtocol;

void setUp(void) {}
void tearDown(void) {}

// ---------------------------------------------------------------------------
// encodeTelegram
// ---------------------------------------------------------------------------

/*
  A query for parameter 309 from address 1. Layout is
  address(3) action(1) '0'(1) parameter(3) length(2) payload checksum(3),
  and the checksum is the sum of every preceding character, modulo 256.
*/
void test_encode_query(void)
{
    String frame = encodeTelegram("001", '0', 309, "=?");
    // Body "0010030902=?" sums to 619; 619 % 256 = 107.
    TEST_ASSERT_EQUAL_STRING("0010030902=?107", frame.c_str());
}

void test_encode_command(void)
{
    String frame = encodeTelegram("001", '1', 10, "111111");
    // 001 1 0 010 06 111111 + checksum
    TEST_ASSERT_EQUAL_STRING("0011001006111111", frame.substring(0, 16).c_str());
    TEST_ASSERT_EQUAL(19, frame.length());
}

/// A telegram with no payload still carries a "00" length field.
void test_encode_no_data(void)
{
    String frame = encodeTelegram("001", '0', 309, nullptr);
    TEST_ASSERT_EQUAL(13, frame.length());
    TEST_ASSERT_EQUAL_STRING("00", frame.substring(8, 10).c_str());
}

/// The checksum is taken modulo 256 and rendered as three digits.
void test_encode_checksum_is_modulo_256(void)
{
    String frame = encodeTelegram("001", '0', 309, "=?");
    uint16_t sum = 0;
    for (size_t i = 0; i < frame.length() - 3; ++i)
        sum += (uint16_t)frame[i];
    TEST_ASSERT_EQUAL(sum % 256, frame.substring(frame.length() - 3).toInt());
}

/*
  The parameter field is three digits wide. Anything larger used to be
  truncated silently into a well-formed but wrong telegram.
*/
void test_encode_rejects_out_of_range_parameter(void)
{
    TEST_ASSERT_EQUAL(0, encodeTelegram("001", '0', 1000, "=?").length());
    TEST_ASSERT_EQUAL(0, encodeTelegram("001", '0', 65535, "=?").length());
    TEST_ASSERT_TRUE(encodeTelegram("001", '0', 999, "=?").length() > 0);
}

void test_encode_rejects_null_address(void)
{
    TEST_ASSERT_EQUAL(0, encodeTelegram(nullptr, '0', 309, "=?").length());
}

// ---------------------------------------------------------------------------
// decodeTelegram
// ---------------------------------------------------------------------------

/// A well-formed reply round-trips back into its fields.
void test_decode_valid_response(void)
{
    String frame = encodeTelegram("001", '1', 309, "082350");
    PfeifferTelegram t = decodeTelegram(frame.c_str());

    TEST_ASSERT_EQUAL(TelegramError::None, t.error);
    TEST_ASSERT_EQUAL_STRING("001", t.address.c_str());
    TEST_ASSERT_EQUAL('1', t.action);
    TEST_ASSERT_EQUAL_STRING("309", t.parameter.c_str());
    TEST_ASSERT_EQUAL_STRING("082350", t.data.c_str());
}

void test_decode_detects_bad_checksum(void)
{
    String frame = encodeTelegram("001", '1', 309, "082350");
    frame.setCharAt(frame.length() - 1, frame[frame.length() - 1] == '0' ? '1' : '0');
    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram(frame.c_str()).error);
}

/*
  Regression: the minimum valid telegram is 13 bytes. The old bound of 8 let
  substring(len - 3, len) overlap the parameter field, so a runt frame was
  scored against its own parameter digits.
*/
void test_decode_rejects_short_telegram(void)
{
    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram("").error);
    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram("00100").error);
    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram("001003090").error);
    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram("001003090210").error); // 12
}

void test_decode_rejects_null(void)
{
    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram(nullptr).error);
}

/// Nothing received leaves the fields empty, which is how callers tell an
/// absent reply from a corrupt one.
void test_decode_empty_leaves_fields_empty(void)
{
    PfeifferTelegram t = decodeTelegram("");
    TEST_ASSERT_EQUAL(0, t.address.length());
    TEST_ASSERT_EQUAL(0, t.parameter.length());
}

/*
  Regression: the declared payload length must match the frame length. Without
  this a truncated frame could pass on a coincidental checksum match.
*/
void test_decode_rejects_length_mismatch(void)
{
    // Claims 6 payload bytes but carries 3.
    String frame = "0011030906123";
    uint16_t sum = 0;
    for (size_t i = 0; i < frame.length(); ++i)
        sum += (uint16_t)frame[i];
    char checksum[4];
    snprintf(checksum, sizeof(checksum), "%03u", (unsigned)(sum % 256));
    frame += checksum;

    TEST_ASSERT_EQUAL(TelegramError::InvalidChecksum, decodeTelegram(frame.c_str()).error);
}

/// The pump reports these three conditions in the payload rather than as a
/// malformed frame.
void test_decode_pump_error_replies(void)
{
    TEST_ASSERT_EQUAL(TelegramError::InvalidParameter,
                      decodeTelegram(encodeTelegram("001", '1', 309, "NO_DEF").c_str()).error);
    TEST_ASSERT_EQUAL(TelegramError::OutOfRange,
                      decodeTelegram(encodeTelegram("001", '1', 309, "_RANGE").c_str()).error);
    TEST_ASSERT_EQUAL(TelegramError::LogicError,
                      decodeTelegram(encodeTelegram("001", '1', 309, "_LOGIC").c_str()).error);
}

// ---------------------------------------------------------------------------
// Datatypes
// ---------------------------------------------------------------------------

void test_boolean_old(void)
{
    TEST_ASSERT_EQUAL_STRING("111111", BooleanOld(true).encode().c_str());
    TEST_ASSERT_EQUAL_STRING("000000", BooleanOld(false).encode().c_str());
    TEST_ASSERT_TRUE(BooleanOld("111111").decode());
    TEST_ASSERT_FALSE(BooleanOld("000000").decode());
}

void test_boolean_new(void)
{
    TEST_ASSERT_EQUAL_STRING("1", BooleanNew(true).encode().c_str());
    TEST_ASSERT_EQUAL_STRING("0", BooleanNew(false).encode().c_str());
    TEST_ASSERT_TRUE(BooleanNew("1").decode());
    TEST_ASSERT_FALSE(BooleanNew("0").decode());
}

void test_u_integer(void)
{
    TEST_ASSERT_EQUAL_STRING("082350", UInteger(82350UL).encode().c_str());
    TEST_ASSERT_EQUAL_STRING("000000", UInteger(0UL).encode().c_str());
    TEST_ASSERT_EQUAL(82350UL, UInteger("082350").decode());
    // Leading zeros must not be read as octal.
    TEST_ASSERT_EQUAL(9UL, UInteger("000009").decode());
}

/// u_real is fixed point with two implied decimals: 001571 is 15.71.
void test_u_real(void)
{
    TEST_ASSERT_EQUAL_STRING("001571", UReal(15.71f).encode().c_str());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 15.71f, UReal("001571").decode());
}

void test_u_short_int(void)
{
    TEST_ASSERT_EQUAL_STRING("085", UShortInt((uint16_t)85).encode().c_str());
    TEST_ASSERT_EQUAL(85, UShortInt("085").decode());
}

void test_strings(void)
{
    TEST_ASSERT_EQUAL_STRING("TC80  ", String6("TC80  ").encode().c_str());
    TEST_ASSERT_EQUAL_STRING("ABCDEFGH", String8("ABCDEFGH").encode().c_str());
    TEST_ASSERT_EQUAL_STRING("0123456789ABCDEF", String16("0123456789ABCDEF").encode().c_str());
    // Oversized input is truncated to the field width, not overrun.
    TEST_ASSERT_EQUAL(6, String6("TOOLONGVALUE").encode().length());
    TEST_ASSERT_EQUAL(8, String8("TOOLONGVALUE1234").encode().length());
}

/*
  The two worked examples from the specification. MMMM is the mantissa scaled
  by 1000 and XX is the exponent plus 20, so 100023 is 1.000 * 10^(23-20).

  Regression: these used to be a factor of 10 out. The mantissa was scaled by
  100 and encode() normalised into [1, 100) to match, so round-trips looked
  correct while every value exchanged with the pump was wrong. Assert the
  absolute values, not just the round-trip, or that can recur undetected.
*/
void test_u_expo_new_matches_spec_examples(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1000.0f, UExpoNew("100023").decode());
    TEST_ASSERT_FLOAT_WITHIN(1e-22f, 1.0e-20f, UExpoNew("100000").decode());
}

void test_u_expo_new_roundtrip(void)
{
    TEST_ASSERT_EQUAL_STRING("100023", UExpoNew(UExpoNew("100023").decode()).encode().c_str());
    TEST_ASSERT_EQUAL_STRING("123425", UExpoNew(UExpoNew("123425").decode()).encode().c_str());
}

void test_u_expo_new_zero(void)
{
    TEST_ASSERT_EQUAL_STRING("000000", UExpoNew(0.0f).encode().c_str());
}

// ---------------------------------------------------------------------------

int main(int, char **)
{
    UNITY_BEGIN();

    RUN_TEST(test_encode_query);
    RUN_TEST(test_encode_command);
    RUN_TEST(test_encode_no_data);
    RUN_TEST(test_encode_checksum_is_modulo_256);
    RUN_TEST(test_encode_rejects_out_of_range_parameter);
    RUN_TEST(test_encode_rejects_null_address);

    RUN_TEST(test_decode_valid_response);
    RUN_TEST(test_decode_detects_bad_checksum);
    RUN_TEST(test_decode_rejects_short_telegram);
    RUN_TEST(test_decode_rejects_null);
    RUN_TEST(test_decode_empty_leaves_fields_empty);
    RUN_TEST(test_decode_rejects_length_mismatch);
    RUN_TEST(test_decode_pump_error_replies);

    RUN_TEST(test_boolean_old);
    RUN_TEST(test_boolean_new);
    RUN_TEST(test_u_integer);
    RUN_TEST(test_u_real);
    RUN_TEST(test_u_short_int);
    RUN_TEST(test_strings);
    RUN_TEST(test_u_expo_new_matches_spec_examples);
    RUN_TEST(test_u_expo_new_roundtrip);
    RUN_TEST(test_u_expo_new_zero);

    return UNITY_END();
}
