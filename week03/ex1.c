#include <stdio.h>
#include <stdlib.h>

int const_tri(int*const p,int n){
    int temp;
    if(n==1){
        return 1;
    }
    if(n==2){
        return 1;
    }
    if(n==3){
        return 2;
    }
    for (int i=3;i<n;i++){
        temp = *p+*(p+1)+*(p+2);
        *p = *(p+1);
        *(p+1) = *(p+2);
        *(p+2) = temp;
    }
    return temp;
}

int main(){
    const int x=1;
    const int *q=&x;
    int *const p = (int*const) malloc (3*sizeof(int));
    *p=*q;
    printf("%p\n",p);
    *(p+1)=*q;
    printf("%p\n",(p+1));
    *(p+2)=2*(*q);
    printf("%p\n",(p+2));
    if ((&*(p+2)-&*(p+1)==1)&&(&*(p+1)-&*(p)==1)){
    printf("Continious cells\n");
    }
    int a = 5;
    int res1 = const_tri(p,a);
    printf("%d\n",res1);
    free(p);
    

    
    return 0;
}

