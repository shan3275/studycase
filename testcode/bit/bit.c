/*
*  abort.c - Demonstrate the abort system call
*/

#include <stdlib.h>
#include <stdio.h>

typedef unsigned int uint32_t;
typedef union{
	struct{
		uint32_t index:24;
		uint32_t flag :1;
		union {
			uint32_t rsvd :7;
			uint32_t depth:7;
		};
	};
	uint32_t data;
}test_t;

int main(void)
{
	test_t aa;
	aa.data = 0xFFFFFFFF;
	printf("aa.data=0x%x\n", aa.data);
	printf("aa.index=0x%x\n", aa.index);
	printf("aa.flag=%d\n", aa.flag);
	printf("aa.rsvd=0x%x\n", aa.rsvd);
    printf("End here!\n");
    exit(EXIT_SUCCESS);
}

