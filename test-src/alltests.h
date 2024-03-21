#ifndef ALLTESTS_H
#define ALLTESTS_H

#include "test/test.h"

int test_str_len_empty_string(void);
int test_str_len_nonempty_string(void);
int test_str_cp_empty_str(void);
int test_str_cp_nonempty_string(void);
int test_str_eq_all_empty(void);
int test_str_eq_equal(void);
int test_str_eq_not_equal(void);
int test_str_eq_not_equals_first_empty(void);
int test_str_eq_not_equals_second_empty(void);
int test_u2str(void);
int test_u2hexstr(void);

int test_heap_alloc_free(void);
int test_heap_free_nullptr(void);
int test_heap_realloc(void);

int test_str_builder_new(void);
int test_str_builder_add_c(void);
int test_str_builder_add_c_multi(void);
int test_str_builder_reset(void);

int test_args(void);

int test_cli_short_flag(void);
int test_cli_long_flag(void);
int test_cli_short_opt(void);
int test_cli_long_opt(void);
int test_cli_extra_arg(void);
int test_cli_empty_opts(void);
int test_cli_error_missing_arg(void);
int test_cli_error_unknown_opt(void);
int test_cli_complex(void);

int test_error(void);
int test_error_null(void);

int test_buffer_reader_read_u8(void);
int test_buffer_reader_read_u16(void);
int test_buffer_reader_read_u32(void);
int test_buffer_reader_read_u64(void);

#endif
