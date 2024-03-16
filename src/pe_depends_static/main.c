#include <windows.h>
#include "util/print.h"
#include "util/str.h"
#include "util/panic.h"

int __main()
{
    int exit_code = 0;
    print("foo");

    ExitProcess(exit_code);
}