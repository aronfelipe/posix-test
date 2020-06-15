#include "mintest/macros.h"
#include <stdio.h>

int test1() {
    char *p = 0;
    fputc(*p, stdout);
    return 0;
}

int test2() {
    test_assert(1 == 0, "This always fails!");
    printf("This never runs!\n");
    test_assert(1 == 1, "Neither this.");
    return 0;
}

int test3() {
    test_printf("<-- Name of the function before the printf!\n");
    test_assert(1 == 1, "This always succeeds");
    return 0;
}

test_list = { TEST(test1), TEST(test2), TEST(test3) };


#include "mintest/runner.h"
