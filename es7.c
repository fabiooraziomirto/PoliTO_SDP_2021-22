#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 7

typedef struct {
    int tid;
    char str[N];
} tS;

void *tF(void *par){
    tS *tD;
    int tid;
    char str[N];
    pthread_t tid1, tid2;
    void *status;
    int i = 0;
    int parInt = (int) par;
    for(i = 0; i < parInt; i++){
        pthread_create(&tid1, NULL, tF, (void*) i);
        pthread_create(&tid2, NULL, tF, (void*) i);
        pthread_join(tid1, &status);
        pthread_join(tid2, &status);
    } 
        printf("Tid = %ld\n", pthread_self());

    pthread_exit(NULL);
}

int main(int argc, char** argv){
    int i = 0, j = 0, n;
    if(argc != 2)
        printf("Errore\n");
    n = atoi(argv[1]);
    pthread_t *t = (pthread_t*) malloc(n*sizeof(pthread_t*));
    tS *v = (tS*) malloc(n*sizeof(tS*));
    tF((void*) n);
    return 0;
}