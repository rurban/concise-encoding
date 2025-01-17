#pragma once

#include <iostream>
#include <memory>
#include <vector>

// Use the same type names as C
#include <decimal/decimal>
typedef std::decimal::decimal32::__decfloat32   dec32_ct;
typedef std::decimal::decimal64::__decfloat64   dec64_ct;
typedef std::decimal::decimal128::__decfloat128 dec128_ct;

#include <smalltime/smalltime.h>
#include <cte/cte.h>

class smalltime_wrapper
{
public:
    const smalltime _value;
    smalltime_wrapper(const smalltime value)
    : _value(value)
    {}
};

class comment_wrapper
{
public:
    const std::string _value;
    comment_wrapper(const std::string& value)
    : _value(value)
    {}
};

namespace enc
{

typedef enum
{
    ENCODE_TYPE_BOOLEAN,
    ENCODE_TYPE_INTEGER,
    ENCODE_TYPE_FLOAT,
    ENCODE_TYPE_DECIMAL,
    ENCODE_TYPE_TIME,
    ENCODE_TYPE_STRING,
    ENCODE_TYPE_BINARY,
    ENCODE_TYPE_COMMENT,
    ENCODE_TYPE_STRING_BEGIN,
    ENCODE_TYPE_BINARY_BEGIN,
    ENCODE_TYPE_COMMENT_BEGIN,
    ENCODE_TYPE_ARRAY_DATA,
    ENCODE_TYPE_STRING_END,
    ENCODE_TYPE_BINARY_END,
    ENCODE_TYPE_COMMENT_END,
    ENCODE_TYPE_LIST,
    ENCODE_TYPE_MAP,
    ENCODE_TYPE_CONTAINER_END,
    ENCODE_TYPE_EMPTY,
    ENCODE_TYPE_PADDING,
} major_type;

major_type get_major_type(const bool value);
major_type get_major_type(const int8_t value);
major_type get_major_type(const int16_t value);
major_type get_major_type(const int32_t value);
major_type get_major_type(const int64_t value);
major_type get_major_type(const int128_ct value);
major_type get_major_type(const float value);
major_type get_major_type(const double value);
major_type get_major_type(const float128_ct value);
major_type get_major_type(const dec32_ct value);
major_type get_major_type(const dec64_ct value);
major_type get_major_type(const dec128_ct value);
major_type get_major_type(const smalltime_wrapper& value);
major_type get_major_type(const std::string& value);
major_type get_major_type(const std::vector<uint8_t>& value);
major_type get_major_type(const comment_wrapper& value);

unsigned int to_doy(const int year, const unsigned int month, const unsigned int day);

std::string to_id_string(const bool value);
std::string to_id_string(const int8_t value);
std::string to_id_string(const int16_t value);
std::string to_id_string(const int32_t value);
std::string to_id_string(const int64_t value);
std::string to_id_string(const int128_ct value);
std::string to_id_string(const float value);
std::string to_id_string(const double value);
std::string to_id_string(const float128_ct value);
std::string to_id_string(const dec32_ct value);
std::string to_id_string(const dec64_ct value);
std::string to_id_string(const dec128_ct value);
std::string to_id_string(const smalltime_wrapper& value);
std::string to_id_string(const std::string& value); 
std::string to_id_string(const std::vector<uint8_t>& value);
std::string to_id_string(const std::string& name, int64_t value);
std::string to_id_string(const comment_wrapper& value);


namespace adl_helper
{
    template<class T> std::string as_string(T&& object)
    {
        return std::to_string(std::forward<T>(object));
    }
}
template<class T> std::string to_string(T&& object)
{
    return adl_helper::as_string(std::forward<T>(object));
}

class encoder;

class encoding
{
private:
    const enc::major_type _type;
    const size_t _size;
    const std::string _string_value;
    std::shared_ptr<encoding> _next;
    std::shared_ptr<encoding> _last;

public:
    encoding(const enc::major_type type, const size_t size, const std::string& string_value);
    virtual ~encoding() {}

    std::shared_ptr<encoding> next();

    std::shared_ptr<encoding> set_next(std::shared_ptr<encoding> next);

