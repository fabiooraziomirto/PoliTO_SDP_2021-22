#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    int i = 0, j = 0, n, h;
    if(argc != 3)
        printf("Errore\n");
    n = atoi(argv[1]);
    h = atoi(argv[2]);

    while(i < n && fork()){
        while(j < h+1 && !fork()){
            j++;
        }
       i++;
    }

    printf("Pid = %d\n", getpid());
    return 0;
}