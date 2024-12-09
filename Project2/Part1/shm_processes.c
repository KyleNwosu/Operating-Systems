#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_SIZE sizeof(int) // Shared memory size for BankAccount

void dear_old_dad(sem_t *sem, int *bank_account);
void poor_student(sem_t *sem, int *bank_account);

int main() {
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    sem_t *sem;

    // Create shared memory
    ShmID = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (ShmID < 0) {
        perror("shmget failed");
        exit(1);
    }

    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize shared memory with 0 (BankAccount)
    *ShmPTR = 0;

    // Create semaphore
    sem = sem_open("/bank_semaphore", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    printf("Shared memory and semaphore initialized.\n");

    // Fork the Dear Old Dad process
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        dear_old_dad(sem, ShmPTR);
        exit(0);
    }

    // Fork the Poor Student process
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        poor_student(sem, ShmPTR);
        exit(0);
    }

    // Parent process waits for both children to finish
    wait(NULL);
    wait(NULL);

    // Cleanup
    sem_close(sem);
    sem_unlink("/bank_semaphore");
    shmdt(ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);

    printf("Resources cleaned up. Exiting.\n");
    return 0;
}

void dear_old_dad(sem_t *sem, int *bank_account) {
    srand(time(NULL) ^ getpid());

    while (1) {
        sleep(rand() % 6); // Sleep for 0–5 seconds

        printf("Dear Old Dad: Attempting to Check Balance\n");

        sem_wait(sem); // Enter critical section
        int local_balance = *bank_account;

        if (rand() % 2 == 0) { // If random number is even
            if (local_balance < 100) {
                int deposit = rand() % 101; // Random deposit between 0–100
                *bank_account += deposit;
                printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", deposit, *bank_account);
            } else {
                printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", local_balance);
            }
        } else {
            printf("Dear Old Dad: Last Checking Balance = $%d\n", local_balance);
        }
        sem_post(sem); // Exit critical section
    }
}

void poor_student(sem_t *sem, int *bank_account) {
    srand(time(NULL) ^ getpid());

    while (1) {
        sleep(rand() % 6); // Sleep for 0–5 seconds

        printf("Poor Student: Attempting to Check Balance\n");

        sem_wait(sem); // Enter critical section
        int local_balance = *bank_account;

        if (rand() % 2 == 0) { // If random number is even
            int need = rand() % 51; // Random need between 0–50
            printf("Poor Student needs $%d\n", need);

            if (need <= local_balance) {
                *bank_account -= need;
                printf("Poor Student: Withdraws $%d / Balance = $%d\n", need, *bank_account);
            } else {
                printf("Poor Student: Not Enough Cash ($%d)\n", local_balance);
            }
        } else {
            printf("Poor Student: Last Checking Balance = $%d\n", local_balance);
        }
        sem_post(sem); // Exit critical section
    }
}
