#ifndef PE_ERROR_H
#define PE_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

struct pe_error;

void pe_error_release(struct pe_error * err);

char const * pe_error_message(struct pe_error const * err);

#ifdef __cplusplus
}
#endif

#endif
