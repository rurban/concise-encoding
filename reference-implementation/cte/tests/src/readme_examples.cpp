#include <gtest/gtest.h>
#include <cte/cte.h>
#include <stdio.h>

#define DEFAULT_DEPTH 500
#define DEFAULT_INDENT_SPACES 0
#define DEFAULT_FLOAT_DIGITS_PRECISION 15

TEST(CTE_Examples, encode)
{
    int indent_spaces = 4;
    uint8_t buffer[1000];
    std::vector<uint8_t> encode_process_backing_store(cte_encode_process_size(DEFAULT_DEPTH));
    cte_encode_process* process = (cte_encode_process*)encode_process_backing_store.data();
    ASSERT_EQ(CTE_DECODE_STATUS_OK, cte_encode_begin(process,
                                                     buffer,
                                                     sizeof(buffer),
                                                     DEFAULT_DEPTH,
                                                     DEFAULT_FLOAT_DIGITS_PRECISION,
                                                     indent_spaces));

    cte_encode_begin_map(process);
    cte_encode_add_string(process, "null");
    cte_encode_add_nil(process);
    cte_encode_add_string(process, "one");
    cte_encode_add_int_64(process, 1);
    cte_encode_add_string(process, "list");
        cte_encode_begin_list(process);
        cte_encode_add_int_64(process, 1);
        cte_encode_add_int_64(process, 2);
        cte_encode_add_int_64(process, 3);
            cte_encode_begin_map(process);
            cte_encode_add_string(process, "a");
            cte_encode_add_int_64(process, 1);
            cte_encode_add_string(process, "b");
            cte_encode_add_int_64(process, 2);
            cte_encode_add_string(process, "c");
            cte_encode_add_int_64(process, 3);
            cte_encode_end_container(process);
        cte_encode_end_container(process);
    cte_encode_add_string(process, "true");
    cte_encode_add_boolean(process, true);
    cte_encode_end_container(process);
    cte_encode_end(process);

    printf("%s\n", buffer);
}

typedef struct
{
    int depth;
} decode_context;

static decode_context* get_context(cte_decode_process* process)
{
    return (decode_context*)cte_decode_get_user_context(process);
}

static void indent(decode_context* context)
{
    for(int i = 0; i < context->depth; i++)
    {
        printf("  ");
    }
}

static void on_error(cte_decode_process* decode_process, const char* message)
{
    (void)decode_process;
    printf("Error: %s\n", message);
}

static bool on_string_begin(cte_decode_process* decode_process)
{
    (void)decode_process;
    return true;
}

static bool on_string_data(cte_decode_process* decode_process, const char* str, int64_t byte_count)
{
    // TODO
    (void)byte_count;
    indent(get_context(decode_process));
    printf("\"%s\"\n", str);
    return true;
}

static bool on_string_end(cte_decode_process* decode_process)
{
    (void)decode_process;
    return true;
}

static bool on_nil(cte_decode_process* decode_process)
{
    indent(get_context(decode_process));
    printf("nil\n");
    return true;
}
    
static bool on_boolean(cte_decode_process* decode_process, bool value)
{
    indent(get_context(decode_process));
    printf(value ? "true\n" : "false\n");
    return true;
}
    
static bool on_int_64(cte_decode_process* decode_process, int64_t value)
{
    indent(get_context(decode_process));
    if(value <= INT64_MAX)
    {
        printf("%ld\n", (int64_t)value);
    }
    else
    {
        // TODO
    }
    return true;
}
    
static bool on_float_64(cte_decode_process* decode_process, double value)
{
    indent(get_context(decode_process));
    printf("%lg\n", value);
    return true;
}

static bool on_list_begin(cte_decode_process* decode_process)
{
    indent(get_context(decode_process));
    printf("[\n");
    (get_context(decode_process))->depth++;
    return true;
}
    
static bool on_list_end(cte_decode_process* decode_process)
{
    (get_context(decode_process))->depth--;
    indent(get_context(decode_process));
    printf("]\n");
    return true;
}
    
static bool on_map_begin(cte_decode_process* decode_process)
{
    indent(get_context(decode_process));
    printf("{\n");
    (get_context(decode_process))->depth++;
    return true;
}
    
static bool on_map_end(cte_decode_process* decode_process)
{
    (get_context(decode_process))->depth--;
    indent(get_context(decode_process));
    printf("}\n");
    return true;
}

TEST(CTE_Examples, decode)
{
    cte_decode_callbacks callbacks;
    callbacks.on_nil = on_nil;
    callbacks.on_boolean = on_boolean;
    callbacks.on_int_64 = on_int_64;
    callbacks.on_float_64 = on_float_64;
    callbacks.on_string_begin = on_string_begin;
    callbacks.on_string_data = on_string_data;
    callbacks.on_string_end = on_string_end;
    callbacks.on_error = on_error;
    callbacks.on_list_begin = on_list_begin;
    callbacks.on_list_end = on_list_end;
    callbacks.on_map_begin = on_map_begin;
    callbacks.on_map_end = on_map_end;

    decode_context context = {0};
    const char* cte_string = "{\"null\":nil \"one\":1 \"list\":[1 2 3 {\"a\":1 \"b\":2 \"c\":3}] \"true\":true}";

    std::vector<uint8_t> decode_process_backing_store(cte_decode_process_size(DEFAULT_DEPTH));
    cte_decode_process* decode_process = (cte_decode_process*)decode_process_backing_store.data();
    ASSERT_EQ(CTE_DECODE_STATUS_OK, cte_decode_begin(decode_process, &callbacks, &context, DEFAULT_DEPTH));
    ASSERT_EQ(CTE_DECODE_STATUS_OK, cte_decode_feed(decode_process, cte_string, strlen(cte_string)));
    cte_decode_end(decode_process);
}
