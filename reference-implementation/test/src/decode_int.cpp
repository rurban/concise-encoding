#include "decode_helpers.h"


DEFINE_ROUNDTRIP_TEST(DecodeInt, int_0,         {0x00})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_1,         {0x01})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_103,       {0x67})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n1,        {0xff})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n104,      {0x98})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_104,       {TYPE_INT_16, 0x68, 0x00})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n105,      {TYPE_INT_16, 0x97, 0xff})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_127,       {TYPE_INT_16, 0x7f, 0x00})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n128,      {TYPE_INT_16, 0x80, 0xff})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_7fff,      {TYPE_INT_16, 0xff, 0x7f})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n8000,     {TYPE_INT_16, 0x00, 0x80})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_8000,      {TYPE_INT_32, 0x00, 0x80, 0x00, 0x00})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n8001,     {TYPE_INT_32, 0xff, 0x7f, 0xff, 0xff})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_7fffffff,  {TYPE_INT_32, 0xff, 0xff, 0xff, 0x7f})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n80000000, {TYPE_INT_32, 0x00, 0x00, 0x00, 0x80})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_80000000,  {TYPE_INT_64, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n80000001, {TYPE_INT_64, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_7fffffffffffffff,  {TYPE_INT_64, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f})
DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n8000000000000000, {TYPE_INT_64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80})
// DEFINE_ROUNDTRIP_TEST(DecodeInt, int_10000000000000000, {TYPE_INT_128, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
// DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n10000000000000000, {TYPE_INT_128, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff})
// DEFINE_ROUNDTRIP_TEST(DecodeInt, int_7fffffffffffffffffffffffffffffff, {TYPE_INT_128, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f})
// DEFINE_ROUNDTRIP_TEST(DecodeInt, int_n80000000000000000000000000000000, {TYPE_INT_128, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80})