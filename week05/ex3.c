#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }
    int i = 2;
    for (; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int primes_count(int a, int b) {
    int temp = 0;
    for (int i = a; i < b; i++) {
        if (is_prime(i)) {
            temp++;
        }
    }
    return temp;
}

typedef struct prime_request {
    int a, b;
} prime_request;

void* prime_counter(void* arg) {
    prime_request* req = (prime_request*)arg;
    int* count = malloc(sizeof(int));
    *count = primes_count(req->a, req->b);
    return (void*)count;
}

int main(int argc, char* argv[]) {
    
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];
    int primeCounts[m];

    int intervalSize = n / m;
    int remainder = n % m;

    for (int i = 0; i < m; i++) {
        requests[i].a = i * intervalSize;
        requests[i].b = (i + 1) * intervalSize;
        if (i == m - 1) {
            requests[i].b += remainder;
        }

        pthread_create(&threads[i], NULL, prime_counter, (void*)&requests[i]);
    }

    int totalPrimes = 0;
    for (int i = 0; i < m; i++) {
        int* count;
        pthread_join(threads[i], (void**)&count);
        totalPrimes += *count;
        free(count);
    }

    printf("Number of primes in the range [0, %d) = %d\n", n, totalPrimes);

    return 0;
}
