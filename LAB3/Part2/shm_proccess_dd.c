#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_KEY 0x1234
#define NUM_LOOPS 25


struct shared_data{
    int Baccount;
    int turn;
};


void dad(struct shared_data *data){
    int account;

    srand(time(NULL)^ getpid());
    int sleep_time = rand() % 6;
    sleep(sleep_time);

    while(data->turn != 0);
    account = data->Baccount;

    if(account <= 100){
        int deposit = rand() % 101;
        if(deposit % 2 == 0){
            account += deposit;
            data->Baccount = deposit;
            printf("Dear old Dad: Deposits $%d / deposit = $%d\n", deposit, account);
        }
        else{
            printf("Dear old Dad: Doesn't have any money to give\n");
        }
    }
    else{
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }
    data->turn = 1;

    }

void student(struct shared_data *data){
    int account;

    srand(time(NULL)^ getpid());
    int sleep_time = rand() % 6;
    sleep(sleep_time);

    while (data->turn != 1);

    int needed = rand() % 51;
    printf("Poor Student needs $%d\n", needed);
    account = data->Baccount;
    if (needed <= account){
        account -= needed;
        printf("Student Withdraws $%d / Balance = $%d\n", needed, account);
    }
    else{
        printf("Student: Doesn't have enough cash ($%d)\n", account);
    }
    data->turn = 0;
}


int main(){
    int shmid;
    struct shared_data *data;

    shmid  = shmget(SHM_KEY, sizeof(struct shared_data),  IPC_CREAT | 0666);
    if (shmid ==-1){
        perror("shmget: Failure");
        exit(1);
    }

    data = (struct shared_data *)shmat(shmid, NULL,0);
    if (data == (void *)-1){
        perror("shmat: Failure");
        exit(1);
    }

    data->Baccount = 0;
    data->turn = 0;

    pid_t pid = fork();

    if (pid < 0){
        perror("fork: Failure");
        exit(1);
    }
    else if (pid > 0){
        for (int i = 0; i < NUM_LOOPS; i++){
            dad(data);
        }
        wait(NULL);

        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        printf("Parent: Complete\n");

    }
    else{
        for (int i = 0; i < NUM_LOOPS; i++){
            student(data);
        }

        shmdt(data);
        printf("Child: Complete\n");
    }
    return 0;

}