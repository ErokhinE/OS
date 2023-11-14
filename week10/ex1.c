#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

void find_all_hlinks(const char* source, const char* path) {
    struct stat st;
    if (lstat(source, &st) == -1) {
        perror("lstat");
        return;
    }

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char entry_path[PATH_MAX];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);

        struct stat entry_st;
        if (lstat(entry_path, &entry_st) == -1) {
            perror("lstat");
            continue;
        }

        if (entry_st.st_ino == st.st_ino) {
            char real_path[PATH_MAX];
            if (realpath(entry_path, real_path) == NULL) {
                perror("realpath");
                continue;
            }
            printf("Inode: %lu, Path: %s\n", entry_st.st_ino, real_path);
        }
    }

    closedir(dir);
}

void unlink_all(const char* source, const char* path) {
    struct stat st;
    if (lstat(source, &st) == -1) {
        perror("lstat");
        return;
    }

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char entry_path[PATH_MAX];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);

        struct stat entry_st;
        if (lstat(entry_path, &entry_st) == -1) {
            perror("lstat");
            continue;
        }

        if (entry_st.st_ino == st.st_ino && strcmp(entry_path, source) != 0) {
            if (unlink(entry_path) == -1) {
                perror("unlink");
                continue;
            }
        }
    }

    closedir(dir);
    printf("Last hard link: %s\n", source);
}

void create_sym_link(const char* source, const char* link) {
    if (symlink(source, link) == -1) {
        perror("symlink");
        return;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }

    const char* path = argv[1];

    // Create file myfile1.txt with the text "Hello world."
    int fd = open("folder1/myfile1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    const char* content = "Hello world.\n";
    if (write(fd, content, strlen(content)) == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);

    // Create two hard links to myfile1.txt
    if (link("folder1/myfile1.txt", "folder1/myfile11.txt") == -1) {
        perror("link");
        return 1;
    }
    if (link("folder1/myfile1.txt", "folder1/myfile12.txt") == -1) {
        perror("link");
        return 1;
    }

    // Find all hard links to myfile1.txt and print their i-nodes, paths, and content
    find_all_hlinks("folder1/myfile1.txt", path);

    // Move the file myfile1.txt to another folder /tmp/myfile1.txt
    if (rename("folder1/myfile1.txt", "/tmp/myfile1.txt") == -1) {
        perror("rename");
        return 1;
    }

    // Modify the file myfile11.txt
    fd = open("folder1/myfile11.txt", O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    const char* modified_content = "Modified content\n";
    if (write(fd, modified_content, strlen(modified_content)) == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);

    // Create a symbolic link myfile13.txt to /tmp/myfile1.txt
    create_sym_link("/tmp/myfile1.txt", "folder1/myfile13.txt");

    // Modify the file /tmp/myfile1.txt
    fd = open("/tmp/myfile1.txt", O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    const char* modified_content2 = "Modified content 2\n";
    if (write(fd, modified_content2, strlen(modified_content2)) == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    close(fd);

    // Find all hard links to myfile1.txt and print their i-nodes, paths, and content
    find_all_hlinks("/tmp/myfile1.txt", path);

    // Unlink all hard links to myfile1.txt except the last one
    unlink_all("/tmp/myfile1.txt", path);

    // Find all hard links to myfile1.txt and print their i-nodes, paths, and content
    find_all_hlinks("/tmp/myfile1.txt", path);

    return 0;
}



