#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

#define N 256
#define L 1024
#define BUFSIZE 2048

void cd_function(char *dir1, char *dir2){
    struct stat buf1;
    DIR *dp1;
    char sourceFile[L], destFile[L], dir2P[L], dir1P[L], buf[BUFSIZE], buf2[BUFSIZE];
    struct dirent *dirp1;
    int nR, nW, fdR, fdW;

    dp1 = opendir(dir1);
    while((dirp1 = readdir(dp1)) != NULL){
        
        if(strcmp(dirp1->d_name, "..") == 0 || strcmp(dirp1->d_name, ".") == 0)
            continue;
       

        sprintf(sourceFile, "%s/%s", dir1, dirp1->d_name);
        printf("Source File %s\n", sourceFile);

        stat(sourceFile, &buf1);
       
        if(S_ISDIR(buf1.st_mode) == 0){
            
            fdR = open(sourceFile, O_RDONLY); 
           
            sprintf(destFile, "%s/%s", dir2, dirp1->d_name);
           
            printf("file destinazione %s\n", destFile);

            fdW = open(destFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
           
            while((nR = read(fdR, buf, BUFSIZE)) > 0){
                printf("%s\n", buf);
                printf("%d\n", nR);
                sprintf(buf2, "%s\n%d\n", dirp1->d_name, nR);
                nW = write(fdW, buf2, strlen(buf2));
                nW = write(fdW, buf, nR);
                printf("%d\n", nW);
            }
        } else {
            sprintf(dir2P, "%s/%s", dir2, dirp1->d_name);
            sprintf(dir1P, "%s/%s", dir1, dirp1->d_name);
            mkdir(dir2P, S_IRWXU);
            cd_function(dir1P, dir2P);
        }
        
    }
    
}

int main(int argc, char **argv){
    
    if(argc != 3)
        printf("Error\n");
    cd_function(argv[1], argv[2]);

}