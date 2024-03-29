#include <stdio.h>

typedef int (*test_func)();

typedef struct {
    char name[50];
    test_func function;
} test_data;

#define test_printf (printf("%s: ", __func__));printf

#define test_assert(expr, str) { if(!(expr)) {return -1; }}

#define TEST(f) {.name=#f, .function=f}

#define test_list test_data all_tests[]