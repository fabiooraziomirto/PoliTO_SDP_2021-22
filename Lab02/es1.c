#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define N 31
#define BUFFSIZE 200 

typedef struct{
    int id;
    long int regNum;
    char surname[N];
    char name[N];
    int mark;
} s;

int main(int argc, char **argv){
    int nR, nW, fdW, i = 0, j;
    char buf[BUFFSIZE], st[100];
    FILE *fdR, *fdR2;
    s str;
    fdR = fopen(argv[1], "rt");
    fdW = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fdR2 = fopen(argv[3], "wt");

    if(fdR == NULL || fdW == (-1)){
        fprintf(stdout, "Error Opening a File.\n");
        exit(1);
    } else {
        fprintf(stdout, "File correctly opened\n");
    }


    while( fscanf(fdR, "%d %ld %s %s %d", &str.id, &str.regNum, str.surname, str.name, &str.mark) != EOF){
        i++;
        write(fdW, &str, sizeof(str));
    }
    

    close(fdW);
    fclose(fdR);

    fdW = open(argv[2], O_RDONLY);

    while(nR = read(fdW, &str, sizeof(str)) > 0){
        if(j == i-1)
            fprintf(fdR2, "%d %ld %s %s %d", str.id, str.regNum, str.surname, str.name, str.mark);
        else 
            fprintf(fdR2, "%d %ld %s %s %d\n", str.id, str.regNum, str.surname, str.name, str.mark);
        j++;
    }
   
    
    fclose(fdR2);
    return 1;
}


