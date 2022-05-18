#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int tmp;

void *tF(void *par){
    int i;
    int *vet = (int *) par;
    
    for(i = 0; i < 5; i++){
        if(vet[i] > vet[i+1]){
            tmp = vet[i];
            vet[i] = vet[i+1];
            vet[i+1] = tmp;
        }
    }

    for(i = 0; i < 5; i++)
        printf("%d ", vet[i]);
    printf("\n");
    pthread_exit(NULL);
}

int main(){
    int n = 5;
    int vet[5] = {102, 99, 34, 234, 25};
    pthread_t tid[5];
    void *status;
    for(int i = 0; i < n; i++){
        pthread_create(&tid[i], NULL, tF, (void *) &vet);
    }
    for(int i = 0; i < n; i++){
        pthread_join(tid[i], &status);
    }

}