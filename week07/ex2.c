#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    // Step 1: Create an empty file text.txt
    FILE *file = fopen("text.txt", "w");
    fclose(file);
    int dev_random = open("/dev/random", O_RDONLY);
    int text_file = open("text.txt", O_WRONLY);
    int printable_chars = 0;
    char c;
    while (1) {
        ssize_t bytes_read = read(dev_random, &c, 1);
        if (isprint(c)) {
            write(text_file, &c, 1);
            printable_chars++;
            if (printable_chars % 1024 == 0) {
                write(text_file, "\n", 1);
            }
        }       
        if (printable_chars >= 500 * 1024 * 1024) {
            break;
        }
    }
    close(text_file);
    close(dev_random);

    
    long page_size = sysconf(_SC_PAGESIZE);
    long chunk_size = 1024 * page_size;
    int fd = open("text.txt", O_RDWR);
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *file_data = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        
    int capital_count = 0;
    for (off_t i = 0; i < file_size; i++) {
        if (isupper(file_data[i])) {
            capital_count++;
            file_data[i] = tolower(file_data[i]);
        }
    }

    printf("Total capital letters in the file: %d\n", capital_count);    
    munmap(file_data, file_size);
    close(fd);

    return 0;
}
