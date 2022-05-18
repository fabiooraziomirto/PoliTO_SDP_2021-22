#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>

#define COPYINCR (1024*1024*1024)
#define N 31

typedef struct{
    int id;
    long int regNum;
    char surname[N];
    char name[N];
    int mark;
    int end;
} s;

void *manager1(void *dir){
    pthread_t tid;
    s *str;
    int i = 0;
    
    str = (s*) dir;

    while(str[i].end == 0){
        str[i].regNum += 1;
        i++;
    }
        str[i].regNum += 1;

    pthread_exit(NULL);
}

void *manager2(void *dir){
    pthread_t tid;
    s *str;
    s nullStruct;
    int i = 0;

    str = (s*) dir;

    while(str[i].end == 0){
        str[i].mark -= 1;
        i++;
    }
        str[i].mark -= 1;

    pthread_exit(NULL);
}



int main(){

    int fdin;
    char *src, *srcC;
    size_t copysz;
    struct stat sbuf;
    s str[4];
    int i = 0, k = 0;
    off_t fsz = 0;
    pthread_t tid[2];
    void *status;

    fdin = open("in.txt", O_RDONLY);
    if(fdin < 0)
        printf("ERROR OPENING FILE\n"); 
    fstat(fdin, &sbuf);

    while(fsz < sbuf.st_size){
        if((sbuf.st_size - fsz) > COPYINCR)
            copysz = COPYINCR;
        else
            copysz = sbuf.st_size - fsz;
        if((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED)
            printf("ERROR\n");
        i++;
        fsz +=  copysz;
    }       

    int j = 0, h = 0;
    while(k != fsz + 1){
        if(src[h] == '\n' || k == fsz){
            sscanf(src, "%d %ld %s %s %d", &str[j].id, &str[j].regNum, str[j].surname, str[j].name, &str[j].mark);
            if(src[h] == '\n')
                str[j].end = 0;
            else 
                str[j].end = 1;
            j++;
            src += h;
            h = 0;
        }
        h++;
        k++;
    }

    pthread_create(&tid[0], NULL, manager1, (void *) str);  
    pthread_create(&tid[1], NULL, manager2, (void *) str);

    pthread_join(tid[0], &status);   
    pthread_join(tid[1], &status); 

    for(int j = 0; j < 4; j++){
        fprintf(stdout, "%d %ld %s %s %d\n", str[j].id, str[j].regNum, str[j].surname, str[j].name, str[j].mark);
    }

    munmap(src, copysz);
    close(fdin);
}