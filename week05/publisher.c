#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_MESSAGE_SIZE 1024

int main(int argc, char* argv[]) {
    

    int num_subscribers = atoi(argv[1]);
    


    char pipe_path[20];
    mkdir("/tmp/ex1", 0777);
    

    pid_t pid;
    
    while(1){
    char message[MAX_MESSAGE_SIZE];
    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);
    for (int i = 1; i <= num_subscribers; i++) {
        pid = fork();
        

        if (pid == 0) {  // Child process (subscriber)
            char subscriber_pipe_path[20];
            printf("%d\n",i);
            sprintf(subscriber_pipe_path, "/tmp/ex1/s%d", i);
            printf("%s\n",subscriber_pipe_path);
            mkfifo(subscriber_pipe_path, 0666);


            int fd = open(subscriber_pipe_path, O_WRONLY);
            write(fd,message,strlen(message)+1);

           
            close(fd);
            exit(0);

            
        }
    }

    

    while(wait(NULL)>0);
         
        
    
    strcpy(message,"");
    }

    

    exit(EXIT_SUCCESS);
}
