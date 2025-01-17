#include "test_utils.h"
#include <sstream>

std::string as_string(const std::vector<uint8_t>& value)
{
    static char nybbles[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string str;
    for(uint8_t ch: value)
    {
        str.append(1, nybbles[ch>>4]);
        str.append(1, nybbles[ch&15]);
        str.append(1, ' ');
    }
    if(str.size() == 0)
    {
        return str;
    }
    return str.substr(0, str.size()-1);
}

std::vector<uint8_t> as_vector(const std::string& value)
{
    return std::vector<uint8_t>(value.begin(), value.end());
}

std::vector<uint8_t> array_length_field(int64_t length)
{
    std::vector<uint8_t> bytes;
    do
    {
        uint8_t byte = (uint8_t)(length & 0x7f);
        length >>= 7;
        if(length > 0)
        {
            byte |= 0x80;
        }
        bytes.push_back(byte);
    } while(length > 0);
    return bytes;
}

std::string make_string(int length)
{
    std::stringstream stream;
    static const char characters[] =
    {
        '0','1','2','3','4','5','6','7','8','9'
    };
    static const int character_count = sizeof(characters) / sizeof(*characters);
    for(int i = 0; i < length; i++)
    {
        stream << characters[i % character_count];
    }
    return stream.str();
}

std::vector<uint8_t> make_7f_bytes(int length)
{
    std::vector<uint8_t> vec;
    for(int i = 0; i < length; i++)
    {
        vec.push_back((uint8_t)(i & 0x7f));
    }
    return vec;
}

std::vector<uint8_t> make_bytes(int length, int start_value)
{
    std::vector<uint8_t> vec;
    int value = start_value;
    for(int i = 0; i < length; i++, value++)
    {
        vec.push_back((uint8_t)(value & 0x7f));
    }
    return vec;
}

std::vector<uint8_t> concat(
    const std::vector<uint8_t>& v1,
    const std::vector<uint8_t>& v2,
    const std::vector<uint8_t>& v3,
    const std::vector<uint8_t>& v4,
    const std::vector<uint8_t>& v5
    )
{
    std::vector<uint8_t> accumulator(v1);
    accumulator.insert(accumulator.end(), v2.begin(), v2.end());
    accumulator.insert(accumulator.end(), v3.begin(), v3.end());
    accumulator.insert(accumulator.end(), v4.begin(), v4.end());
    accumulator.insert(accumulator.end(), v5.begin(), v5.end());
    return accumulator;
}