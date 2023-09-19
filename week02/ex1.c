#include <stdio.h>
#include <limits.h>
#include <float.h>

int main()
{
    int a = INT_MAX;
    unsigned short int b=USHRT_MAX;
    long int c=LONG_MAX;
    float d=FLT_MAX;
    double e=DBL_MAX;
    printf("%d\n",a);
    printf("%lu\n",sizeof a);
    printf("%u\n",b);
    printf("%lu\n",sizeof b);
    printf("%ld\n",c);
    printf("%lu\n",sizeof c);
    printf("%f\n",d);
    printf("%lu\n",sizeof d);
    printf("%lf\n",e);
    printf("%lu\n",sizeof e);
    

    return 0;
}

