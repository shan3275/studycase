#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){
	int fd;
	char buf[80];
	fd = open( "zieckey_fifo", O_RDONLY );
	while(1) {
		read( fd, buf, sizeof(buf) );
		printf("%s\n", buf);
		sleep(1);
		sleep(1);
	}
	return 0;
}
