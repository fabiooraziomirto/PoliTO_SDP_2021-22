#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
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
    fcntl (file[1], F_SETFL, O_NONBLOCK);
    fcntl (file[0], F_SETFL, O_NONBLOCK);
    if(pipe(file) == 0){
        pid = fork();
        if(pid == 0){
            // Child reads
            close(file[0]);
            
            for(int i = 0; i < STR_NUM; i++){
                wait(&WAIT_TIME_1);
                for(j = 0; j < rand()%STR_SIZE+1; j++){
                    randomString[j] = string[rand()%26];
                    n += write(file[1], &randomString[j], 1);
                }
                randomString[j+1] = '\0';
                n += write(file[1], &randomString[j+1], 1);
                printf("Wrote %d bytes: %s Pid = %d\n", n, randomString, getpid());
                n = 0;
            }
            
        } else if (fork() == 0){
            close(file[0]);
            for(int i = 0; i < STR_NUM; i++){
                wait(&WAIT_TIME_2);
                for(j = 0; j < rand()%STR_SIZE+1; j++){
                    randomString[j] = string[rand()%26];
                    n += write(file[1], &randomString[j], 1);
                }
                randomString[j+1] = '\0';
                n += write(file[1], &randomString[j+1], 1);
                printf("Wrote %d bytes: %s Pid = %d\n", n, randomString, getpid());
                n = 0;
            }
              
        }else {
            // Parent writes
            close(file[1]);
            for(i = 0; i < 2*STR_NUM; i++){
                for(j = 0; j < STR_SIZE + 1; j++){
                    n += read(file[0], &randomStringRead[j], 1); 
                    if(randomStringRead[j] == '\0')
                        break;
                    else 
                        randomStringRead[j] -= ' ';
                }
                printf("Read %d bytes: %s\n", n, randomStringRead);
                n = 0;
            }
        }
    }
    return 0;
}