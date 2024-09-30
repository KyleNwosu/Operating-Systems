#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int rdnum = 31;
int ten = 11;

void  ChildProcess(void);              
void  ParentProcess(void); 

int main(void){
    ParentProcess();
    return 0;
}



void ChildProcess(void){
  int i;
  for(i= 1; i <= rand()%rdnum; i++){
    printf("Child Pid: %d is going to sleep \n",getpid());
    sleep(rand()%ten);
    printf("Child Pid: %d is awake. \nWhere is my parent:%d\n", getpid(), getppid());
  }
  exit(0);
}

void ParentProcess(void){
  int status;
  pid_t pida,pidb;
  pida = fork();
  pidb = fork();

  if(pida ==0)
    ChildProcess();
  else if (pida>0){
    wait(&status);
    printf("Child Pid: %d has completed\n ", pida);
  }

  else {
		printf("Child 1 could not be forked.");
  }

  if (pidb==0)
    ChildProcess();
  else if (pidb>0){
    wait(&status);
    printf("Child Pid: %d has completed", pidb);
  }
  else {
		printf("Child 2 could not be forked.");
  }
}

 
 
