#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>

#define N 600
#define BUFSIZE 2048

void cd_function(char *dir1, char *dir2){
    struct stat buf1, buf2;
    DIR *dp1, *dp2;
    char fullName[N], fullName2[N], fullName3[N], fullName4[40], buf[BUFSIZE];
    struct dirent *dirp1, *dirp2;
    int i, nR, nW, fdR, fdW;

    dp1 = opendir(dir1);
    while((dirp1 = readdir(dp1)) != NULL){
        
        if(strcmp(dirp1->d_name, "..") == 0 || strcmp(dirp1->d_name, ".") == 0)
            continue;
       

        sprintf(fullName, "%s/%s", dir1, dirp1->d_name);
        printf("file sorgente %s\n", fullName);

        stat(fullName, &buf1);
       
        if(S_ISDIR(buf1.st_mode) == 0){
            
            fdR = open(fullName, O_RDONLY); 
           
            sprintf(fullName2, "%s/%s", dir2, dirp1->d_name);
           
            printf("file destinazione %s\n", fullName2);

            fdW = open(fullName2, O_WRONLY | O_CREAT | O_TRUNC, 0666);
           
            while((nR = read(fdR, buf, BUFSIZE)) > 0){
                printf("%s\n", buf);
                printf("%d\n", nR);
                nW = write(fdW, buf, nR);
                printf("%d\n", nW);
            }
        } else {
            sprintf(fullName3, "%s/%s", dir2, dirp1->d_name);
            sprintf(fullName4, "%s/%s", dir1, dirp1->d_name);
            //printf("%s\n", fullName3);
            //printf("%s\n", fullName4);
            mkdir(fullName3, S_IRWXU);
            cd_function(fullName4, fullName3);
        }
        
    }
    
}

int main(int argc, char **argv){
    DIR *dp;
    struct dirent *dirp;
    if(argc != 3)
        printf("Error\n");
    cd_function(argv[1], argv[2]);

}