#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_MESSAGE_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process (subscriber)
        close(pipefd[1]);  // Close unused write end

        char message[MAX_MESSAGE_SIZE];
        int bytes_read = read(pipefd[0], message, sizeof(message));
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Subscriber received message: %s\n", message);

        close(pipefd[0]);  // Close read end
        exit(EXIT_SUCCESS);
    } else {  // Parent process (publisher)
        close(pipefd[0]);  // Close unused read end

        char message[MAX_MESSAGE_SIZE];
        printf("Enter a message: ");
        fgets(message, sizeof(message), stdin);

        ssize_t bytes_written = write(pipefd[1], message, strlen(message));
        if (bytes_written == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);  // Close write end

        wait(NULL);  // Wait for the child process to finish
        exit(EXIT_SUCCESS);
    }
}
