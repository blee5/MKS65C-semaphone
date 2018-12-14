#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define KEY 0x25DEDEDE

int main()
{
    int semid = semget(KEY, 1, 0);
    if (semid < 0)
    {
        printf("Error getting semaphore: %s\n", strerror(errno));
        exit(1);
    }
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO;
    printf("Getting semaphore...\n");
    semop(semid, &op, 1);
    printf("Got semaphore!\n");
    // TODO:
    // Display last line in "story.txt" (use shared memory to determine size of last line)
    /* fgets(buf, ??, stdin); */
    // Write that line to file, update shared memory with size of the line
    op.sem_op = 1;
    printf("Releasing semaphore\n");
    semop(semid, &op, 1);
    return 0;
}
