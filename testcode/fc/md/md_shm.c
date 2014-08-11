#include <stdlib.h>
#include <stdio.h>
#include "md_init.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include "md_type.h"
#define PATH "/home/shan/tmp/shmid.log"
extern struct mpp_shared_data *gsdata;
int md_mk_shm(void)
{
    int shmid;
    FILE *fd;
    char buff[32];
    int len = 0;
    if ((shmid = shmget(IPC_PRIVATE, sizeof(struct mpp_shared_data), 0666)) < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    printf("segment created: %d\n", shmid);
    memset((void *)buff, 0, sizeof(buff));
    sprintf(buff, "ipcs -m -i %d ", shmid);
    system(buff);

    fd = fopen(PATH, "w+");
    if (fd == NULL)
    {
        return MD_OPEN;
    }
    memset((void *)buff, 0, sizeof(buff));
    len = sprintf(buff, "%d", shmid);
    if (fwrite(buff, sizeof(char), len, fd) != len)
    {
        return MD_FAIL;
    }
    if (fclose(fd))
    {
        return MD_FAIL;
    }

    return MD_SUCCESS;
}

int md_get_shm(void)
{
    int shmid;
    FILE *fd;
    char buff[32];
    int len = 0;
    fd = fopen(PATH, "r");
    if (fd == NULL)
    {
        return MD_OPEN;
    }


    memset((void *)buff, 0, sizeof(buff));
    len = fscanf(fd, "%d", &shmid);
    if ( len )
    {
        memset((void *)buff, 0, sizeof(buff));
        sprintf(buff, "ipcs -m -i %d ", shmid);
        system(buff);
        gsdata = (struct mpp_shared_data *)shmat(shmid,NULL,0);
    }
    else
    {
        return MD_INIT;
    }
    if (fclose(fd))
    {
        return MD_FAIL;
    }

    return MD_SUCCESS;
}
