#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
int main(void){
	int fd;
	char s[] = "Hello!\n";
	fd = open( "zieckey_fifo", O_WRONLY );
	while(1) {
		write( fd, s, sizeof(s) );
		sleep(1);
	}
	return 0;
}
