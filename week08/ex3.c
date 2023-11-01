#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>

#define MEMORY_TO_ALLOCATE 10 * 1024 * 1024

int main() {
    struct rusage usage;
    void *ptr;

    for (int i = 0; i < 10; i++) {
        ptr = malloc(MEMORY_TO_ALLOCATE);
        if (ptr == NULL) {
            fprintf(stderr, "Failed to allocate memory\n");
            return 1;
        }

        memset(ptr, 0, MEMORY_TO_ALLOCATE);

        getrusage(RUSAGE_SELF, &usage);
        printf("Memory usage (MB): %ld\n", usage.ru_maxrss / 1024);

        sleep(1);

        free(ptr);
       
    }

    return 0;
}

