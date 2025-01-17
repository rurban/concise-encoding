#include "encoding.h"

#include <quadmath.h> // Need -lquadmath at the end of the line
// stdlib.h:
// _Float128 strtof128 (const char *__restrict __nptr, char **__restrict __endptr)
// extern int strfromf128 (char *__dest, size_t __size, const char * __format, _Float128 __f)
#include <string.h>


// https://github.com/libdfp/libdfp

// Literal Suffixes:
//  * decimal32: 1.1df
//  * decimal64: 1.1dd
//  * decimal128: 1.1dl
//  * binary32: 1.1f
//  * binary64: 1.1
//  * binary128: 1.1l

// #define KSLogger_LocalLevel DEBUG
#include "kslogger.h"


#define IDENTIFIER_MAX_LENGTH 20


namespace enc
{

major_type get_major_type(const bool value)
{
    (void) value; return ENCODE_TYPE_BOOLEAN;
}

major_type get_major_type(const int8_t value)
{
    (void) value; return ENCODE_TYPE_INTEGER;
}

major_type get_major_type(const int16_t value)
{
    (void) value; return ENCODE_TYPE_INTEGER;
}

major_type get_major_type(const int32_t value)
{
    (void) value; return ENCODE_TYPE_INTEGER;
}

major_type get_major_type(const int64_t value)
{
    (void) value; return ENCODE_TYPE_INTEGER;
}

major_type get_major_type(const int128_ct value)
{
    (void) value; return ENCODE_TYPE_INTEGER;
}

major_type get_major_type(const float value)
{
    (void) value; return ENCODE_TYPE_FLOAT;
}

major_type get_major_type(const double value)
{
    (void) value; return ENCODE_TYPE_FLOAT;
}

major_type get_major_type(const float128_ct value)
{
    (void) value; return ENCODE_TYPE_FLOAT;
}

major_type get_major_type(const dec32_ct value)
{
    (void) value; return ENCODE_TYPE_DECIMAL;
}

major_type get_major_type(const dec64_ct value)
{
    (void) value; return ENCODE_TYPE_DECIMAL;
}

major_type get_major_type(const dec128_ct value)
{
    (void) value; return ENCODE_TYPE_DECIMAL;
}

major_type get_major_type(const smalltime_wrapper& value)
{
    (void) value; return ENCODE_TYPE_TIME;
}

major_type get_major_type(const std::string& value)
{
    (void) value; return ENCODE_TYPE_STRING;
}

major_type get_major_type(const std::vector<uint8_t>& value)
{
    (void) value; return ENCODE_TYPE_BINARY;
}

major_type get_major_type(const comment_wrapper& value)
{
    (void) value; return ENCODE_TYPE_COMMENT;
}


static int g_days_to_the_month[] =
{
    0, // Nothing
    g_days_to_the_month[ 0] +  0, // January
    g_days_to_the_month[ 1] + 31, // February
    g_days_to_the_month[ 2] + 28, // March
    g_days_to_the_month[ 3] + 31, // April
    g_days_to_the_month[ 4] + 30, // May
    g_days_to_the_month[ 5] + 31, // June
    g_days_to_the_month[ 6] + 30, // July
    g_days_to_the_month[ 7] + 31, // August
    g_days_to_the_month[ 8] + 31, // September
    g_days_to_the_month[ 9] + 30, // October
    g_days_to_the_month[10] + 31, // November
    g_days_to_the_month[11] + 30, // December
};

static inline int is_leap_year(int year)
{
    return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}

unsigned int ymd_to_doy(int year, unsigned int month, unsigned int day)
{
    // Extremely naive converter, only good for modern dates.
    unsigned int days = g_days_to_the_month[month] + day;
    if(is_leap_year(year) && month > 2)
    {
        days++;
    }
    return days;
}

static std::string to_string(const int128_ct value)
{
    char buffer[50];
    char* ptr = buffer + sizeof(buffer) - 1;
    *ptr = 0;
    if(value == 0)
    {
        *--ptr = '0';
        return ptr;
    }

    // TODO: Does not handle -max
    __uint128_t uvalue = value > 0 ? value : -value;

    while(uvalue > 0)
    {
        *--ptr = (uvalue % 10) + '0';
        uvalue /= 10;
    }
    if(value < 0)
    {
        *--ptr = '-';
    }
    return ptr;
}

static std::string to_string(const float128_ct value)
{
    char buffer[50];
    int precision = 20;
    quadmath_snprintf(buffer, sizeof(buffer)-1, "%+-#46.*Qe", precision, value);
    buffer[sizeof(buffer)-1] = 0;
    return buffer;
}

static std::string to_string(const std::vector<uint8_t>& value, int max_length)
{
    static char hex_table[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    };

    std::string str;
    int length = value.size();
    if(max_length < length)
    {
        length = max_length;
    }

    for(int i = 0; i < length; i++)
    {
        char buffer[3] = {' ', ' ', ' '};
        uint8_t byte = value[i];
        buffer[0] = hex_table[byte >> 4];
        buffer[1] = hex_table[byte & 15];
        str.append(buffer, 3);
    }
    if(str.size() > 0)
    {
        if(value.size() > (unsigned)max_length)
        {
            str.append("...");
        }
        else
        {
            str.erase(str.size() - 1, 1);
        }
    }
    return str;
}

static std::string to_string(const std::string& value, int max_length)
{
    if(value.size() <= (unsigned)max_length)
    {
        return value;
    }
    return value.substr(0, max_length);
}

// static std::string to_string(const comment_wrapper& value, int max_length)
// {
//     if(value._value.size() <= (unsigned)max_length)
//     {
//         return value._value;
//     }
//     return value._value.substr(0, max_length);
// }

static std::string to_string(const smalltime_wrapper& value)
{
    char buff[30];
    snprintf(buff, sizeof(buff),
        "%04d-%03d:%02d-%02d-%02d",
        smalltime_get_year(value._value),
        smalltime_get_day(value._value),
        smalltime_get_hour(value._value),
        smalltime_get_minute(value._value),
        smalltime_get_second(value._value)
        );
    int usec = smalltime_get_microsecond(value._value);
    int length = strlen(buff);
    if(usec != 0)
    {
        snprintf(buff + length, sizeof(buff) - length, ".%d", usec);
        length = strlen(buff);
    }

    return buff;
}

// TODO: These are only approximations
static std::string to_string(const dec32_ct value)
{
    return "~" + std::to_string((const double)value);
}
static std::string to_string(const dec64_ct value)
{
    return "~" + std::to_string((const double)value);
}
static std::string to_string(const dec128_ct value)
{
    return "~" + to_string((const float128_ct)value);
}

std::string to_id_string(const bool value)        {return std::string("bl(")   + std::to_string(value) + ")";}
std::string to_id_string(const int8_t value)      {return std::string("i8(")   + std::to_string(value) + ")";}
std::string to_id_string(const int16_t value)     {return std::string("i16(")  + std::to_string(value) + ")";}
std::string to_id_string(const int32_t value)     {return std::string("i32(")  + std::to_string(value) + ")";}
std::string to_id_string(const int64_t value)     {return std::string("i64(")  + std::to_string(value) + ")";}
std::string to_id_string(const int128_ct value)    {return std::string("i128(") + to_string(value)      + ")";}
std::string to_id_string(const float value)       {return std::string("f32(")  + std::to_string(value) + ")";}
std::string to_id_string(const double value)      {return std::string("f64(")  + std::to_string(value) + ")";}
std::string to_id_string(const float128_ct value)  {return std::string("f128(") + to_string(value)      + ")";}
std::string to_id_string(const dec32_ct value)  {return std::string("d32(")  + to_string(value)      + ")";}
std::string to_id_string(const dec64_ct value)  {return std::string("d64(")  + to_string(value)      + ")";}
std::string to_id_string(const dec128_ct value) {return std::string("d128(") + to_string(value)      + ")";}
std::string to_id_string(const std::string& value) {return std::string("str(") + to_string(value, IDENTIFIER_MAX_LENGTH) + ")";}
std::string to_id_string(const std::vector<uint8_t>& value) {return std::string("bin(") + to_string(value, IDENTIFIER_MAX_LENGTH) + ")";}
std::string to_id_string(const smalltime_wrapper& value) {return std::string("time(") + to_string(value)  + ")";}
std::string to_id_string(const std::string& name, int64_t value) {return name + "(" + std::to_string(value) + ")";}
std::string to_id_string(const comment_wrapper& value) {return std::string("com(") + to_string(value._value, IDENTIFIER_MAX_LENGTH) + ")";}


encoding::encoding(const enc::major_type type, const size_t size, const std::string& string_value)
: _type(type)
, _size(size)
, _string_value(string_value)
{
    KSLOG_DEBUG("Create type %d: %s", _type, _string_value.c_str());
}

std::shared_ptr<encoding> encoding::next()
{
    return _next;
}

std::shared_ptr<encoding> encoding::set_next(std::shared_ptr<encoding> next)
{
    if(_last)
    {
        _last->_next = next;
    }
    else
    {
        _next = next;
    }
    _last = next;

    // FIXME:
    //
    // Ugly hack to keep things in shared_ptr land because there's no way
    // to get ahold of an owning smart pointer from within an object,
    // since it's not baked into the language.
    //
    // The fallout is a broken contract about this shared_ptr's lifetime
    // and validity.
    //
    // This is workable from within the confines of the testing framework
    // since the lifetime of an encoding object will never actually exceed
    // its creation context.
    //
    // Have I ever mentioned how much I hate C++?
    return std::shared_ptr<encoding>(this, [](encoding*){});
}

const std::string encoding::as_string() const
{
    std::string str = _string_value;
    if(_next)
    {
        str.append("->");
        str.append(_next->as_string());
    }

    return str;
}

enc::major_type encoding::get_type() const
{
    return _type;
}

bool encoding::is_equal_in_type(const encoding& rhs) const
{        
    return _type == rhs._type && is_equal(rhs);
}

bool encoding::is_equal_in_type_and_size(const encoding& rhs) const
{
    return _size == rhs._size && is_equal_in_type(rhs);
}

bool encoding::operator ==(const encoding& rhs) const
{
    return is_equal(rhs);
}

bool encoding::has_value(bool value) const
{
    (void) value; return false;
}

bool encoding::has_value(int8_t value) const
{
    (void) value; return false;
}

bool encoding::has_value(int16_t value) const
{
    (void) value; return false;
}

bool encoding::has_value(int32_t value) const
{
    (void) value; return false;
}

bool encoding::has_value(int64_t value) const
{
    (void) value; return false;
}

bool encoding::has_value(int128_ct value) const
{
    (void) value; return false;
}

bool encoding::has_value(float value) const
{
    (void) value; return false;
}

bool encoding::has_value(double value) const
{
    (void) value; return false;
}

bool encoding::has_value(float128_ct value) const
{
    (void) value; return false;
}

bool encoding::has_value(dec32_ct value) const
{
    (void) value; return false;
}

bool encoding::has_value(dec64_ct value) const
{
    (void) value; return false;
}

bool encoding::has_value(dec128_ct value) const
{
    (void) value; return false;
}

bool encoding::has_value(const smalltime_wrapper& value) const
{
    (void) value; return false;
}

bool encoding::has_value(const std::string& value) const
{
    (void) value; return false;
}

bool encoding::has_value(const std::vector<uint8_t>& value) const
{
    (void) value; return false;
}

bool encoding::has_value(const comment_wrapper& value) const
{
    (void) value; return false;
}




std::ostream& operator << (std::ostream& os, const encoding& rhs)
{
    os << to_string(rhs);
    return os;
}

cte_encode_status list_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status map_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status container_end_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status nil_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status time_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status string_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status comment_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status binary_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status string_begin_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status binary_begin_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status comment_begin_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status data_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status string_end_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status binary_end_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status comment_end_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status boolean_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status number_encoding<int8_t>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status number_encoding<int16_t>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status number_encoding<int32_t>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status number_encoding<int64_t>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

cte_encode_status int128_encoding::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status float_encoding<float>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status float_encoding<double>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status float_encoding<float128_ct>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status dfp_encoding<dec32_ct>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status dfp_encoding<dec64_ct>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}

