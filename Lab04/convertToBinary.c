#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 2048

int main(int argc, char **argv){

    if(argc != 3){
        fprintf(stderr, "Please insert 3 argument\n");
    }
    FILE *fdR;
    int fdW;
    int nR, nW;
    int *array, n;
    char buf[BUFSIZE];
    fdR = fopen(argv[1], "r");
    fdW = open(argv[2], O_WRONLY);
    fscanf(fdR, "%d", &n);

    array = (int *) malloc((n+1)*sizeof(int));
    array[0] = n;

    for(int i = 1; i < (n + 1); i++){
        fscanf(fdR, "%d", &array[i]);
    }

    nW = write(fdW, array, (n+1)*sizeof(int));

    fclose(fdR);
    close(fdW);

    return 0;
}