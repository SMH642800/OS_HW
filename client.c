#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PERMS 0666
#define MAXBUF 100

int inputchat(){
  int shmid;
  key_t key = 1234;
  char *shm;
  char str_buf[MAXBUF]="";

  /* attach the shared memory from server creating */
  if ((shmid = shmget(key, MAXBUF+1, PERMS)) < 0) {
    perror("shmget");
    exit(1);
  } 
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }

  while (1) {
    int output_len = 0;
    char output[100]="";
    
    printf("Please enter two numbers with using a blank to separate it (e.g. '12 34'):\n");
    strcpy(str_buf, "");
    fgets(str_buf, 100, stdin);
    strcpy(shm, str_buf);
    
    sleep(1);  // wait 1 sec

    /* break when the string get the sum in shared memory */
    while(strstr(shm, "ok") == NULL){ 
        continue;
    }
    output_len = strlen(shm);
    strncpy(output, shm+2, (output_len-2));
    printf("Sum = %s \n\n", output);
    strcpy(shm, "");
  }

}

int main(int argc, char **argv){
  pid_t fpid;
  char c;

  printf("pid %d\n", getpid());

  inputchat();

  return 0;
}


