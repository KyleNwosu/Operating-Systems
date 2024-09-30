/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int handled = 0;
int running = 1;
int alarms = 1;

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
    handled = 1; //exit after printing
}

void sig_handler(int sign){
  printf("\nTotal alarms sent: %d\n", alarms);
  printf("Execution duration: %d seconds\n", alarms);
  running = 0;
  exit(0);
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); 
	signal(SIGINT, sig_handler);

  while(running){
    alarm(1);
    pause();

    if (handled){
      printf("Turing was right!\n");
      handled = 0;
      alarms ++;
    }
  }
  return 0;

}