template <> cte_encode_status dfp_encoding<dec128_ct>::encode(encoder& encoder)
{
    return encoder.encode(*this);
}



std::shared_ptr<encoding> encoding::list()                           {return this->set_next(enc::list());}
std::shared_ptr<encoding> encoding::map()                            {return this->set_next(enc::map());}
std::shared_ptr<encoding> encoding::end()                            {return this->set_next(enc::end());}
std::shared_ptr<encoding> encoding::nil()                            {return this->set_next(enc::nil());}
std::shared_ptr<encoding> encoding::smtime(smalltime value)          {return this->set_next(enc::smtime(value));}
std::shared_ptr<encoding> encoding::str(const std::string& value)    {return this->set_next(enc::str(value));}
std::shared_ptr<encoding> encoding::bin(cte_binary_encoding_radix radix, const std::vector<uint8_t>& value) {return this->set_next(enc::bin(radix, value));}
std::shared_ptr<encoding> encoding::comment(const std::string& value){return this->set_next(enc::comment(value));}
std::shared_ptr<encoding> encoding::strb()                           {return this->set_next(enc::strb());}
std::shared_ptr<encoding> encoding::binb(cte_binary_encoding_radix radix) {return this->set_next(enc::binb(radix));}
std::shared_ptr<encoding> encoding::commentb()                       {return this->set_next(enc::commentb());}
std::shared_ptr<encoding> encoding::data(const std::vector<uint8_t>& value) {return this->set_next(enc::data(value));}
std::shared_ptr<encoding> encoding::stre()                           {return this->set_next(enc::stre());}
std::shared_ptr<encoding> encoding::bine()                           {return this->set_next(enc::bine());}
std::shared_ptr<encoding> encoding::commente()                       {return this->set_next(enc::commente());}
std::shared_ptr<encoding> encoding::bl(bool value)                   {return this->set_next(enc::bl(value));}
std::shared_ptr<encoding> encoding::i8(int8_t value)                 {return this->set_next(enc::i8(value));}
std::shared_ptr<encoding> encoding::i16(int16_t value)               {return this->set_next(enc::i16(value));}
std::shared_ptr<encoding> encoding::i32(int32_t value)               {return this->set_next(enc::i32(value));}
std::shared_ptr<encoding> encoding::i64(int64_t value)               {return this->set_next(enc::i64(value));}
std::shared_ptr<encoding> encoding::i128(int128_ct value)             {return this->set_next(enc::i128(value));}
std::shared_ptr<encoding> encoding::i128(int64_t high, uint64_t low) {return this->set_next(enc::i128(high, low));}
std::shared_ptr<encoding> encoding::f32(int precision, float value)       {return this->set_next(enc::f32(precision, value));}
std::shared_ptr<encoding> encoding::f64(int precision, double value)      {return this->set_next(enc::f64(precision, value));}
std::shared_ptr<encoding> encoding::f128(int precision, float128_ct value) {return this->set_next(enc::f128(precision, value));}
std::shared_ptr<encoding> encoding::d32(dec32_ct value)            {return this->set_next(enc::d32(value));}
std::shared_ptr<encoding> encoding::d64(dec64_ct value)            {return this->set_next(enc::d64(value));}
std::shared_ptr<encoding> encoding::d128(dec128_ct value)          {return this->set_next(enc::d128(value));}

