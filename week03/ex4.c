#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

void* aggregate(void* base, size_t s, int n, void* v, void* 
(*opr)(const void*, const void*)) {
    void* result = v;
    for (int i = 0; i < n; i++) {
        result = opr(result, base + i * s);
    }
    return result;
}


void* add(const void* a, const void* b) {
    double *result = malloc(sizeof(double) * 10000000);
    *result = *(double*)a + *(double*)b;
    return (void*)result;
}

void* myltiply(const void* a, const void* b) {
    double *result = malloc(sizeof(double) * 10000000);
    *result = *(double*)a * *(double*)b;
    return (void*)result;
}

void* max_i(const void* a, const void* b) {
    int a1 = *(int*)a;
    int a2 = *(int*)b;
    int *result = malloc(sizeof(int) * 10000000);
    if (a1>a2){
        *result = a1;
    }
    else{
        *result = a2;
    }
    return (void*)result;
}

void* max_d(const void* a, const void* b) {
    double a1 = *(double*)a;
    double a2 = *(double*)b;
    double *result = malloc(sizeof(double) * 10000000);
    if (a1>a2){
        *result = a1;
    }
    else{
        *result = a2;
    }
    return (void*)result;
}

int main() {
    int arr1[] = {1000, 45, 0, 9, 67};
    double arr2[] = {12.0, 25.5, 33.5, 99.0, 1.0};

    void* res1 = aggregate(arr2, sizeof(double), 5, 
(void*)&(double){0}, add);
    printf("Add: %lf\n", 
*(double*)res1);

    void* res2 = aggregate(arr1, sizeof(int), 5, 
(void*)&(int){0}, add);
    printf("Add : %d\n", 
*(int*)res2);

    void* res3 = aggregate(arr2, sizeof(double), 
5, (void*)&(double){1}, myltiply);
    printf("Multiply: %lf\n", 
*(double*)res3);

    void* res4 = aggregate(arr1, sizeof(int), 5, 
(void*)&(int){1}, myltiply);
    printf("Multiply: %d\n", 
*(int*)res4);

    void* res5 = aggregate(arr2, sizeof(double), 5, 
(void*)&(double){-DBL_MAX}, max_d);
    printf("Max: %.2lf\n", 
*(double*)res5);

    void* res6 = aggregate(arr1, sizeof(int), 5, 
(void*)&(int){-2147483648}, max_i);
    printf("Max: %d\n", *(int*)res6);

    return 0;
}

