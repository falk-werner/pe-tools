#include "test/test.h"
#include "util/print.h"
#include "util/heap.h"

#include <windows.h>

static char const * g_test_message = 0;

static char* int_to_str(int value)
{
    static char buffer[10];
    buffer[9] = '\0';
    int i = 9;

    do {
        i--;
        buffer[i] = '0' + (value % 10);
        value /= 10;
    }
    while (value > 0);

    return &(buffer[i]);
}

int run_tests(struct test const * tests)
{
    int exit_code = 0;
    int count = 0;
    for(; tests[count].name != 0; count++);

    print("TAP version 14\n");
    print("1..");
    print(int_to_str(count));
    print("\n");

    for(int i = 0; tests[i].name != 0; i++) {
        struct test const * test = &(tests[i]);
        g_test_message = 0;
        int const heap_size = heap_used_size();

        int result = test->run();
        if ((result == 0) && (heap_size != heap_used_size())) {
            test_set_message("memory leak detected");
            result = 1;
        }

        if (result != 0) {            
            exit_code = 1;
            print("not ");
        }
        
        print("ok ");
        print(int_to_str(i+1));
        print(" - ");
        print(test->name);
        print("\n");

        if (g_test_message != 0) {
            print("  ---\n  message: \"");
            print(g_test_message);
            print("\"\n  ...\n");
        }
    }

    return exit_code;
}

void test_set_message(char const * message)
{
    g_test_message = message;
}
