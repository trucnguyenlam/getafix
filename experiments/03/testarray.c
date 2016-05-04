#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

int c;

int* f(void)
{
    return &c;
}


int main(int argc, char const *argv[])
{
    int a[10];
    int *d;

    d = f();

    printf("%ld\n", &a);

    return 0;
}