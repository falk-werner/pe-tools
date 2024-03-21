#ifndef PE_DLL_FINDER_H
#define PE_DLL_FINDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define PE_DLL_FOUND 0
#define PE_DLL_KNOWN 1
#define PE_DLL_NOT_FOUND 2

struct pe_dll_finder;

struct pe_dll_finder * pe_dll_finder_new(void);
void pe_dll_finder_release(struct pe_dll_finder * finder);

int pe_dll_finder_find(struct pe_dll_finder * finder, char const * name,
char * buffer, int buffer_size);

void pe_dll_finder_disable_default_search_order(struct pe_dll_finder * finder, int disable);

void pe_dll_finder_add_search_path(struct pe_dll_finder * finder, char const * path);

void pe_dll_finder_add_known_dll(struct pe_dll_finder * finder, char const * dll);

#ifdef __cplusplus
}
#endif

#endif