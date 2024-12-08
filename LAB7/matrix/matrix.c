#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 


typedef struct{
    int thread_id;
    int start;
    int end;

} ThreadArgs;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*) args;
    int start = thread_args->start;
    int end = thread_args->end;
    for (int i = 0; i < MAX; i++){
        for (int j = 0; j < MAX; j++){
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    free(args);
    return NULL;
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { 
    ThreadArgs* thread_args = (ThreadArgs*) args;
    int start = thread_args->start;
    int end = thread_args->end;
    for (int i = 0; i < MAX; i++){
        for (int j = 0; j < MAX; j++){
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    free(args);
    return NULL;
}// pass in the number of the ith thread

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    ThreadArgs* thread_args = (ThreadArgs*) args;
    int start = thread_args->start;
    int end = thread_args->end;

    for (int i = start; i < end ; i++){
        for (int j = 0 ; j < MAX; j++){
            matProductResult[i][j] = 0;
            for(int k = 0; k < MAX; k++){
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    free(args);
    return NULL;

}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
    
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t threads[10];
    int thread_count = 0;
    int rows_per_thread = MAX / thread_count;
    
    // 4. Create a thread for each cell of each matrix operation.
    // 
    // You'll need to pass in the coordinates of the cell you want the thread
    // to compute.
    // 
    // One way to do this is to malloc memory for the thread number i, populate the coordinates
    // into that space, and pass that address to the thread. The thread will use that number to calcuate 
    // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    for (int i = 0; i < thread_count;i++){
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->thread_id = i;
        args->start = i * rows_per_thread;
        args->end = (i == thread_count - 1) ? MAX : (i + 1) * rows_per_thread;  
        pthread_create(&threads[i], NULL, computeSum, (void*) args);
    }
    // 5. Wait for all threads to finish.
    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < thread_count; i++){
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->thread_id = i;
        args->start = i * rows_per_thread;
        args->end = (i == thread_count - 1) ? MAX : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, computeDiff, (void*) args);
    }

    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < thread_count; i++){
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->thread_id = i;
        args->start = i * rows_per_thread;
        args->end = (i == thread_count - 1) ? MAX : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, computeProduct, (void*) args);
    }

    for (int i = 0; i < thread_count; i++){
        pthread_join(threads[i], NULL);
    }
    
    
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    return 0;
  
}