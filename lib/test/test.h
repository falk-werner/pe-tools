#ifndef TEST_H
#define TEST_H

typedef int test_run_fn(void);

struct test {
    char const * name;
    test_run_fn * run;
};

int run_tests(struct test const * tests);

void test_set_message(char const *);

#endif
