#include <stdio.h>
#include <stdlib.h>
#include <string.h>
long long convert(long long num,int baseInit,int baseFinal){
    long long temp = num;
    for(int i=temp;i>0;i=i/10){
        int sign = i % 10;
        if(sign>=baseInit){
            printf("cannot convert!");
            return -1;
        }
    }
    
    
    long long dec = 0;
    long long base3 = 1;
    temp = num;
    for(int i=temp;i>0;i=i/10){
        int sign = i % 10;
        dec = dec +sign*base3;
        base3 = base3*baseInit;
    
    }
    if (dec<0){
         printf("cannot convert!");
            return -1;
    }
    if (baseInit<2||baseFinal<2||baseInit>10||baseFinal>10){
        printf("cannot convert!");
        return -1;
    }
    
    long long converted = 0;
    long long base4 = 1;
    for(int i=dec;i>0;i=i/baseFinal){
        int sign = i % baseFinal;
        converted = converted +sign*base4;
        base4 = base4*10;
    }
    return converted;
    
    
    
}

int main()
{
    long long num;
    int baseInit,baseFinal;
    scanf("%lld",&num);
    scanf("%d",&baseInit);
    scanf("%d",&baseFinal);
    long long converted = convert(num,baseInit,baseFinal);
    if(converted!=-1){
        printf("%lld",converted);
    }
    return 0;
}

