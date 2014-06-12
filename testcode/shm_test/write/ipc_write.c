#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char name;
	int age;
}people;

int main(int argc,char **argv)
{
     int shm_id;
     char temp_char;
     people *p_map;

     shm_id=shmget(IPC_PRIVATE,4096,IPC_CREAT);
     if(shm_id<0)
        {perror("shmget error");
	return 0;
	}else {
        printf("shm_id=%d\n", shm_id);
	}
     p_map=(people*)shmat(shm_id,NULL,0);
     temp_char='a';
     for(;;)
     {
        (*(p_map)).name=temp_char;
        (*(p_map)).age=rand();    
	printf("name=%c, age=%d\n", p_map->name, p_map->age);
	sleep(1);
     }
     if(shmdt(p_map)<0)
    perror("detach error");
    
     return 0;
}
