/*函数名: atoi
功  能: 把字符串转换成长整型数
用  法: int atoi(const char *nptr);
程序例:*/
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
   int n;
   char *str = "   12345M_116";

   n = atoi(str);
   printf("string = %s integer = %d\n", str, n);
   return 0;
}
