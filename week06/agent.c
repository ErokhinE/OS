
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigusr1_handler(int signum) {
    // Read and print the contents of text.txt
    FILE* file = fopen("text.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    
    fclose(file);
}

void sigusr2_handler(int signum) {
    printf("Process terminating...\n");
    exit(0);
}

int main() {
    // Create and write PID to /var/run/agent.pid
    FILE* pid_file = fopen("/var/run/agent.pid", "w");
    if (pid_file == NULL) {
        perror("Error creating PID file");
        exit(1);
    }
    
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);
    
    // Register signal handlers
    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);
    
    // Read and print the contents of text.txt
    FILE* file = fopen("text.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    
    fclose(file);
    
    // Sleep indefinitely
    while (1) {
      
    }
    
    return 0;
}

