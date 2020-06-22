#include "mintest/macros.h"
#include <stdio.h>
#include <unistd.h>

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

    return 0;
}

int test4() {
    for(int i = 0; i < 10000000; i++) {
    }
    test_printf("Working... \n");

    return 0;
}

int test5() {
    test_assert(1 == 1, "Smoothie.");
    test_assert(420 == 420, "Sounds good");
    test_assert(1 == 0, "This always fails!");
    test_assert(1 == 1, "Neither this.");
    return 0;
}

int test6() {
    for(int i = 0; i < 10; i++) {
        test_printf("%d \n", i);
        for (int j = 0; j < 10; j++) {
            test_printf("%d \n", j);
        } 
    }
    return 0;
}

int test7() {
    while(1) {
        sleep(1);
        return 0;
    }
}

int test8() {
    sleep(0.0001);
    return 0;
}

test_list = { TEST(test1), TEST(test2), TEST(test3), TEST(test4), TEST(test5), TEST(test6), TEST(test7), TEST(test8)};

#include "mintest/runner.h"
