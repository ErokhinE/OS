#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


void parse(char *in, char *command, char *args[]) {
    char *temp = strtok(in, " \t\n");
    strcpy(command, temp);

    int i = 0;
    while (temp != NULL) {
        args[i++] = temp;
        temp = strtok(NULL, " \t\n");
    }

    args[i] = NULL;
}
void greet(){
printf("Enter commands: \n");
printf("If you want to finish write exit.\n");

}

int main() {
    char in[256];
    char command[16];
    char *args[16];
    greet();

    while (1) {
        
        fgets(in, 256, stdin);

        int commandLength = strlen(in);
        if (commandLength > 0 && in[commandLength - 1] == '\n') {
            in[commandLength - 1] = '\0';
        }

        if (strcmp(in, "exit") == 0) {
         
            break;
        }

        parse(in, command, args);
        pid_t pid1 = fork();

        if (pid1 == 0) {
            execvp(command, args);
        } else {
            int s;
            waitpid(pid1, &s, 0);
        }
    }

    return 0;
}

