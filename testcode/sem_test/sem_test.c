#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
 
union semun 
{ 
    int val; 
    struct semid_ds *buf; 
    unsigned short int *array; 
    struct seminfo *__buf; 
}; 
 
int main(void) 
{ 
    char* buf_child[]={"this", "is", "the", "child", "process"}; 
    char* buf_father[]={"father", "say", "hello", "to", "child"}; 
    int i = 0, semid, fd; 
    pid_t pid; 
    struct sembuf sb; //信号量操作
    union semun sem; 
    semid = semget(1000, 2, 0666 | IPC_CREAT); //申请信号量组，包含2个信号量
 
    sem.val = 0; 
    semctl(semid, 0, SETVAL, sem); //初始化0号信号量为0
    sem.val = 1; 
    semctl(semid, 1, SETVAL, sem); //初始化1号信号量为1
 
    fd=open("tmp",O_CREAT|O_TRUNC|O_WRONLY,0666); 
 
    pid = fork(); 
    switch (pid) { 
        case -1: 
            perror("fork fail"); 
            break; 
        case 0: /* child consume */ 
            srand((unsigned int)getpid()); 
            while (i < 5) { 
                sb.sem_num = 1; //将1号信号量
                sb.sem_op = -1; //减1
                sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT; 
                semop(semid, &sb, 1); 
 
                write(fd,buf_child[i], strlen(buf_child[i])); 
                //sleep(rand()); 
                sleep(1); 
                write(fd,&" ", 1); 
                i++; 
 
                sb.sem_num = 0; //将0号信号量
                sb.sem_op = 1;  //加1
                sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT; 
                semop(semid, &sb, 1); //操作信号量
            } 
            break; 
        default:/* parent production  */ 
            srand((unsigned int)getpid()); 
            while (i < 5) { 
                sb.sem_num = 0; //将0号信号量
                sb.sem_op = -1; //减1
                sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT; 
                semop(semid, &sb, 1); //操作信号量
 
                write(fd,buf_father[i], strlen(buf_father[i])); 
                //sleep(rand()); 
                sleep(1); 
                write(fd,&" ", 1); 
                i++; 
 
                sb.sem_num = 1; 
                sb.sem_op = 1; 
                sb.sem_flg = sb.sem_flg & ~IPC_NOWAIT; 
                semop(semid, &sb, 1); 
            } 
            break; 
    } 
    return 0; 
} 
