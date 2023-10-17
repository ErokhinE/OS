#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int agent_pid;

void sigterm_handler(int signum) {
    // Send SIGTERM to the agent
    kill(agent_pid, SIGTERM);
    exit(0);
}

int main() {
    // Check if agent process is running
    FILE* pid_file = fopen("/var/run/agent.pid", "r");
    if (pid_file == NULL) {
        printf("Error: No agent found.\n");
        exit(1);
    }
    
    fscanf(pid_file, "%d", &agent_pid);
    fclose(pid_file);
    
    printf("Agent found.\n");
    
    // Register signal handler for Ctrl+C (SIGINT)
    signal(SIGINT, sigterm_handler);
    
    char command[20];
    
    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        scanf("%s", command);
        
        if (strcmp(command, "exit") == 0) {
            // Send SIGUSR2 to the agent and terminate
            kill(agent_pid, SIGUSR2);
            exit(0);
        } else if (strcmp(command, "read") == 0) {
            // Send SIGUSR1 to the agent
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "stop") == 0) {
            // Send SIGSTOP to the agent
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            // Send SIGCONT to the agent
            kill(agent_pid, SIGCONT);
        } else {
            printf("Invalid command. Please try again.\n");
        }
    }
    
    return 0;
}
