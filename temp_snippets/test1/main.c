//
// Created by bili on 2015/12/25.
//

#include <stdio.h>

int absolute_value(int x);
void foo(int, int);
int test_enum(void);
int add_range(int, int);
int test_add_range(void);

/* main: generate some simple output */
int main(void) {
//    printf("Hello, world. %f\n", sin(3.1416/2));
//    printf("absolute_value of (%d) is %d\n", -5, absolute_value(-5));
//    printf("absolute_value of (%d) is %d\n", 0, absolute_value(0));
//    foo(1, 2);
//    test_enum();
    test_add_range();
    return 0;
}

int absolute_value(int x) {
    if (x < 0) {
        return -x;
    } else if (x > 0) {
        return x;
    }
}

void foo(int x, int y) {
    if (x >= 0 || y >= 0) {
        printf("both x and y are positive.\n");
        return;
    } else if (x < 0 || y < 0) {
        printf("both x and y are negetive.\n");
        return;
    }
    printf("x has a different sign from y.\n");
}

enum coordinate_type {
    RECTANGULAR = 1, POLAR
};

int test_enum(void) {
    int RECTANGULAR;  // shadow the enum RECTANGULAR
    printf("%d %d\n", RECTANGULAR, POLAR);
    return 0;
}

int add_range(int low, int high) {
    int i, sum;
    for (i = low; i <= high; i++)
        sum = sum + i;
    return sum;
}

int test_add_range(void) {
    int result[100];
    result[0] = add_range(1, 10);
    result[1] = add_range(1, 100);
    printf("result[0]=%d\nresult[1]=%d\n", result[0], result[1]);
    return 0;
}
