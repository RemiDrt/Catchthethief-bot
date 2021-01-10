#include <stdio.h>
struct  test
{
    int a;
    int b;
    int c[2][2];
};
struct test2
{
    int a;
};

struct essai{
    int a;
    int b;
    struct test* c;
    struct test2* d[2];
};

int main(int argc, char const *argv[])
{
    struct test a;
    scanf("%d %d", &(a.a), &(a.b));
    printf(" : %d %d %d\n", a.a, a.b, a.c[0][0]);
    a.a++;
    printf("%d\n", a.a);
    return 0;
}
