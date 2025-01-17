#include "helpers/test_helpers.h"

// #define KSLogger_LocalLevel DEBUG
#include "kslogger.h"

using namespace enc;

#define TEST_ENCODE_DECODE_SHRINKING_TIME(YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, USEC, ...) \
TEST(Time, time_ ## YEAR ## _ ## MONTH ## _ ## DAY ## _ ## HOUR ## _ ## MINUTE ## _ ## SECOND ## _ ## USEC) \
{ \
    cte_test::expect_encode_decode_with_shrinking_buffer_size(0, \
        smtime(YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, USEC), \
         __VA_ARGS__); \
}


#define TEST_ENCODE_DECODE_SHRINKING_INT128(NAME, HIGH, LOW, ...) \
TEST_ENCODE_DECODE_SHRINKING(Int128, NAME, 0, i128(HIGH, LOW),  __VA_ARGS__) \

#define TEST_ENCODE_DECODE_SHRINKING_INT128_SIMPLE(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Int128, NAME, 0, i128(VALUE),  __VA_ARGS__) \

#define TEST_ENCODE_DECODE_SHRINKING_INT64(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Int64, NAME, 0, i64(VALUE),  __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKING_INT128_SIMPLE(NAME, VALUE, __VA_ARGS__)

#define TEST_ENCODE_DECODE_SHRINKING_INT32(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Int32,NAME, 0, i32(VALUE),  __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKING_INT64(NAME, VALUE, __VA_ARGS__)

#define TEST_ENCODE_DECODE_SHRINKING_INT16(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Int16,NAME, 0, i16(VALUE),  __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKING_INT32(NAME, VALUE, __VA_ARGS__)

#define TEST_ENCODE_DECODE_SHRINKING_INT8(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Int8, NAME, 0, i8(VALUE),   __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKING_INT16(NAME, VALUE, __VA_ARGS__)


#define TEST_ENCODE_DECODE_SHRINKING_FLOAT128(NAME, PRECISION, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Float128, NAME, 0, f128(PRECISION, VALUE),   __VA_ARGS__)

#define TEST_ENCODE_DECODE_SHRINKING_FLOAT64(NAME, PRECISION, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Float64, NAME, 0, f64(PRECISION, VALUE),   __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKING_FLOAT128(NAME, PRECISION, VALUE, __VA_ARGS__)

#define TEST_ENCODE_DECODE_SHRINKING_FLOAT32(NAME, PRECISION, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Float32, NAME, 0, f32(PRECISION, VALUE),   __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKING_FLOAT64(NAME, PRECISION, VALUE, __VA_ARGS__)


#define TEST_ENCODE_DECODE_SHRINKINGdec128_ct(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Decimal128, NAME, 0, d128(VALUE),  __VA_ARGS__) \

#define TEST_ENCODE_DECODE_SHRINKINGdec64_ct(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Decimal64, NAME, 0, d64(VALUE),  __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKINGdec128_ct(NAME, VALUE, __VA_ARGS__)

#define TEST_ENCODE_DECODE_SHRINKINGdec32_ct(NAME, VALUE, ...) \
TEST_ENCODE_DECODE_SHRINKING(Decimal32, NAME, 0, d32(VALUE),  __VA_ARGS__) \
TEST_ENCODE_DECODE_SHRINKINGdec64_ct(NAME, VALUE, __VA_ARGS__)


// ==================================================================

TEST_ENCODE_DECODE_SHRINKING(Nil, nil, 0, nil(), "nil")

TEST_ENCODE_DECODE_SHRINKING(Bool, false, 0, bl(false), "false")
TEST_ENCODE_DECODE_SHRINKING(Bool, true,  0, bl(true), "true")

TEST_ENCODE_DECODE_SHRINKING_INT8(p_0,                                       0, "0")
TEST_ENCODE_DECODE_SHRINKING_INT8(p_1,                                       1, "1")
TEST_ENCODE_DECODE_SHRINKING_INT8(p_109,                                   109, "109")
TEST_ENCODE_DECODE_SHRINKING_INT8(n_1,                                     -1, "-1")
TEST_ENCODE_DECODE_SHRINKING_INT8(n_109,                                 -109, "-109")
TEST_ENCODE_DECODE_SHRINKING_INT8(p_110,                                   110, "110")
TEST_ENCODE_DECODE_SHRINKING_INT8(n_110,                                 -110, "-110")
TEST_ENCODE_DECODE_SHRINKING_INT16(n_1111,                              -1111, "-1111")
TEST_ENCODE_DECODE_SHRINKING_INT32(p_9999999,                          9999999, "9999999")
TEST_ENCODE_DECODE_SHRINKING_INT64(p_99999999999999,            99999999999999, "99999999999999")

TEST_DECODE_DATA(Hex,   p_ff,              99, 99, i8(0xff),                "h:ff")
TEST_DECODE_DATA(Hex,   n_0f,              99, 99, i8(-0x0f),               "h:-0f")
TEST_DECODE_DATA(Hex,   p_abc,             99, 99, i16(0xabc),              "h:abc")
TEST_DECODE_DATA(Hex,   p_1fe9a00,         99, 99, i32(0x1fe9a00),          "h:1fe9a00")
TEST_DECODE_DATA(Hex,   n_59aabe74fa13304, 99, 99, i64(-0x59aabe74fa13304), "h:-59aabe74fa13304")

TEST_DECODE_DATA(Bin,   p_0,               99, 99, i8(0),                   "b:0")
TEST_DECODE_DATA(Bin,   p_1,               99, 99, i8(1),                   "b:1")
TEST_DECODE_DATA(Bin,   p_10,              99, 99, i8(2),                   "b:10")
TEST_DECODE_DATA(Bin,   n_101110100101001, 99, 99, i32(-23849),             "b:-101110100101001")

TEST_DECODE_DATA(Octal, p_0,               99, 99, i8(0),                   "o:0")
TEST_DECODE_DATA(Octal, n_7,               99, 99, i8(-7),                  "o:-7")
TEST_DECODE_DATA(Octal, p_1353365,         99, 99, i32(382709),             "o:1353365")

TEST_ENCODE_DECODE_DATA(Float, _0_1, 99, 99, 0, f32(5, 0.1), "0.1")


// TEST_ENCODE_DECODE_SHRINKING_FLOAT32(_0_1,        5, 0.1,         "0.1")
// TEST_ENCODE_DECODE_SHRINKING_FLOAT32(n967234_125, -967234.125, {0x72, 0x22, 0x24, 0x6c, 0xc9})
// TEST_ENCODE_DECODE_SHRINKING_FLOAT64(_1_0123,     1.0123,      {0x73, 0x51, 0xda, 0x1b, 0x7c, 0x61, 0x32, 0xf0, 0x3f})
// TEST_ENCODE_DECODE_SHRINKING_FLOAT128(_1_01234,   1.01234l,    {0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xba, 0x7e, 0xc1, 0x6e, 0xd8, 0xb6, 0x28, 0x03, 0xff, 0x3f})

// TEST_ENCODE_DECODE_SHRINKINGdec32_ct(_0,   0.0df, {0x75, 0x00, 0x00, 0x00, 0x32})
// TEST_ENCODE_DECODE_SHRINKINGdec32_ct(_0_5, 0.5df, {0x75, 0x05, 0x00, 0x00, 0x32})
// TEST_ENCODE_DECODE_SHRINKINGdec32_ct(_0_1, 0.1df, {0x75, 0x01, 0x00, 0x00, 0x32})
// TEST_ENCODE_DECODE_SHRINKINGdec64_ct(_1000000_000001, 1000000.000001dd, {0x76, 0x01, 0x10, 0xa5, 0xd4, 0xe8, 0x00, 0x00, 0x31})
// TEST_ENCODE_DECODE_SHRINKINGdec128_ct(_1000000000000_000000000001, 1000000000000.000000000001dl, {0x77, 0x01, 0x00, 0x00, 0xa1, 0xed, 0xcc, 0xce, 0x1b, 0xc2, 0xd3, 0x00, 0x00, 0x00, 0x00, 0x28, 0x30})

// TEST_ENCODE_DECODE_SHRINKING(Time, zero, 0, smtime(0), {0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
TEST_ENCODE_DECODE_SHRINKING_TIME(1955, 11,  5,  8, 21,  0,      0, "1955-11-05T08:21:00Z")
// TEST_ENCODE_DECODE_SHRINKING_TIME(2015, 10, 21, 14, 28,  9, 714000, {0x78, 0x10, 0xe5, 0x9a, 0x70, 0xce, 0xe4, 0xf7, 0x01})
// TEST_ENCODE_DECODE_SHRINKING_TIME(1985, 10, 26,  8, 22, 16, 900142, {0x78, 0x2e, 0xbc, 0x0d, 0x59, 0x68, 0x65, 0xf0, 0x01})


// TEST_STOP_IN_CALLBACK(SIC, nil, nil())
// TEST_STOP_IN_CALLBACK(SIC, bool, bl(false))
// TEST_STOP_IN_CALLBACK(SIC, int8, i8(1))
// TEST_STOP_IN_CALLBACK(SIC, int16, i16(1))
// TEST_STOP_IN_CALLBACK(SIC, int32, i32(1))
// TEST_STOP_IN_CALLBACK(SIC, int64, i64(1))
// TEST_STOP_IN_CALLBACK(SIC, int128, i128(0, 1))
// TEST_STOP_IN_CALLBACK(SIC, float32, f32(1))
// TEST_STOP_IN_CALLBACK(SIC, float64, f64(1))
// TEST_STOP_IN_CALLBACK(SIC, float128, f128(1))
// TEST_STOP_IN_CALLBACK(SIC, decimal32, d32(1))
// TEST_STOP_IN_CALLBACK(SIC, decimal64, d64(1))
// TEST_STOP_IN_CALLBACK(SIC, decimal128, d128(1))
// TEST_STOP_IN_CALLBACK(SIC, smalltime, smtime(1))
// TEST_STOP_IN_CALLBACK(SIC, list, list()->end())
// TEST_STOP_IN_CALLBACK(SIC, map, map()->end())
// TEST_STOP_IN_CALLBACK(SIC, string, str("test"))
// TEST_STOP_IN_CALLBACK(SIC, binary, bin(std::vector<uint8_t>()))
