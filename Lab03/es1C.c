#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <dirent.h>
#include <fcntl.h>

int main(){
    int n, i, j;
    int file[2];
    char cR;
    char cW = 'x';
    const int STR_NUM = 3;
    const int STR_SIZE = 3;
    int WAIT_TIME_1 = 1;
    int WAIT_TIME_2 = 2;
    char string[26] = "qwertyuiopasdfghjklzxcvbnm";
    char randomString[STR_SIZE];
    char randomStringRead[STR_SIZE];
    pid_t pid;
    struct timeval tvptr;
    tvptr.tv_sec = 1;
    tvptr.tv_usec = 0;
    fd_set rdR, rdW;
    FD_ZERO(&rdR);
    FD_ZERO(&rdW);
    FD_SET(file[0], &rdR);
    FD_SET(file[1], &rdW);
    //select(3, NULL, &rdW, NULL, &tvptr);
    if(pipe(file) == 0){
        pid = fork();
        if(pid == 0){
            // Child reads
            close(file[0]);
            
            for(int i = 0; i < STR_NUM; i++){
                select(1, NULL, &rdW, NULL, &tvptr);
                for(j = 0; j < rand()%STR_SIZE+1; j++){
                    randomString[j] = string[rand()%26];
                    if(FD_ISSET(file[1], &rdW)){
                        n += write(file[1], &randomString[j], 1);
                    }
                }
                randomString[j+1] = '\0';
                if(FD_ISSET(file[1], &rdW)){
                        n += write(file[1], &randomString[j+1], 1);
                }
                if(FD_ISSET(file[1], &rdW)){
                        printf("Wrote %d bytes: %s Pid = %d\n", n, randomString, getpid());
                }
                n = 0;
            }
            
        } else if (fork() == 0){
            close(file[0]);
            for(int i = 0; i < STR_NUM; i++){
                select(1, NULL, &rdW, NULL, &tvptr);
                for(j = 0; j < rand()%STR_SIZE+1; j++){
                    randomString[j] = string[rand()%26];
                    if(FD_ISSET(file[1], &rdW)){
                        n += write(file[1], &randomString[j], 1);
                    }
                }
                randomString[j+1] = '\0';
                if(FD_ISSET(file[1], &rdW)){
                        n += write(file[1], &randomString[j+1], 1);
                }
                if(FD_ISSET(file[1], &rdW)){
                        printf("Wrote %d bytes: %s Pid = %d\n", n, randomString, getpid());
                }
                n = 0;
            }
              
        }else {
            // Parent writes
            close(file[1]);
            for(i = 0; i < 2*STR_NUM; i++){
                select(1, &rdW, NULL, NULL, &tvptr);
                for(j = 0; j < STR_SIZE+1; j++){
                    if(FD_ISSET(file[0], &rdR)){
                        n += read(file[0], &randomStringRead[j], 1); 
                        if(randomStringRead[j] == '\0')
                            break;
                        else 
                            randomStringRead[j] -= ' ';
                    }
                }
                if(FD_ISSET(file[0], &rdR))
                    printf("Read %d bytes: %s\n", n, randomStringRead);
                n = 0;
            }
        }
    }
    return 0;
}