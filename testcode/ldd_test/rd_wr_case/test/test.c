#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <getopt.h>

char *l_opt_arg;
char buf[16]={0};
static const char *shortopts = "w:r";
struct option longopts[] = {
{"read", no_argument, NULL, 'r'},
{"write", required_argument, NULL, 'w'},
{0, 0, 0, 0},
};

int read_test(char *buf);
int write_test(char *buf);

int main (int argc, char *argv[])
{
int c;

while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      switch (c)
   {
   case 'r':
      if (read_test(buf))
      {
	      printf("Read failed!\n");
	      return 1;
      }
      printf ("Read from kernel: %s.\n",buf);
      break;
   case 'w':
      l_opt_arg = optarg;
      if (write_test(l_opt_arg))
      {
	      printf("Write failed!\n");
	      return 1;
      }
      break;
   }
    }
return 0;
}

int read_test(char *buf)
{
	int fd;
	fd = open("/dev/rd_wr", O_RDWR, 777);
	if(fd < 0 )
	{
		printf("open /dev/rd_wr failed!\n");
		return 1;
	}
	read(fd, buf, 16);
	close(fd);
	return 0;
}

int write_test(char *buf)
{
	int fd;
	fd = open("/dev/rd_wr", O_RDWR, 777);
	if(fd < 0 )
	{
		printf("open /dev/rd_wr failed!\n");
		return 1;
	}
	write(fd, buf, 16);
	close(fd);
	return 0;
}
