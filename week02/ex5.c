#include <stdio.h>

int tribonacci(int n){
    int zero = 0;
    int first = 0;
    int second = 1;
    int newElement;
    for (int i=3;i<=n;i++){
        newElement = zero+first+second;
        zero = first;
        first = second;
        second = newElement;
    }
    return newElement;
}

int main(){
    int a = 4;
    int b = 36;
    int res1 = tribonacci(a);
    int res2 = tribonacci(b);
    printf("%d\n",res1);
    printf("%d",res2);

    
    return 0;
}

