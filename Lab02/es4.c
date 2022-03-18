#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define BUFSIZE 255

void *manager(void *dir){
    struct dirent *dirp;
    DIR *dp;
    struct stat buf;
    pthread_t tid;
    void *status;
    char str[BUFSIZE], str1[BUFSIZE*3];
    
    strcpy(str, (char*) dir);
    
    dp = opendir(str);

    while((dirp = readdir(dp)) != NULL){

        if(strcmp(dirp->d_name, "..") == 0 || strcmp(dirp->d_name, ".") == 0)
            continue;
        
        sprintf(str1, "%s/%s", str, dirp->d_name);
        stat(str1, &buf);

        if(S_ISDIR(buf.st_mode) == 0){
            printf("%ld %s\n", pthread_self(), str1);
        } else {
            printf("%ld %s\n", pthread_self(), str1);
            pthread_create(&tid, NULL, manager, (void *) &str1);
            pthread_join(tid, &status);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    int i = 0, j = 0;
    pthread_t tid[argc - 1];
    void *status; 

    for(i = 0; i < (argc - 1); i++){
        pthread_create(&tid[i], NULL, manager, (void *) argv[i+1]);   
    }
    
     
    for(j = 0; j < (argc - 1); j++){
        pthread_join(tid[j], &status);
        j++;
    }

    return 0;

}