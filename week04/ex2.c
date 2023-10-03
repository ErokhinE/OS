#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "time.h"
#include "sys/wait.h"



int main(){
int u[120];
int v[120];
FILE *file = fopen("temp.txt","w+");
for (int i= 0 ;i<120;i++){
u[i] = rand()%100;
v[i] = rand()%100;


}
int n;
scanf("%d", &n);
if (n<=10&&n>=2&&n%2==0){
for (int i=0;i<n;i++){
pid_t pid_new = fork();
if (pid_new == 0){
int res = 0;
for (int j=120/n*i;j<(120/n)*(i+1);j++){
res +=v[j]*u[j];
fprintf(file, "%d\n", res);
fclose(file);
exit(0);
}
} 
}
for (int i = 0;i<n;i++){
wait(NULL);
}
rewind(file);
int res1 = 0;
char str[256];
while(fgets(str,sizeof(str),file)){
int res;
if (sscanf(str,"%d\n",&res) == 1){
res1 += res;
}
}
printf("%d",res1);
}

else{
printf("n is Incorrect");
}
return 0;
}

