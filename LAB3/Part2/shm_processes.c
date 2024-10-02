#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_SIZE 2 * sizeof(int)

void ParentProcess(int *);
void ChildProcess(int *);

int main()
{
     printf("Process has received a shared memory of two integers...\n");
     printf("Process has attached the shared memory...\n");
     printf("Orig Bank Account = 0\n");

    int memoryID;
    int *memoryPointer;
    pid_t processID;
    int status;

    memoryID = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (memoryID < 0)
    {
        fprintf(stderr, "*** shmget error (server) ***\n");
        exit(1);
    }

    memoryPointer = (int *)shmat(memoryID, NULL, 0);
    if (*memoryPointer == -1)
    {
        fprintf(stderr, "*** shmat error (server) ***\n");
        exit(1);
    }

    memoryPointer[0] = 0; // BankAccount
    memoryPointer[1] = 0; // Turn

    processID = fork();

    if (processID < 0)
    {
        fprintf(stderr, "*** fork error ***\n");
        exit(1);
    }
    else if (processID == 0)
    {
        ChildProcess(memoryPointer);
        exit(0);
    }
    else
    {
        ParentProcess(memoryPointer);

        wait(&status);

        shmdt((void *)memoryPointer);
        shmctl(memoryID, IPC_RMID, NULL);
    }
     printf("Process has detected the completion of its child...\n");
     printf("Process has detached its shared memory...\n");
     printf("Process has removed its shared memory...\n");
     printf("Process exits...\n");
     
    return 0;
}

void ParentProcess(int *SharedMemory)
{
    srand(time(NULL));

    int i = 0;
    while (i < 25)
    {
        sleep(rand() % 6); 

        int account = SharedMemory[0];

        while (SharedMemory[1] != 0)
        {
            
        }

        if (account <= 100)
        {
            int balance = rand() % 101;

            if (balance % 2 == 0)
            {
                SharedMemory[0] += balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, SharedMemory[0]);
            }
            else
            {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }

            account = SharedMemory[0];
        }
        else
        {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }

        SharedMemory[1] = 1;
        i++;
    }
}

void ChildProcess(int *SharedMemory)
{
    srand(time(NULL));

    int i = 0;
    while (i < 25)
    {
        sleep(rand() % 6); 

        int account = SharedMemory[0];

        while (SharedMemory[1] != 1)
        {
           
        }

        int balance = rand() % 51;

        printf("Poor Student needs $%d\n", balance);

        if (balance <= account)
        {
            SharedMemory[0] -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, SharedMemory[0]);
        }
        else
        {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }

        account = SharedMemory[0];

        SharedMemory[1] = 0;
        i++;
    }
}