#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "time.h"
#include "sys/wait.h"



int main(){
clock_t clock1 = clock();

pid_t pid1 = fork();


if (pid1 == 0){
clock_t clock2 = clock();
printf("First child %d, Parent %d, Time %lf ms\n", getpid(),getppid(),(double)(clock1-clock2)*1000/CLOCKS_PER_SEC);


exit(0);
}
else{
pid_t pid2 = fork();
clock1 = clock();

if (pid2==0){
clock_t clock2 = clock();

printf("Second child %d, Parent %d, Time %lf ms\n", getpid(),getppid(),(double)(clock2-clock1)*1000/CLOCKS_PER_SEC);
exit(0);

}
else{
int wait_temp;
waitpid(pid1, &wait_temp, 0);
waitpid(pid2, &wait_temp, 0);
clock_t clock2 = clock();
printf("Parent child %d, Parent %d, Time %lf ms\n", getpid(),getppid(),(double)(clock2-clock1)*1000/CLOCKS_PER_SEC);


}

}



return 0;
}

