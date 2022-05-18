


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
 
#define L 70
#define N 31
#define BUFFSIZE 200

typedef struct{
    int id; // 2
    long int regNum; // 4
    char surname[N]; // 31
    char name[N]; // 31 
    int mark; // 2
} s;

int main(int argc, char **argv){
    int nR, nW, fdR, i = 0, j, number;
    char letter;
    s str;
    fdR = open("file2.txt", O_RDWR);
    off_t offset;
    //fdW = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    
    if(fdR == (-1)){
        fprintf(stdout, "Error Opening a File.\n");
        exit(1);
    } else {
        fprintf(stdout, "File correctly opened\n");
    }
    while(1){
        fscanf(stdin, "%c%*c", &letter);
        if (letter == 'E'){
            fprintf(stdout, "Ending program...\n");
            
            exit(0);
        }
        fscanf(stdin, "%d%*c", &number);
        //fprintf(stdout, "%c %d", letter, number);
        if(letter == 'R'){
            lseek(fdR, (number-1)*sizeof(s), SEEK_SET);
            read(fdR, &str, sizeof(s));
            fprintf(stdout, "%d %ld %s %s %d\n", str.id, str.regNum, str.surname, str.name, str.mark);
        } else if (letter == 'W'){
            fscanf(stdin, "%d %ld %s %s %d\n", &str.id, &str.regNum, str.surname, str.name, &str.mark);
            lseek(fdR, (number-1)*sizeof(s), SEEK_SET);
            write(fdR, &str, sizeof(s));

        } else {
            printf("Error\n");
            exit(0);
        }


        
    }
    return 1;
}
