#include <stdio.h>

int main(int argc,char *argv[])
{
	FILE* cons;

	if (argc < 2)
	{
		printf("input like this: ./pts_write /dev/pts/n \n");
		return 1;
	}

	cons = fopen(argv[1], "a");
	fprintf(cons, "\nThis is a test. write to %s\n",argv[1]);
	fclose(cons);
	return 0;    
}
