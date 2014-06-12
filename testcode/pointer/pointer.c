#include <stdio.h>

typedef struct {
    int a;
    int b;
}test;
int main(void)
{
    int a=4;
    int b=-1;
    int * p=NULL;
    printf("a=0x%x, addr=%p\n",a, &a);
    printf("p=0x%x, addr=%p\n",p, &p);
    //printf("*p=0x%x\n",*p);

    p=&a;
    printf("a=0x%x, addr=%p\n",a, &a);
    printf("p=0x%x, addr=%p\n",p, &p);
    printf("*p=0x%x\n",*p);

    /* struct test */
    test *test_p;
    test testt;

    testt.a=6;
    testt.b=7;
    test_p = &testt;
    printf("*test_p:%d\n", *test_p);
    printf("%d", b);

    test **test_pp;
    test_pp = &test_p;

    return 0;
}
