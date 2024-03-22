#ifndef UTIL_STR_H
#define UTIL_STR_H

unsigned int str_len(char const * value);

void str_cp(char * target, char const * source);

int str_eq(char const * a, char const * b);

int str_eqi(char const * a, char const * b);

int str_find(char const * value, char c);

int str_starts_with(char const * value, char const * prefix);

char * str_dup(char const * s);

char * u2str(unsigned long long value);

char * u2hexstr(unsigned long long value);


#endif
