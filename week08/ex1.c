 #include <stdio.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <string.h>
 #include <errno.h>
 #include <ctype.h>
 #include <stdlib.h>
 #define PASS_LEN 8

 int main() {
     pid_t pid = getpid();
     FILE *fp = fopen("/home/erzhe/week08/ex1.pid", "w");
     if (fp == NULL) {
         perror("Error opening file");
         return 1;
     }
     fprintf(fp, "%d", pid);
     fclose(fp);

     int randomData = open("/dev/urandom", O_RDONLY);
     if (randomData < 0) {
         perror("Unable to open /dev/urandom");
         return 1;
     }
     char *password = malloc(PASS_LEN);

     memcpy(password, "pass:", 5);
     int bytes_read = 0;
     while (bytes_read < PASS_LEN) {
         char temp;
         int result = read(randomData, &temp, sizeof(char));
         if (result < 0) {
             if (errno == EINTR)
                 continue;
             perror("Error reading from /dev/urandom");
             return 1;
         }
         if (temp >= 33 && temp <= 126) {
             password[bytes_read + 5] = temp;
             bytes_read++;
         }
     }
     printf("Generated password: %s\n", password);
     fflush(stdout);

     while (1) {
         sleep(1);
     }
     return 0;
 }
