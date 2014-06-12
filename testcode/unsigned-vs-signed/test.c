#include <stdio.h>

int main()
{
    unsigned char b;
    b=(unsigned long)~0==(long)~0u;
    printf("(unsigned long)~0:%ld, (long)~0u:%ld\n",(unsigned long)~0, (long)~0u);
    printf("%d\n",b);
    b=(unsigned short)~0==(short)~0;//为什么不成立？
    printf("(unsigned short)~0:%d, (short)~0:%d\n",(unsigned short)~0, (short)~0);
    printf("%d\n",b);
    b=(unsigned char)~0==(char)~0;//为什么不成立？
    printf("(unsigned char)~0:%d, (char)~0:%d\n",(unsigned char)~0, (char)~0);
    printf("%d\n",b);
    return 0;
} 
/*
int main()
{
    unsigned int un;
    unsigned char uch;
    uch = (unsigned char) 128;
    un = uch;
    printf("un = %d\n", un);
    printf("un = %d\n", un);
    return 0;
}*/
/*
int main()
{
    int     n;
    char    ch;
    ch = (char) 128;
    n = ch;
    printf("n = %d\n", n);
    printf("n = %d\n", n);
    return 0;
}*/
