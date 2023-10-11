#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_MESSAGE_SIZE 1024

int main(int argc, char* argv[]) {
    
    int subscriber_index = atoi(argv[1]);

    char pipe_path[20];
    
    sprintf(pipe_path, "/tmp/ex1/s%d", subscriber_index);
    printf("%s\n",pipe_path);
    int fd; 
    

    char message[MAX_MESSAGE_SIZE];
    
    while (1){
    fd = open(pipe_path, O_RDONLY);
    if ((read(fd, message,MAX_MESSAGE_SIZE)) > 0) {
        printf("Message from subscriber: %s\n", message);
        close(fd);
    }
     
    }
    
   

    exit(EXIT_SUCCESS);
}
