#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int tmp;

typedef struct{
    int A[2][2];
    int B[2][2];
    int C[2][2];
    int tmp;
} matrici;

int somma;

void *tF(void *par){
    int A[2][2];
    int B[2][2];
    int C[2][2];
    matrici *m;
    m =  (matrici *) par;
   for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
             A[i][j] = m->A[i][j];
           }
    } 
    for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
             B[i][j] = m->B[i][j];
        }
    } 

    for(int i = 0; i < 2; i++){
        somma += A[i][m->tmp]*B[m->tmp][i];
    }

    C[m->tmp][m->tmp] = somma;
    printf("%d\n", C[m->tmp][m->tmp]);

   pthread_exit(NULL);
}

int main(){
    int n = 2;
    int vetA[2][2] = {102, 99, 104, 85};
    int vetB[2][2] = {34, 25, 26, 96};
    matrici m[4];
    pthread_t tid[4];
    void *status;
    for(int k = 0; k < 4; k++){
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
             m[k].A[i][j] = vetA[i][j];
           }
       } 
    }

    for(int k = 0; k < 4; k++){
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
             m[k].B[i][j] = vetB[i][j];
           }
       } 
    }

    for(int k = 0; k < 4; k++){
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
             m[k].C[i][j] = 0;
           }
       } 
    }
    m[0].tmp = 0;
    m[1].tmp = 1;
    //for(int i = 0; i < 4; i++){
        pthread_create(&tid[0], NULL, tF, (void *) &m[0]);
        pthread_create(&tid[1], NULL, tF, (void *) &m[1]);
    //}
    //for(int i = 0; i < 4; i++){
        pthread_join(tid[0], &status);
        pthread_join(tid[1], &status);
    //}

    }