    std::shared_ptr<encoding> list();
    std::shared_ptr<encoding> map();
    std::shared_ptr<encoding> end();
    std::shared_ptr<encoding> nil();
    std::shared_ptr<encoding> smtime(smalltime value);
    std::shared_ptr<encoding> str(const std::string& value);
    std::shared_ptr<encoding> bin(cte_binary_encoding_radix radix, const std::vector<uint8_t>& value);
    std::shared_ptr<encoding> comment(const std::string& value);
    std::shared_ptr<encoding> strb();
    std::shared_ptr<encoding> binb(cte_binary_encoding_radix radix);
    std::shared_ptr<encoding> commentb();
    std::shared_ptr<encoding> data(const std::vector<uint8_t>& value);
    std::shared_ptr<encoding> stre();
    std::shared_ptr<encoding> bine();
    std::shared_ptr<encoding> commente();
    std::shared_ptr<encoding> bl(bool value);
    std::shared_ptr<encoding> i8(int8_t value);
    std::shared_ptr<encoding> i16(int16_t value);
    std::shared_ptr<encoding> i32(int32_t value);
    std::shared_ptr<encoding> i64(int64_t value);
    std::shared_ptr<encoding> i128(int128_ct value);
    std::shared_ptr<encoding> i128(int64_t high, uint64_t low);
    std::shared_ptr<encoding> f32(int precision, float value);
    std::shared_ptr<encoding> f64(int precision, double value);
    std::shared_ptr<encoding> f128(int precision, float128_ct value);
    std::shared_ptr<encoding> d32(dec32_ct value);
    std::shared_ptr<encoding> d64(dec64_ct value);
    std::shared_ptr<encoding> d128(dec128_ct value);

    enc::major_type get_type() const;

    virtual bool is_equal(const encoding& rhs) const = 0;

    bool is_equal_in_type(const encoding& rhs) const;

    bool is_equal_in_type_and_size(const encoding& rhs) const;

    bool operator ==(const encoding& rhs) const;

    const std::string as_string() const;

    virtual bool has_value(bool value) const;
    virtual bool has_value(int8_t value) const;
    virtual bool has_value(int16_t value) const;
    virtual bool has_value(int32_t value) const;
    virtual bool has_value(int64_t value) const;
    virtual bool has_value(int128_ct value) const;
    virtual bool has_value(float value) const;
    virtual bool has_value(double value) const;
    virtual bool has_value(float128_ct value) const;
    virtual bool has_value(dec32_ct value) const;
    virtual bool has_value(dec64_ct value) const;
    virtual bool has_value(dec128_ct value) const;
    virtual bool has_value(const smalltime_wrapper& value) const;
    virtual bool has_value(const std::string& value) const;
    virtual bool has_value(const std::vector<uint8_t>& value) const;
    virtual bool has_value(const comment_wrapper& value) const;

    virtual cte_encode_status encode(encoder& encoder) = 0;

