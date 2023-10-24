#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MEMORY_SIZE 10000000

unsigned int memory[MEMORY_SIZE] = {0};

void allocateFirstFit(unsigned int adrs, unsigned int size) {
    unsigned int start = 0;
    unsigned int count = 0;

    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (count == 0) {
                start = i;
            }
            count++;
        } else {
            count = 0;
        }

        if (count >= size) {
            for (unsigned int j = start; j < start + size; j++) {
                memory[j] = adrs;
            }
            return;
        }
    }
}

void allocateBestFit(unsigned int adrs, unsigned int size) {
    unsigned int bestFitStart = 0;
    unsigned int bestFitSize = MEMORY_SIZE + 1;

    unsigned int currentStart = 0;
    unsigned int currentSize = 0;

    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (currentSize == 0) {
                currentStart = i;
            }
            currentSize++;
        } else {
            if (currentSize >= size && currentSize < bestFitSize) {
                bestFitStart = currentStart;
                bestFitSize = currentSize;
            }
            currentSize = 0;
        }
    }

    if (currentSize >= size && currentSize < bestFitSize) {
        bestFitStart = currentStart;
        bestFitSize = currentSize;
    }

    if (bestFitSize <= MEMORY_SIZE) {
        for (unsigned int i = bestFitStart; i < bestFitStart + size; i++) {
            memory[i] = adrs;
        }
    }
}

void allocateWorstFit(unsigned int adrs, unsigned int size) {
    unsigned int worstFitStart = 0;
    unsigned int worstFitSize = 0;

    unsigned int currentStart = 0;
    unsigned int currentSize = 0;

    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            if (currentSize == 0) {
                currentStart = i;
            }
            currentSize++;
        } else {
            if (currentSize > worstFitSize) {
                worstFitStart = currentStart;
                worstFitSize = currentSize;
            }
            currentSize = 0;
        }
    }

    if (currentSize > worstFitSize) {
        worstFitStart = currentStart;
        worstFitSize = currentSize;
    }

    if (worstFitSize >= size) {
        for (unsigned int i = worstFitStart; i < worstFitStart + size; i++) {
            memory[i] = adrs;
        }
    }
}

void clear(unsigned int adrs) {
    for (unsigned int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}

int main() {
    FILE *file = fopen("queries.txt", "r");
    char line[100];
    double totalSeconds = 0;
    int queryCount = 0;
    clock_t startTime = clock();

    while (fgets(line, sizeof(line), file)) {
        
        if (strncmp(line, "allocate", 8) == 0) {
            unsigned int adrs, size;
            sscanf(line, "allocate %u %u", &adrs, &size);
            //allocateFirstFit(adrs, size); 
            allocateBestFit(adrs, size);
            //allocateWorstFit(adrs, size);
        } else if (strncmp(line, "clear", 5) == 0) {
            unsigned int adrs;
            sscanf(line, "clear %u", &adrs);
            clear(adrs);
        }
        queryCount++;
    }

    fclose(file);

    clock_t endTime = clock();
    //printf("%d\n",queryCount);
    totalSeconds += ((double)(endTime - startTime)) / CLOCKS_PER_SEC;
    //printf("%f\n",totalSeconds);
    double throughput = queryCount / totalSeconds;
    

    
    printf("Throughput: %.2f queries per second\n", throughput);

    return 0;
}