std::shared_ptr<list_encoding>                 list()                           {return std::make_shared<list_encoding>();}
std::shared_ptr<map_encoding>                  map()                            {return std::make_shared<map_encoding>();}
std::shared_ptr<container_end_encoding>        end()                            {return std::make_shared<container_end_encoding>();}
std::shared_ptr<nil_encoding>                  nil()                            {return std::make_shared<nil_encoding>();}
std::shared_ptr<time_encoding>                 smtime(smalltime value)          {return std::make_shared<time_encoding>(value);}
std::shared_ptr<time_encoding>                 smtime(int year, int month, int day, int hour, int minute, int second, int usec) {return smtime(smalltime_new(year, enc::ymd_to_doy(year, month, day), hour, minute, second, usec));}
std::shared_ptr<string_encoding>               str(const std::string& value)    {return std::make_shared<string_encoding>(value);}
std::shared_ptr<binary_encoding>               bin(cte_binary_encoding_radix radix, const std::vector<uint8_t>& value) {return std::make_shared<binary_encoding>(radix, value);}
std::shared_ptr<comment_encoding>              comment(const std::string& value){return std::make_shared<comment_encoding>(value);}
std::shared_ptr<string_begin_encoding>         strb()                           {return std::make_shared<string_begin_encoding>();}
std::shared_ptr<binary_begin_encoding>         binb(cte_binary_encoding_radix radix) {return std::make_shared<binary_begin_encoding>(radix);}
std::shared_ptr<comment_begin_encoding>        commentb()                       {return std::make_shared<comment_begin_encoding>();}
std::shared_ptr<data_encoding>                 data(const std::vector<uint8_t>& value) {return std::make_shared<data_encoding>(value);}
std::shared_ptr<string_end_encoding>           stre()                           {return std::make_shared<string_end_encoding>();}
std::shared_ptr<binary_end_encoding>           bine()                           {return std::make_shared<binary_end_encoding>();}
std::shared_ptr<comment_end_encoding>          commente()                       {return std::make_shared<comment_end_encoding>();}
std::shared_ptr<boolean_encoding>              bl(bool value)                   {return std::make_shared<boolean_encoding>(value);}
std::shared_ptr<number_encoding<int8_t>>       i8(int8_t value)                 {return std::make_shared<number_encoding<int8_t>>(value);}
std::shared_ptr<number_encoding<int16_t>>      i16(int16_t value)               {return std::make_shared<number_encoding<int16_t>>(value);}
std::shared_ptr<number_encoding<int32_t>>      i32(int32_t value)               {return std::make_shared<number_encoding<int32_t>>(value);}
std::shared_ptr<number_encoding<int64_t>>      i64(int64_t value)               {return std::make_shared<number_encoding<int64_t>>(value);}
std::shared_ptr<int128_encoding>               i128(int128_ct value)             {return std::make_shared<int128_encoding>(value);}
std::shared_ptr<int128_encoding>               i128(int64_t high, uint64_t low) {return i128((((int128_ct)high) << 64) + low);}
std::shared_ptr<float_encoding<float>>         f32(int precision, float value)       {return std::make_shared<float_encoding<float>>(precision, value);}
std::shared_ptr<float_encoding<double>>        f64(int precision, double value)      {return std::make_shared<float_encoding<double>>(precision, value);}
std::shared_ptr<float_encoding<float128_ct>>    f128(int precision, float128_ct value) {return std::make_shared<float_encoding<float128_ct>>(precision, value);}
std::shared_ptr<dfp_encoding<dec32_ct>>      d32(dec32_ct value)            {return std::make_shared<dfp_encoding<dec32_ct>>(value);}
std::shared_ptr<dfp_encoding<dec64_ct>>      d64(dec64_ct value)            {return std::make_shared<dfp_encoding<dec64_ct>>(value);}
std::shared_ptr<dfp_encoding<dec128_ct>>     d128(dec128_ct value)          {return std::make_shared<dfp_encoding<dec128_ct>>(value);}

} // namespace enc
