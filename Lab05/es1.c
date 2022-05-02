#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int generate_random_number(){
    int n;
    n = rand() % 10;
    return n;
}

int generate_random_letter(){
    int c;
    int random = rand() % 3;
    if(random == 0)
        c = 'a' + (rand() % 26);
    else if(random == 1)
        c = ' ';
    else if(random == 2)
        c = 10;
    return c;
}

int main(){
    int n;
    int shmid;
    int file[2];
    char *data;
    pid_t pid;
    int count;

    srand(time(NULL));
    
    
    key_t key;
    key = ftok("hello.txt", 3);
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);

    
    if(pipe(file) == 0){
            pid = fork();
            if(pid == 0){
                close(file[1]);
                if(data[0] == 0){
                    exit(EXIT_FAILURE);
                }
                read(file[0], data, strlen(data));
                sleep(1);
                printf("First process n = %d ", data[0]);
                for(int i = 2; i < data[0] + 2; i++){
                    if(data[i] == ' ' || data[i] == 10)
                        printf("%c ", data[i]);
                    else
                        printf("%c ", data[i] - ' ');
                }
                printf("\n");
            } else {
                close(file[0]);
                n = generate_random_number();
                data[0] = n;
                if(data[0] == 0){
                    exit(EXIT_FAILURE);
                }
                data[1] = ' ';

                for(int i = 2; i < data[0] + 2; i++){
                    data[i] = generate_random_letter();
                }
                write(file[1], data, strlen(data));
                sleep(1);
                printf("Second process n = %d ", data[0]);
                for(int i = 2; i < data[0] + 2; i++){
                    printf("%c ", data[i]);
                }
                printf("\n");
        }  
    }

    shmdt(data);

    return 0;
}