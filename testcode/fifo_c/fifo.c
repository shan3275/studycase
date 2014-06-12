#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MaxSize  10

typedef struct
{
 char cBuff[MaxSize];
 int  in,out,count;
}queue;

void InitQueue(queue* pQ)
{
    pQ->in  = 0;
    pQ->out = 0;
    pQ->count = 0;
}

void InQueue(queue* pQ,char cInData)
{
    if (pQ->count==MaxSize)
    {
     printf("\nQueue is overflow!\n");
     return;
    }

    pQ->cBuff[pQ->in++] =  cInData;
    pQ->in %= MaxSize;
    pQ->count++;
}

char OutQueue(queue* pQ)
{
    char ret;

    if (pQ->count==0)
    {
        printf("\nqueue is blank\n");
        return(0);
    }

    pQ->count--;
    ret = pQ->cBuff[pQ->out];
    pQ->out++;
    pQ->out %= MaxSize;
    return(ret);
}

int main()
{
    queue Q;
    char i=0,sel=0;

    InitQueue(&Q);

    printf("queue test start!\n");
    do
    {
        sel = getchar();

        switch(sel)
        {
            case 'a':
                 for (i=0;i<MaxSize;i++)
                    InQueue(&Q,'0'+ i);

                  for (i=Q.count-1;i>=0;i--)
                    printf("%c",OutQueue(&Q));
                  InQueue(&Q,'a');
                 break;

         case 'b':
                 for (i=0;i<MaxSize+2;i++)
                 {
                    InQueue(&Q,'a'+ i);
                    printf("In=%c\t",'a'+i);
                 }

                 for (i=Q.count-1;i>=0;i--)
                 {
                     printf("%c",OutQueue(&Q));
                 }
                  InQueue(&Q,'b');
                 break;
          case 'c':
                 for (i=0;i<MaxSize/2;i++)
                 {
                    InQueue(&Q,'a'+ i);
                    printf("\nc_In=%c\t",'a'+i);
                 }

                 InQueue(&Q,'z');
                 InQueue(&Q,'y');

                 for (i=Q.count-1;i>=0;i--)
                 {
                     printf("%c",OutQueue(&Q));
                 }

                 InQueue(&Q,'c');
          default:
                 break;
        }
     }while(sel!='e');

     return 0;
}