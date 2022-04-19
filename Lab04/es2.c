#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

#define BUFSIZE 2048

typedef struct{
    char file_name[BUFSIZE];
    int dim;
    int *array;
} arg_t;

void *order(void *arg_f){
    arg_t *arg;
    arg = (arg_t*) arg_f;
    int *vet, dim;
    char file[BUFSIZE];
    int fdR, nR, tmp;
    char buf[BUFSIZE];
    strcpy(file, arg->file_name);

    fdR = open(file, O_RDONLY);
    nR = read(fdR, &dim, sizeof(int));
    
    printf("%s - %d\n", file, dim);
    vet = (int *) malloc(dim * sizeof(int));
    
    for(int i = 0; i < dim; i++){
        read(fdR, &vet[i], sizeof(int));
    }

    for(int i = 0; i < dim; i++){
        printf("%d ", vet[i]);
    }
    printf("\n");

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            if(vet[j] > vet[i]){
                tmp = vet[i];
                vet[i] = vet[j];
                vet[j] = tmp;
            }
        }
    }
    printf("Order: ");
    for(int i = 0; i < dim; i++){
        printf("%d ", vet[i]);
    }
    printf("\n");

    arg->array = vet;
    arg->dim = dim;

}

void merge_file(int arr1[], int arr2[], int len1, int len2, int arr3[]){

    int i = 0, k = 0, j = 0;
    while(i < len1 && j < len2){
        if(arr1[i] < arr2[j])
            arr3[k++] = arr1[i++];
        else
            arr3[k++] = arr2[j++];
    }
    while(i < len1)
        arr3[k++] = arr1[i++];

    while(j < len2)
        arr3[k++] = arr2[j++];

}

int main(int argc, char **argv){
    arg_t *arg;
    pthread_t *th;
    int i = 0;
    int *ordered_array, *ordered_array_final, dim = 0;
    void *status;
    int fdW;

    if(argc < 3){
        fprintf(stderr, "Please insert at least one file to read and one file to write\n");
    }
    printf("%d\n", argc);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[argc - 1]);
    fdW = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    th = (pthread_t *) malloc((argc - 1) * sizeof(pthread_t));
    arg = (arg_t *) malloc((argc - 1) * sizeof(arg_t));
    
    for(i = 1; i < argc - 1; i++){
        strcpy(arg[i-1].file_name, argv[i]);
        pthread_create(&th[i], NULL, order, &arg[i-1]);
    }

    for(i = 0; i < argc - 2; i++){
        pthread_join(th[i], &status);
    }

    for(i = 0; i < argc - 2; i++){
        dim += arg[i].dim;
        printf("Dim = %d\n", arg[i].dim);
    }

    printf("Tot dim = %d\n", dim);

    ordered_array = (int *) malloc(dim * sizeof(int));
    ordered_array_final = (int *) malloc(dim * sizeof(int));

    merge_file(arg[0].array, arg[1].array,
                    arg[0].dim, arg[1].dim, ordered_array);

    for(i = 2; i < argc - 2; i++){
        merge_file(ordered_array, arg[i].array,
                    dim, arg[i].dim, ordered_array_final);
    }
    
    printf("Order : %d ", dim);
    for(int i = 0; i < dim; i++){
        printf("%d ", ordered_array_final[i]);
    }
    printf("\n");


    write(fdW, &dim, sizeof(int));
    write(fdW, ordered_array_final, dim * sizeof(int));
    

    close(fdW);
    free(ordered_array);
    free(ordered_array_final);
    free(th);
    free(arg);

    return 0;
}