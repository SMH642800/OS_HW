#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define PERMS 0666
#define MAXBUF 100


int outputchat(){
  int shmid;
  key_t key = 1234;
  char *shm, *s;

  /*Create shared memory space*/
  if ((shmid = shmget(key, MAXBUF+1, IPC_CREAT | PERMS)) < 0) {
    perror("shmget");
    exit(1);
  }
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }


  while(1){
    int num1=0, num2=0;
    int sum=0;
    char s_num1[50]="";
    char s_num2[50]="";
    char s_sum[100]="";

    shm[MAXBUF] = 1;
    while(strstr(shm, " ") == NULL){  /* detect the string from shared memory until get the string with "num1 num2" */
      if(shm[MAXBUF] == 1){
        continue;
      }
    }
    printf("Get two numbers from client: %s", shm);
    
    /* using flag=true to detect that num1 is complete and switch to num2 */
    bool flag = false;
    int count = 0;
    for (int i = 0; i < strlen(shm); i++) {
        if (shm[i] == ' ') { 
            s_num1[i] = '\0';
            count += 1;
            flag = true;
        } 
        else if (shm[i] == '\n') {   // delete '/n' at the end of the string
            s_num2[i] = '\0';
        } 
        else if (flag == false) {
            s_num1[i] = shm[i];
            count += 1;
        }
        else if (flag == true){
            s_num2[i-count] = shm[i];
      }
    }
    num1 = atoi(s_num1);
    num2 = atoi(s_num2);
    sum = num1 + num2;
    sprintf(s_sum, "%d", sum);
    printf("Sum = %d\n\n", sum);
    
    strcpy(shm, "");
    strcpy(shm, "ok");
    strcat(shm, s_sum);
  }

}


int main(int argc, char **argv){
  pid_t fpid;
  char c;

  printf("pid %d\n", getpid());

  outputchat();

  return 0;
}

