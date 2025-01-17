#pragma once

#include <functional>
#include <vector>
#include <cbe/cbe.h>
#include "encoding.h"

typedef enum
{
    CBE_DECODING_OTHER,
    CBE_DECODING_STRING,
    CBE_DECODING_BYTES,
    CBE_DECODING_URI,
    CBE_DECODING_COMMENT,
} cbe_decoding_type;

class cbe_decoder
{
private:
    std::vector<char> _process_backing_store;
    cbe_decode_process* _process;
    std::vector<uint8_t> _received_data;
    int64_t _read_offset = 0;
    bool _process_is_valid = true;
    std::shared_ptr<enc::encoding> _decoded;
    cbe_decoding_type _currently_decoding_type = CBE_DECODING_OTHER;
    int64_t _currently_decoding_length;
    int64_t _currently_decoding_offset;
    std::vector<uint8_t> _currently_decoding_data;
    bool _callback_return_value;
    int _max_container_depth;

public:
    // Internal functions
    bool set_next(std::shared_ptr<enc::encoding> encoding);
    bool string_begin(int64_t byte_count);
    bool bytes_begin(int64_t byte_count);
    bool uri_begin(int64_t byte_count);
    bool comment_begin(int64_t byte_count);
    bool add_string_data(const std::string& data);
    bool add_bytes_data(const std::vector<uint8_t>& data);
    bool add_uri_data(const std::string& data);
    bool add_comment_data(const std::string& data);

public:
    cbe_decoder(int max_container_depth, bool callback_return_value);

    // Begin the decoding process.
    cbe_decode_status begin();

    // Feed data to be decoded.
    cbe_decode_status feed(std::vector<uint8_t>& data);

    // End the decoding process.
    cbe_decode_status end();

    // Decode an entire document
    cbe_decode_status decode(std::vector<uint8_t>& document);

    // Get the complete raw data received.
    std::vector<uint8_t>& received_data();

    // Get the decoded encoding objects.
    std::shared_ptr<enc::encoding> decoded();

    bool get_callback_return_value();
};
