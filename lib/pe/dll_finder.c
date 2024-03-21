#include "pe/dll_finder.h"
#include "util/heap.h"
#include "util/str_stack.h"

struct pe_dll_finder
{
    struct str_stack * known_dlls;
    struct str_stack * extra_known_dlls;
    struct str_stack * paths;
    struct str_stack * extra_paths;
    int use_default_search_order;
};

struct pe_dll_finder * pe_dll_finder_new(void)
{
    struct pe_dll_finder * finder = heap_alloc(sizeof(struct pe_dll_finder));
    finder->known_dlls = str_stack_new();
    finder->extra_known_dlls = str_stack_new();
    finder->paths = str_stack_new();
    finder->extra_paths = str_stack_new();
    finder->use_default_search_order = 1;

    return finder;
}

void pe_dll_finder_release(struct pe_dll_finder * finder)
{
    heap_free(finder->extra_paths);
    heap_free(finder->paths);
    heap_free(finder->extra_known_dlls);
    heap_free(finder->known_dlls);
}

int pe_dll_finder_find(struct pe_dll_finder * finder, char const * name,
char * buffer, int buffer_size);

void pe_dll_finder_disable_default_search_order(struct pe_dll_finder * finder, int disable);

void pe_dll_finder_add_search_path(struct pe_dll_finder * finder, char const * path);

void pe_dll_finder_add_known_dll(struct pe_dll_finder * finder, char const * dll);
