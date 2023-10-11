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


int k = 0;
int c = 0;
int n = 0;
pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int get_number_to_check() {
    int ret = k;
    
    if (k != n) {
        k++;
    }
    
    return ret;
}

void increment_primes() {
    
    c++;
    
}

void* check_primes(void* arg) {
    while (1) {
    	pthread_mutex_lock(&global_lock);
        int num = get_number_to_check();
        pthread_mutex_unlock(&global_lock);
        if (num >= n) {
            break;
        }
        if (is_prime(num)) {
            pthread_mutex_lock(&global_lock);
            increment_primes();
            pthread_mutex_unlock(&global_lock);
        }
    }
    //pthread_exit(NULL);
    return NULL;
}

int main(int argc, char* argv[]) {

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];

    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&global_lock);

    printf("Number of primes in the range [0, %d) = %d\n", n, c);
    return 0;
}
