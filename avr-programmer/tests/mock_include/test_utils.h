/******************************************************************************
 * z80-board-firmware
 * test_utils.h - common test functions
 *
 *****************************************************************************/

#include <stdbool.h>
#include <stdio.h>

#ifndef Z80_BOARD_TEST_UTILS_H
#define Z80_BOARD_TEST_UTILS_H

extern bool pass;
extern int testcount;

static inline bool test_assert_func(bool check, const char* name, const char* function)
{
    if(!check)
    {
        fprintf(stderr, "%d. FAIL: %s (in %s)\n", testcount, name, function);
    }
    else
    {
        fprintf(stderr, "%d. PASS\n", testcount);
    }

    return check;
}

#define TEST_INIT() bool pass = true; int testcount = 1
#define STRINGIFY(X) #X

#define TEST_ASSERT(X) ++testcount; pass &= test_assert_func((X), STRINGIFY(X), __FUNCTION__)
#define TEST_ASSERT_EQ(A,B) ++testcount; pass &= test_assert_func((A) == (B), STRINGIFY(A)  " != " STRINGIFY(B), __FUNCTION__)

#define TEST_FUNCTION(X) X(); if(!pass) return 1
#define TEST_END() return pass ? 0 : 1

#endif
