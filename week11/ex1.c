#include <stdio.h>
#include <dirent.h>

int main() {
    printf("%s\n","Star showing");
    DIR *root = opendir("/");

    
    struct dirent *entry;
    while ((entry = readdir(root)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(root);
    printf("%s\n","End showing");

    return 0;
}
