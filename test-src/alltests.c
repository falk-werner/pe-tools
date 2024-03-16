#include "alltests.h"
#include "test/test.h"
#include <windows.h>

void __main(void)
{
    struct test tests[] = {
        {"str_len empty string", &test_str_len_empty_string},
        {"str_len nonempty string", &test_str_len_nonempty_string},
        {"str_cp empty string", &test_str_cp_empty_str},
        {"str_cp nonempty string", &test_str_cp_nonempty_string},
        {"str_eq equal both empty", &test_str_eq_all_empty},
        {"str_eq equal", &test_str_eq_equal},
        {"str_eq not equal", &test_str_eq_not_equal},
        {"str_eq not equal 1st empty", &test_str_eq_not_equals_first_empty},
        {"str_eq not equal 2nd empty", &test_str_eq_not_equals_second_empty},
        {"u2str", &test_u2str},
        {"u2hexstr", &test_u2hexstr},

        {"heap_alloc and heap_free", &test_heap_alloc_free},
        {"heap_free accept null pointer", &test_heap_free_nullptr},
        {"heap_realloc", &test_heap_realloc},

        {"str_builder_new", &test_str_builder_new},
        {"str_builder_add_c", &test_str_builder_add_c},
        {"str_builder_add_c multiple times", &test_str_builder_add_c_multi},
        {"str_builder_reset", &test_str_builder_reset},

        {"args", &test_args},

        {"pe_error", &test_error},
        {"pe_error ignore if null", &test_error_null},

        {"pe_buffer_reader_read_u8", &test_buffer_reader_read_u8},
        {"pe_buffer_reader_read_u16", &test_buffer_reader_read_u16},
        {"pe_buffer_reader_read_u32", &test_buffer_reader_read_u32},
        {"pe_buffer_reader_read_u64", &test_buffer_reader_read_u64},

        {0, 0}
    };

    run_tests(tests);
}