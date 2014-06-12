#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
	char name;
	int age;
}people;
int main(int argc,char **argv)
{
     int shm_id;
     people *p_map;
     struct shmid_ds shmbuf;

    /* Expect a segment id on the command line */
    if (argc != 2) {
        puts("USAGE: ipc_read <identifier>");
        exit(EXIT_FAILURE);
    }
    shm_id = atoi(argv[1]);

     p_map=(people*)shmat(shm_id,NULL,0);
     for(;;)
     {
        printf("name=%c",(*(p_map)).name);
        printf("age=%d\n",(*(p_map)).age);
	sleep(1);
     }
     if(shmdt(p_map)<0)
          perror("shmdt error");
     if(shmctl(shm_id,IPC_RMID,&shmbuf)<0)
          perror("shmctl error");
    
    return 0;
}
