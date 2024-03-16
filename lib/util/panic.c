#include "util/panic.h"
#include "util/print.h"

#include <windows.h>

void panic(char const * message)
{
    print_error(message);
    ExitProcess(1);
}
