#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"



int main(int argc, char *argv[]){
printf("Parent process %d\n",getpid());

int n=atoi(argv[1]);
for (int i=0;i<n;i++){
pid_t pid_new = fork();
if (pid_new == 0){
printf("Child process %d\n",getpid());
sleep(5);
}
} 

for (int i = 0;i<n;i++){
wait(NULL);
}

return 0;
}

