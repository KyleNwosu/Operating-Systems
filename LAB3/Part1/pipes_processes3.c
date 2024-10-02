#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    int pipe1[2], pipe2[2];
    pid_t child1, child2;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <argument>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if ((child1 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {
        close(pipe1[0]);
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);
        execlp("cat", "cat", "scores", NULL);
        perror("execlp cat");
        exit(EXIT_FAILURE);
    }

    if ((child2 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) {
        close(pipe1[1]);
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        close(pipe2[0]);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);

        execlp("grep", "grep", argv[1], NULL);
        perror("execlp grep");
        exit(EXIT_FAILURE);
    }

    close(pipe1[0]);
    close(pipe1[1]);

    if ((child1 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {
        close(pipe2[1]);
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);
        execlp("sort", "sort", NULL);
        perror("execlp sort");
        exit(EXIT_FAILURE);
    }

    return 0;
}