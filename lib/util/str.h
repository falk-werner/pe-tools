// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file str.h
/// \brief Interface of string handling functions.

#ifndef UTIL_STR_H
#define UTIL_STR_H

/// @brief Returns the length of the string without '\0' terminator.
/// @param value Pointer to a string.
/// @return Length of the string without '\0' terminator.
unsigned int str_len(char const * value);

/// @brief Copy a string to a buffer.
/// @param target Buffer to store the string.
/// @param source Source of the copy.
void str_cp(char * target, char const * source);

/// @brief Checks whether two string are equal.
/// @param a a string
/// @param b another string
/// @return 1 if the strings are equal, 0 otherwise.
int str_eq(char const * a, char const * b);

/// @brief Checks whether two strings are equal ignoring case.
/// @param a a string
/// @param b another string
/// @return 1 if the strings are equal ignoring case, 0 otherwise. 
int str_eqi(char const * a, char const * b);

/// @brief Finds a character within a string.
/// @param value String to search.
/// @param c Character to find.
/// @return Index of the character within the string or -1 if not found.
int str_find(char const * value, char c);

/// @brief Checks whether a string starts with a prefix.
/// @param value String to check.
/// @param prefix Prefix.
/// @return 1 if the string starts with the prefix, 0 otherwise.
int str_starts_with(char const * value, char const * prefix);

/// @brief Duplicated a string.
///
/// The duplicated string must be released using \fn heap_free.
///
/// @param s String to duplicate.
/// @return Newly allocated duplicate of the string.
char * str_dup(char const * s);

/// @brief Converts a string to a string.
///
/// This function is not reentrant since it
/// uses a global buffer.
///
/// @param value Value to convert.
/// @return Converted string.
char * u2str(unsigned long long value);

/// @brief Converts a value into a hex string.
///
/// This function is not reentrant since it
/// uses a global buffer.
///
/// @param value Value to convert.
/// @return Converted hex string.
char * u2hexstr(unsigned long long value);


#endif
