#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define READ_BUFF 100
int main (int argc,char *argv[])
{

    if(argc != 3)
    {
        printf("Input like this:\n");
        printf("./readwrite readfile writefile!\n");
    }
    int nread = 0;
    char buff[READ_BUFF]={0};
    FILE *fp_read,*fp_write;
    if((fp_read=fopen(argv[1],"rb"))==NULL)
    {
        printf("Cann't open %s !",argv[1]);
        return 1;
    }
    if((fp_write=fopen(argv[2],"wb"))==NULL)
    {
        printf("Cann't open %s !",argv[2]);
        return 1;
    }
    fwrite(buff,sizeof(char),32,fp_write);
    while((nread=fread(buff,sizeof(char),READ_BUFF,fp_read))>0)
        fwrite(buff,sizeof(char),nread,fp_write);

    fclose(fp_read);
    fclose(fp_write);
    return 0;
}