    friend std::string to_string(encoding const& self) {return self.as_string();}
};

std::ostream& operator << (std::ostream& os, const encoding& rhs);


class no_value_encoding: public encoding
{
public:
    no_value_encoding(enc::major_type type, const std::string& type_string): encoding(type, 0, type_string) {}
    bool is_equal(const encoding& rhs) const { (void) rhs; return true; }
};

class list_encoding: public no_value_encoding
{
public:
    list_encoding(): no_value_encoding(enc::ENCODE_TYPE_LIST, "List") {}
    cte_encode_status encode(encoder& encoder);
};

class map_encoding: public no_value_encoding
{
public:
    map_encoding(): no_value_encoding(enc::ENCODE_TYPE_MAP, "Map") {}
    cte_encode_status encode(encoder& encoder);
};

class container_end_encoding: public no_value_encoding
{
public:
    container_end_encoding(): no_value_encoding(enc::ENCODE_TYPE_CONTAINER_END, "End") {}
    cte_encode_status encode(encoder& encoder);
};

class nil_encoding: public no_value_encoding
{
public:
    nil_encoding(): no_value_encoding(enc::ENCODE_TYPE_EMPTY, "Empty") {}
    cte_encode_status encode(encoder& encoder);
};

class time_encoding: public encoding
{
private:
    const smalltime _value;
public:
    time_encoding(smalltime value): encoding(enc::ENCODE_TYPE_TIME, sizeof(value), enc::to_id_string(smalltime_wrapper(value))), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const { return get_type() == rhs.get_type() && rhs.has_value(_value); }
    bool has_value(smalltime value) const { return _value == value; }
    smalltime value() {return _value;}
};

class boolean_encoding: public encoding
{
private:
    const bool _value;
public:
    boolean_encoding(bool value): encoding(enc::get_major_type(value), sizeof(value), enc::to_id_string(value)), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const { return get_type() == rhs.get_type() && rhs.has_value(_value); }
    bool has_value(bool value) const           { return _value == value; }
    bool value() {return _value;}
};

// FIXME: number_encoding, int128_encoding, and dfp_encoding are split because
// conversions between decimal floating point and 128-bit integers cause the
// compiler to reference __bid_floattisd and __bid_fixddti from libdfp, which
// is only present on PPC and s390 in Ubuntu :/

template<typename T> class number_encoding: public encoding
{
private:
    const T _value;
public:
    number_encoding(T value): encoding(enc::get_major_type(value), sizeof(value), enc::to_id_string(value)), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const       { return rhs.has_value(_value); }
    bool has_value(int8_t value) const             { return (int8_t)_value == value; }
    bool has_value(int16_t value) const            { return (int16_t)_value == value; }
    bool has_value(int32_t value) const            { return (int32_t)_value == value; }
    bool has_value(int64_t value) const            { return (int64_t)_value == value; }
    bool has_value(int128_ct value) const           { return (int128_ct)_value == value; }
    bool has_value(float value) const              { return (float)_value == value; }
    bool has_value(double value) const             { return (double)_value == value; }
    bool has_value(float128_ct value) const         { return (float128_ct)_value == value; }
    bool has_value(dec32_ct value) const         { return (dec32_ct)_value == value; }
    bool has_value(dec64_ct value) const         { return (dec64_ct)_value == value; }
    bool has_value(dec128_ct value) const        { return (dec128_ct)_value == value; }
    T value() {return _value;}
};

template<typename T> class float_encoding: public encoding
{
private:
    const T _value;
    const int _precision;
public:
    float_encoding(int precision, T value)
    : encoding(enc::get_major_type(value)
    , sizeof(value), enc::to_id_string(value))
    , _value(value), _precision(precision) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const       { return rhs.has_value(_value); }
    bool has_value(int8_t value) const             { return (int8_t)_value == value; }
    bool has_value(int16_t value) const            { return (int16_t)_value == value; }
    bool has_value(int32_t value) const            { return (int32_t)_value == value; }
    bool has_value(int64_t value) const            { return (int64_t)_value == value; }
    bool has_value(int128_ct value) const           { return (int128_ct)_value == value; }
    bool has_value(float value) const              { return (float)_value == value; }
    bool has_value(double value) const             { return (double)_value == value; }
    bool has_value(float128_ct value) const         { return (float128_ct)_value == value; }
    bool has_value(dec32_ct value) const         { return (dec32_ct)_value == value; }
    bool has_value(dec64_ct value) const         { return (dec64_ct)_value == value; }
    bool has_value(dec128_ct value) const        { return (dec128_ct)_value == value; }
    T value() {return _value;}
    int get_precision() {return _precision;}
};

class int128_encoding: public encoding
{
private:
    const int128_ct _value;
public:
    int128_encoding(int128_ct value): encoding(enc::get_major_type(value), sizeof(value), enc::to_id_string(value)), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const       { return rhs.has_value(_value); }
    bool has_value(int8_t value) const             { return (int8_t)_value == value; }
    bool has_value(int16_t value) const            { return (int16_t)_value == value; }
    bool has_value(int32_t value) const            { return (int32_t)_value == value; }
    bool has_value(int64_t value) const            { return (int64_t)_value == value; }
    bool has_value(int128_ct value) const           { return (int128_ct)_value == value; }
    bool has_value(float value) const              { return (float)_value == value; }
    bool has_value(double value) const             { return (double)_value == value; }
    bool has_value(float128_ct value) const         { return (float128_ct)_value == value; }
    // FIXME: Casting to float instead of decimal to avoid linker error due to
    //        missing libdfp: undefined reference to `__bid_floattisd'
    bool has_value(dec32_ct value) const         { return (float)_value == value; }
    bool has_value(dec64_ct value) const         { return (double)_value == value; }
    bool has_value(dec128_ct value) const        { return (float128_ct)_value == value; }
    int128_ct value() {return _value;}
};

template<typename T> class dfp_encoding: public encoding
{
private:
    const T _value;
public:
    dfp_encoding(T value): encoding(enc::get_major_type(value), sizeof(value), enc::to_id_string(value)), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const       { return rhs.has_value(_value); }
    bool has_value(int8_t value) const             { return (int8_t)_value == value; }
    bool has_value(int16_t value) const            { return (int16_t)_value == value; }
    bool has_value(int32_t value) const            { return (int32_t)_value == value; }
    bool has_value(int64_t value) const            { return (int64_t)_value == value; }
    // FIXME: Casting to float128_ct instead of int128_ct to avoid linker error due to
    //        missing libdfp: undefined reference to `__bid_fixddti'
    bool has_value(int128_ct value) const           { return (float128_ct)_value == value; }
    bool has_value(float value) const              { return (float)_value == value; }
    bool has_value(double value) const             { return (double)_value == value; }
    bool has_value(float128_ct value) const         { return (float128_ct)_value == value; }
    bool has_value(dec32_ct value) const         { return (dec32_ct)_value == value; }
    bool has_value(dec64_ct value) const         { return (dec64_ct)_value == value; }
    bool has_value(dec128_ct value) const        { return (dec128_ct)_value == value; }
    T value() {return _value;}
};

class string_encoding: public encoding
{
private:
    const std::string _value;
public:
    string_encoding(const std::string& value): encoding(enc::get_major_type(value), 1, enc::to_id_string(value)), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const { return get_type() == rhs.get_type() && rhs.has_value(_value); }
    bool has_value(const std::string& value) const       { return _value == value; }
    std::string value() {return _value;}
};

class binary_encoding: public encoding
{
private:
    cte_binary_encoding_radix _radix;
    const std::vector<uint8_t> _value;
public:
    binary_encoding(cte_binary_encoding_radix radix, const std::vector<uint8_t>& value)
    : encoding(enc::get_major_type(value), 1, enc::to_id_string(value))
    , _radix(radix)
    , _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const    { return get_type() == rhs.get_type() && rhs.has_value(_value); }
    bool has_value(const std::vector<uint8_t>& value) const { return _value == value; }
    std::vector<uint8_t> value() {return _value;}
    cte_binary_encoding_radix get_radix() {return _radix;}
};

class comment_encoding: public encoding
{
private:
    const std::string _value;
public:
    comment_encoding(const std::string& value): encoding(ENCODE_TYPE_COMMENT, 1, enc::to_id_string(comment_wrapper(value))), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const { return get_type() == rhs.get_type() && rhs.has_value(_value); }
    bool has_value(const std::string& value) const       { return _value == value; }
    std::string value() {return _value;}
};

class string_begin_encoding: public no_value_encoding
{
public:
    string_begin_encoding(): no_value_encoding(enc::ENCODE_TYPE_STRING_BEGIN, "strb") {}
    cte_encode_status encode(encoder& encoder);
};

class binary_begin_encoding: public no_value_encoding
{
private:
    cte_binary_encoding_radix _radix;
public:
    binary_begin_encoding(cte_binary_encoding_radix radix)
    : no_value_encoding(enc::ENCODE_TYPE_BINARY_BEGIN, "binb")
    , _radix(radix) {}
    cte_encode_status encode(encoder& encoder);
    cte_binary_encoding_radix get_radix() {return _radix;}
};

class comment_begin_encoding: public no_value_encoding
{
public:
    comment_begin_encoding(): no_value_encoding(enc::ENCODE_TYPE_COMMENT_BEGIN, "commentb") {}
    cte_encode_status encode(encoder& encoder);
};

class data_encoding: public encoding
{
private:
    const std::vector<uint8_t> _value;
public:
    data_encoding(const std::vector<uint8_t>& value): encoding(enc::ENCODE_TYPE_ARRAY_DATA, 1, enc::to_id_string("data", value.size())), _value(value) {}
    cte_encode_status encode(encoder& encoder);
    bool is_equal(const encoding& rhs) const    { return get_type() == rhs.get_type() && rhs.has_value(_value); }
    bool has_value(const std::vector<uint8_t>& value) const { return _value == value; }
    std::vector<uint8_t> value() {return _value;}
};

class string_end_encoding: public no_value_encoding
{
public:
    string_end_encoding(): no_value_encoding(enc::ENCODE_TYPE_STRING_END, "stre") {}
    cte_encode_status encode(encoder& encoder);
};

class binary_end_encoding: public no_value_encoding
{
public:
    binary_end_encoding(): no_value_encoding(enc::ENCODE_TYPE_BINARY_END, "bine") {}
    cte_encode_status encode(encoder& encoder);
};

class comment_end_encoding: public no_value_encoding
{
public:
    comment_end_encoding(): no_value_encoding(enc::ENCODE_TYPE_COMMENT_END, "commente") {}
    cte_encode_status encode(encoder& encoder);
};

class encoder
{
public:
    virtual ~encoder() {}
    virtual cte_encode_status encode(list_encoding& encoding) = 0;
    virtual cte_encode_status encode(map_encoding& encoding) = 0;
    virtual cte_encode_status encode(container_end_encoding& encoding) = 0;
    virtual cte_encode_status encode(nil_encoding& encoding) = 0;
    virtual cte_encode_status encode(time_encoding& encoding) = 0;
    virtual cte_encode_status encode(string_encoding& encoding) = 0;
    virtual cte_encode_status encode(binary_encoding& encoding) = 0;
    virtual cte_encode_status encode(comment_encoding& encoding) = 0;
    virtual cte_encode_status encode(string_begin_encoding& encoding) = 0;
    virtual cte_encode_status encode(binary_begin_encoding& encoding) = 0;
    virtual cte_encode_status encode(comment_begin_encoding& encoding) = 0;
    virtual cte_encode_status encode(data_encoding& encoding) = 0;
    virtual cte_encode_status encode(string_end_encoding& encoding) = 0;
    virtual cte_encode_status encode(binary_end_encoding& encoding) = 0;
    virtual cte_encode_status encode(comment_end_encoding& encoding) = 0;
    virtual cte_encode_status encode(boolean_encoding& encoding) = 0;
    virtual cte_encode_status encode(number_encoding<int8_t>& encoding) = 0;
    virtual cte_encode_status encode(number_encoding<int16_t>& encoding) = 0;
    virtual cte_encode_status encode(number_encoding<int32_t>& encoding) = 0;
    virtual cte_encode_status encode(number_encoding<int64_t>& encoding) = 0;
    virtual cte_encode_status encode(int128_encoding& encoding) = 0;
    virtual cte_encode_status encode(float_encoding<float>& encoding) = 0;
    virtual cte_encode_status encode(float_encoding<double>& encoding) = 0;
    virtual cte_encode_status encode(float_encoding<float128_ct>& encoding) = 0;
    virtual cte_encode_status encode(dfp_encoding<dec32_ct>& encoding) = 0;
    virtual cte_encode_status encode(dfp_encoding<dec64_ct>& encoding) = 0;
    virtual cte_encode_status encode(dfp_encoding<dec128_ct>& encoding) = 0;
};


std::shared_ptr<list_encoding>               list();
std::shared_ptr<map_encoding>                map();
std::shared_ptr<container_end_encoding>      end();
std::shared_ptr<nil_encoding>                nil();
std::shared_ptr<time_encoding>               smtime(smalltime value);
std::shared_ptr<time_encoding>               smtime(int year, int month, int day, int hour, int minute, int second, int usec);
std::shared_ptr<string_encoding>             str(const std::string& value);
std::shared_ptr<binary_encoding>             bin(cte_binary_encoding_radix radix, const std::vector<uint8_t>& value);
std::shared_ptr<comment_encoding>            comment(const std::string& value);
std::shared_ptr<string_begin_encoding>       strb();
std::shared_ptr<binary_begin_encoding>       binb(cte_binary_encoding_radix radix);
std::shared_ptr<comment_begin_encoding>      commentb();
std::shared_ptr<data_encoding>               data(const std::vector<uint8_t>& value);
std::shared_ptr<string_end_encoding>         stre();
std::shared_ptr<binary_end_encoding>         bine();
std::shared_ptr<comment_end_encoding>        commente();
std::shared_ptr<boolean_encoding>            bl(bool value);
std::shared_ptr<number_encoding<int8_t>>     i8(int8_t value);
std::shared_ptr<number_encoding<int16_t>>    i16(int16_t value);
std::shared_ptr<number_encoding<int32_t>>    i32(int32_t value);
std::shared_ptr<number_encoding<int64_t>>    i64(int64_t value);
std::shared_ptr<int128_encoding>             i128(int128_ct value);
std::shared_ptr<int128_encoding>             i128(int64_t high, uint64_t low);
std::shared_ptr<float_encoding<float>>       f32(int precision, float value);
std::shared_ptr<float_encoding<double>>      f64(int precision, double value);
std::shared_ptr<float_encoding<float128_ct>>  f128(int precision, float128_ct value);
std::shared_ptr<dfp_encoding<dec32_ct>>    d32(dec32_ct value);
std::shared_ptr<dfp_encoding<dec64_ct>>    d64(dec64_ct value);
std::shared_ptr<dfp_encoding<dec128_ct>>   d128(dec128_ct value);

} // namespace enc
