// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file str_stack.h
/// \brief Interface of stack of strings.

#ifndef UTIL_STR_STACK_H
#define UTIL_STR_STACK_H

/// @brief Stack of strings
struct str_stack
{
    char * * data;          ///< Array of string on the stack.
    unsigned int size;      ///< Current count of string on the stack.
    unsigned int capacity;  ///< Total capacity of the stack.
};

/// @brief Returns a new stack of strings.
/// @return Newly created stack of strings.
struct str_stack * str_stack_new(void);

/// @brief Releases a stack of string.
/// @param stack Stack to release.
void str_stack_release(struct str_stack * stack);

/// @brief Pushes a new string on the stack.
///
/// The string will be copied before push.
///
/// @param stack Pointer to the stack.
/// @param value String to push.
void str_stack_push(struct str_stack * stack, char const * value);

/// @brief Removes a string from the stack.
///
/// The popped string must be released using \fn heap_free.
///
/// @param stack 
/// @return Popped string of 0 if stack was empty.
char * str_stack_pop(struct str_stack * stack);

/// @brief Checks if a string is already on the stack.
/// @param stack Pointer to the stack.
/// @param value String to check
/// @return 1 of string is on stack, 0 otherwise
int str_stack_contains(struct str_stack * stack, char const * value);


#endif
