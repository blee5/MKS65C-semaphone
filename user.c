#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define KEY 0x25DEDEDE
#define MAXLEN 128

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int main() {
  int semid = semget(KEY, 1, 0);
  union semun us;
  printf("Waiting for others to finish editing...\n\n");
  while (!semctl(semid, 0, GETVAL, us));

  if (semid < 0) {
      printf("Error getting semaphore: %s\n", strerror(errno));
      exit(1);
    }
  struct sembuf op;
  op.sem_num = 0;
  op.sem_op = -1;
  op.sem_flg = SEM_UNDO;
  //printf("Getting semaphore...\n");
  semop(semid, &op, 1);
  //printf("Got semaphore!\n");

  // Display last line in "story.txt" (use shared memory to determine size of last line)
  int shmid = shmget(KEY, MAXLEN, 0644);
  if(shmid == -1){
    printf("Error displaying last line: %s\n", strerror(errno));
  }

  //Pointer to shared memory
  char * shminfo = shmat(shmid, (void *)0, 0);

  //Printing last line
  if(strcmp(shminfo, "")) {
    printf("Last line in \"story.txt\":\n%s\n", shminfo);
  }
  //int shmlength = strlen(shminfo);

  printf("What should the next line be?\n");

  // Write that line story.txt, update shared memory with size of the line
  int storyfd = open("story.txt", O_WRONLY, 0);
  if (storyfd < 0) {
    printf("Error opening storyfd: %s\n", strerror(errno));
  }
  char line[MAXLEN];
  fgets(line, sizeof(line), stdin);
  line[strlen(line)] = 0;

  //Copying line to shared memory
  strncpy(shminfo, line, MAXLEN);

  //appending line to story.txt
  lseek(storyfd, 0, SEEK_END);

  write(storyfd, shminfo, strlen(line));
  write(storyfd, "\n", 1);

  op.sem_op = 1;
  printf("Releasing semaphore\n");
  semop(semid, &op, 1);
  return 0;
}
