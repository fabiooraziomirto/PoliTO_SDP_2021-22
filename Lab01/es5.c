#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    int i = 0, j = 0, n;
    if(argc != 2)
        printf("Errore\n");
    n = atoi(argv[1]);
    int *vet = (int*) malloc(sizeof(int*) * n);

    pid_t pid;
    for(i = 0; i < n; i++){
        pid = fork();
        if(pid){
            vet[i] = 0;
            printf("%d", vet[i]);
        }else{
            vet[i] = 1;
            printf("%d", vet[i]);
        }
       

    }
    printf("\n");
    free(vet);
    return 0;
}