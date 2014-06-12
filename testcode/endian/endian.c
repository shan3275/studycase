#include<stdio.h>
int main(void)
{
	int a = 0x12345678;
	unsigned char *p=(unsigned char *)&a;
	if(0x78==*p)
	{
	    printf("little end\n");
	}
	else
	{
	    printf("big end\n");
	}
	return 0;
}

