#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20

// Matrices
int matA[MAX][MAX], matB[MAX][MAX];
int matSumResult[MAX][MAX], matDiffResult[MAX][MAX], matProductResult[MAX][MAX];

// Thread argument structure
typedef struct {
    int start, end;
} ThreadArgs;

// Function to fill a matrix with random values
void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

// Function to print a matrix
void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to compute the sum of matrices
void* computeSum(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*) args;
    int start = thread_args->start;
    int end = thread_args->end;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    free(args);
    return NULL;
}

// Function to compute the difference of matrices
void* computeDiff(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*) args;
    int start = thread_args->start;
    int end = thread_args->end;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    free(args);
    return NULL;
}

// Function to compute the product of matrices
void* computeProduct(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*) args;
    int start = thread_args->start;
    int end = thread_args->end;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    free(args);
    return NULL;
}

// Function to spawn threads for a matrix operation
void spawnThreads(pthread_t* threads, int thread_count, void* (*computeFunc)(void*)) {
    for (int i = 0; i < thread_count; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->start = i * (MAX / thread_count);
        args->end = (i == thread_count - 1) ? MAX : (i + 1) * (MAX / thread_count);

        if (pthread_create(&threads[i], NULL, computeFunc, (void*) args) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    for (int i = 0; i < thread_count; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            exit(1);
        }
    }
}

// Main function
int main() {
    srand(time(0));  // Seed for random number generation

    // Fill matrices with random values
    fillMatrix(matA);
    fillMatrix(matB);

    // Print the initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // Create threads for each operation
    int thread_count = 4;  // Number of threads to spawn
    pthread_t threads[thread_count];

    // Compute and print the sum of matrices
    spawnThreads(threads, thread_count, computeSum);
    printf("Sum of Matrices:\n");
    printMatrix(matSumResult);

    // Compute and print the difference of matrices
    spawnThreads(threads, thread_count, computeDiff);
    printf("Difference of Matrices:\n");
    printMatrix(matDiffResult);

    // Compute and print the product of matrices
    spawnThreads(threads, thread_count, computeProduct);
    printf("Product of Matrices:\n");
    printMatrix(matProductResult);

    return 0;
}
