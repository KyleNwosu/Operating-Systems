/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int handled = 0;

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  handled = 1; //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGARLM,handler);

  while(1){
    alarm(1);
    pause();

    if (handled){
      printf("Turing was right!");
      handled = 0;
    }
  }
  return 0;

}