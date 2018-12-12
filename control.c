#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define KEY 0x25DEDEDE

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void setup()
{
    int shmid = shmget(KEY, 500, IPC_CREAT | IPC_EXCL | 0644);
    if (shmid < 0)
    {
        printf("Error creating shared memory: %s\n", strerror(errno));
    }
    int fd = open("story.txt", O_TRUNC | O_CREAT | O_EXCL);
    if (fd < 0)
    {
        printf("Error creating file: %s\n", strerror(errno));
    }
    int semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semid < 0)
    {
        printf("Error creating semaphore: %s\n", strerror(errno));
    }
}

void cleanup()
{
    int shmid = shmget(KEY, 500, 0);
    if (shmid < 0)
    {
        printf("Error removing shared memory: %s\n", strerror(errno));
    }
    shmctl(shmid, IPC_RMID, NULL);
    int semid = semget(KEY, 1, 0);
    if (semid < 0)
    {
        printf("Error removing semaphore: %s\n", strerror(errno));
    }
    semctl(semid, 0, IPC_RMID, 0);
    if (remove("story.txt") < 0)
    {
        printf("Error removing file: %s\n", strerror(errno));
    }
}

void view_story()
{
    int fd = open("story.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("Error reading file: %s\n", strerror(errno));
    }
    else
    {
        // TODO: read file
    }
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Invalid arguments.\nFormat: control -[c/r/v]\n");
        return -1;
    }
    if (strcmp(argv[1], "-c") == 0)
    {
        setup();
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        cleanup();
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        view_story();
    }
    else
    {
        printf("Invalid arguments.\nFormat: control -[c/r/v]\n");
        return -1;
    }
    return 0;
}
