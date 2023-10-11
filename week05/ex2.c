#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



struct Thread {
    pthread_t id;
    int i;
    char message[256];
};

void* threadFunction(void* arg) {
    struct Thread* thread = (struct Thread*)arg;
    printf("%s ", thread->message);
    printf("%ld\n",thread->id);
    pthread_exit(NULL);
}

int main() {
    
    int n;
    scanf("%d",&n);
    struct Thread threads[n];

    for (int i = 0; i < n; i++) {
        threads[i].i = i + 1;
        sprintf(threads[i].message, "Hello from thread %d", threads[i].i);

        printf("Thread %d is created\n", threads[i].i);
        pthread_create(&threads[i].id, NULL, threadFunction, (void*)&threads[i]);
        pthread_join(threads[i].id, NULL);
    }

    return 0;
}
