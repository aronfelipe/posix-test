#include "mintest/macros.h"
#include <stdio.h>

int test1() {
    char *p = 0;
    fputc(*p, stdout);
    return 0;
}

int test2() {
    int divison = 100/0;
    return 0;
}

int test3() {
    while(1) {
        
    }
}

test_list = { TEST(test1), TEST(test2), TEST(test3) };


#include "mintest/runner.h"
