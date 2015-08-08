/*
*  system.c - Demonstrate the system() call
*/

#include <stdlib.h>
#include <stdio.h>


#if 0
void  main(void)
{
    char *p1 = "testy";
    char *p2 = (char *)malloc(10);
    memset((void *)p2, 0, sizeof(p2));
    memcpy((void *)p2, (void *)p1, strlen(p1));
    //memcpy((void *)p2, (void *)p1, sizeof(p1));
    printf("%d  %d %s\n", sizeof(p2), strlen(p2), p2);
}
#endif
#if 0
void  main(void)
{
    int x = 5;
    printf("%d, %d, %d\n", x <<=1, x<<=2, x >>=3);
}
#endif
#if 1
void getPortStr(int portNum, char *pStr)
{
    sprintf(pStr, "The Port Number is: %d\r\n", portNum);
}
void  main(void)
{
    char str[24] = {0};
     int port = 32000;
     getPortStr(port, str);
     printf("%s", str);
}
#endif

#if 0

int main(int argc, char **argv)
{
    char *pStr = "This is a test.";
    char str[] = "Hello world";
    printf("%s", strcpy(&pStr[2], str));

    return 0;
}
#endif

