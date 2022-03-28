#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

int main(){
    int n;
    int file[2];
    char cR;
    char cW = 'x';
    pid_t pid;
    if(pipe(file) == 0){
        pid = fork();
        if(pid == 0){
            // Child reads
            close(file[1]);
            n = read(file[0], &cR, 1);
            printf("Read %d bytes: %s\n", n, cR);
        } else {
            // Parent writes
            close(file[0]);
            n = write(file[1], &cW, 1);
            printf("Wrote %d bytes: %c\n", n, cW);
        }
    }
    return 0;
}