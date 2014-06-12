#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
void dynamicPrint(char str [],unsigned int usecond)
{
	int i;
	printf("%s",str);
	char achar[4]={'-','//','/'};
	for(i=0;i<101;++i)
        {
	      printf("\r%c %2.0f%%",achar[i%3],(i/100.0)*100);
              //fflush(stdout);
              usleep(usecond);
              //printf("/b/b/b/b/b");
	      fflush(stdout);
        }
}
 
int main()
{
	unsigned int us=500000;
	char str[]="Good luck,your file has downloaded.....\n";
	dynamicPrint(str,us);
        return 0;
}
