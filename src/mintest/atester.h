#include <stdio.h>

typedef int (*test_func)();

typedef enum test_type { segmentation, division, loop, work, assert, print, slow, fast }; 

typedef struct {
    char name[50];
    test_func function;
    test_type type;
} test_data;

#define test_printf (printf("%s: ", __func__));printf

#define test_assert(expr, str) { if(!(expr)) { printf("%s: [FAIL] %s in %s:%d\n", __func__, str, __FILE__, __LINE__); return -1; }  else { printf("%s: [PASS] %s in %s:%d\n", __func__, str, __FILE__, __LINE__); return 1; }}

#define test_slow()

#define test_fast()

#define TEST(f) {.name=#f, .function=f}

#define test_list test_data all_tests[]