#include "encode_test_helpers.h"

DEFINE_ADD_VALUE_TEST(Int32Test, add_0,         (int32_t)          0, {0x00})
DEFINE_ADD_VALUE_TEST(Int32Test, add_1,         (int32_t)          1, {0x01})
DEFINE_ADD_VALUE_TEST(Int32Test, add_101,       (int32_t)        103, {0x67})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n1,        (int32_t)         -1, {0xff})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n101,      (int32_t)       -103, {0x99})
DEFINE_ADD_VALUE_TEST(Int32Test, add_102,       (int32_t)        104, {TYPE_INT_16, 0x68, 0x00})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n102,      (int32_t)       -104, {TYPE_INT_16, 0x98, 0xff})
DEFINE_ADD_VALUE_TEST(Int32Test, add_127,       (int32_t)        127, {TYPE_INT_16, 0x7f, 0x00})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n128,      (int32_t)       -128, {TYPE_INT_16, 0x80, 0xff})
DEFINE_ADD_VALUE_TEST(Int32Test, add_7fff,      (int32_t)     0x7fff, {TYPE_INT_16, 0xff, 0x7f})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n8000,     (int32_t)    -0x8000, {TYPE_INT_16, 0x00, 0x80})
DEFINE_ADD_VALUE_TEST(Int32Test, add_8000,      (int32_t)     0x8000, {TYPE_INT_32, 0x00, 0x80, 0x00, 0x00})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n8001,     (int32_t)    -0x8001, {TYPE_INT_32, 0xff, 0x7f, 0xff, 0xff})
DEFINE_ADD_VALUE_TEST(Int32Test, add_7fffffff,  (int32_t) 0x7fffffff, {TYPE_INT_32, 0xff, 0xff, 0xff, 0x7f})
DEFINE_ADD_VALUE_TEST(Int32Test, add_n80000000, (int32_t)-0x80000000, {TYPE_INT_32, 0x00, 0x00, 0x00, 0x80})
