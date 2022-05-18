#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 7

typedef struct {
    int n;
    int max;
    pthread_t *t;
} tS;

void *tF(void *par){
    tS *tD, *tD1, *tD2;
    int i;
    pthread_t tid1, tid2;
    void *status;
    tD = (tS*) par;
    if(tD->n < tD->max){
        tD1 = (tS*) malloc(sizeof(tS));
        tD2 = (tS*) malloc(sizeof(tS));

        tD1->t = (pthread_t*) malloc((tD->max)*sizeof(pthread_t));
        tD2->t = (pthread_t*) malloc((tD->max)*sizeof(pthread_t));

        tD1->max = tD->max;
        tD2->max = tD->max;

        tD1->n = tD->n;
        for(i = 0; i < tD->n; i++)
            tD1->t[i] = tD->t[i];
        tD1->t[tD->n] = pthread_self();
        tD1->n++;

        tD2->n = tD->n;
        for(i = 0; i < tD->n; i++)
            tD2->t[i] = tD->t[i];
        tD2->t[tD->n] = pthread_self();
        tD2->n++;

        pthread_create(&tid1, NULL, tF, (void*) tD1);
        pthread_create(&tid2, NULL, tF, (void*) tD2);

        pthread_join(tid1, &status);
        free(tD1->t);
        free(tD1);

        pthread_join(tid2, &status);
        free(tD2->t);
        free(tD2);
    } else {
        for(i = 0; i < tD->max; i++)
            printf("Tid = %ld ", tD->t[i]);
        printf("\n");
    }
        

    pthread_exit(NULL);
}

int main(int argc, char** argv){
    int i = 0, j = 0, n;
    if(argc != 2)
        printf("Errore\n");
    n = atoi(argv[1]);
    
    tS *v = (tS*) malloc(n*sizeof(tS));
    v->max = n+1;
    v->n = 0;
    v->t = (pthread_t*) malloc(v->max*sizeof(pthread_t));
    tF((void*) v);

    free(v->t);
    free(v);
    
    return 0;
}