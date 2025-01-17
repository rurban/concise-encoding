#pragma once

#include <functional>
#include <cte/cte.h>
#include "encoding.h"

class cte_encoder: public enc::encoder
{
private:
    std::vector<char> _process_backing_store;
    cte_encode_process* _process;
    std::vector<uint8_t> _buffer;
    std::vector<uint8_t> _encoded_data;
    int _max_container_depth;
    int _indent_spaces;
    std::function<bool(uint8_t* data_start, int64_t length)> _on_data_ready;

protected:
    bool flush_buffer();
    cte_encode_status flush_and_retry(std::function<cte_encode_status()> my_function);

public:
    // Internal functions
    cte_encode_status encode(enc::number_encoding<int8_t>& e);
    cte_encode_status encode(enc::number_encoding<int16_t>& e);
    cte_encode_status encode(enc::number_encoding<int32_t>& e);
    cte_encode_status encode(enc::number_encoding<int64_t>& e);
    cte_encode_status encode(enc::int128_encoding& e);
    cte_encode_status encode(enc::float_encoding<float>& e);
    cte_encode_status encode(enc::float_encoding<double>& e);
    cte_encode_status encode(enc::float_encoding<float128_ct>& e);
    cte_encode_status encode(enc::dfp_encoding<dec32_ct>& e);
    cte_encode_status encode(enc::dfp_encoding<dec64_ct>& e);
    cte_encode_status encode(enc::dfp_encoding<dec128_ct>& e);
    cte_encode_status encode(enc::boolean_encoding& e);
    cte_encode_status encode(enc::time_encoding& e);
    cte_encode_status encode(enc::nil_encoding& e);
    cte_encode_status encode(enc::list_encoding& e);
    cte_encode_status encode(enc::map_encoding& e);
    cte_encode_status stream_array(const std::vector<uint8_t>& data);
    cte_encode_status encode(enc::string_encoding& e);
    cte_encode_status encode(enc::binary_encoding& e);
    cte_encode_status encode(enc::comment_encoding& e);
    cte_encode_status encode(enc::string_begin_encoding& e);
    cte_encode_status encode(enc::binary_begin_encoding& e);
    cte_encode_status encode(enc::comment_begin_encoding& e);
    cte_encode_status encode(enc::data_encoding& e);
    cte_encode_status encode(enc::string_end_encoding& e);
    cte_encode_status encode(enc::binary_end_encoding& e);
    cte_encode_status encode(enc::comment_end_encoding& e);
    cte_encode_status encode(enc::container_end_encoding& e);

public:
    cte_encoder(int64_t buffer_size,
                int max_container_depth,
                int indent_spaces,
                std::function<bool(uint8_t* data_start, int64_t length)> on_data_ready =
                    [](uint8_t* data_start, int64_t length)
                    {(void)data_start; (void)length; return true;});
    virtual ~cte_encoder() {}

    // Encode an encoding object and all linked objects.
    cte_encode_status encode(std::shared_ptr<enc::encoding> enc);

    // Encode entire array objects. If the entire object won't fit,
    // returns with failure.
    cte_encode_status encode_string(std::vector<uint8_t> value);
    cte_encode_status encode_binary(cte_binary_encoding_radix radix, std::vector<uint8_t> value);
    cte_encode_status encode_comment(std::vector<uint8_t> value);

    int64_t get_encode_buffer_offset();

    // Get the complete raw encoded data.
    std::vector<uint8_t>& encoded_data() {return _encoded_data;}
};
