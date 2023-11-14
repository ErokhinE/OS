#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <limits.h>
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))


void print_stat_info(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Stat info for %s:\n", path);
        printf("  Inode: %ld\n", st.st_ino);
        printf("  Number of hard links to the file: %ld\n", st.st_nlink);
    } 
    
}


void sigint_handler(int signum) {
    printf("\nTerminating the program...\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(1);
    }

    const char* directory = argv[1];

    
    int fd = inotify_init();
    //if (fd == -1) {
      //  perror("Error initializing inotify");
       // exit(1);
    //}

    
    int wd = inotify_add_watch(fd, directory, IN_ALL_EVENTS);
    //if (wd == -1) {
     //   perror("Error adding watch");
     //   exit(1);
    //}

    
    signal(SIGINT, sigint_handler);

    
    char buf[BUF_LEN];
    ssize_t num_bytes;
    struct inotify_event* event;

    printf("Monitoring directory: %s\n", directory);
    printf("Press Ctrl-C to terminate the program.\n\n");

    while (1) {
        num_bytes = read(fd, buf, BUF_LEN);
        if (num_bytes == -1) {
            perror("Error reading events");
            exit(1);
        }

        for (char* p = buf; p < buf + num_bytes; p += EVENT_SIZE + event->len) {
            event = (struct inotify_event*)p;

            // Print the type of change
            if (event->mask & IN_CREATE) {
                printf("File/directory created: %s\n", event->name);
            } else if (event->mask & IN_DELETE) {
                printf("File/directory deleted: %s\n", event->name);
            } else if (event->mask & IN_MODIFY) {
                printf("File modified: %s\n", event->name);
            } else if (event->mask & IN_ATTRIB) {
                printf("Metadata changed: %s\n", event->name);
            } else if (event->mask & IN_ACCESS) {
                printf("File accessed: %s\n", event->name);
            } else if (event->mask & IN_OPEN) {
                printf("File/directory opened: %s\n", event->name);
            }

            // Print the stat info of the changed item
            char path[PATH_MAX];
            snprintf(path, sizeof(path), "%s/%s", directory, event->name);
            print_stat_info(path);
        }
    }

    // Close the inotify file descriptor
    close(fd);

    return 0;
}



