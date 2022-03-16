#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int sig1R;
int sig2R;

void manager1(int sig){
        sig1R++;
    if(sig1R == 1 && sig2R == 1){
        printf("Success\n");
    } else if (sig1R == 2 || sig2R == 2){
        printf("Error\n");
    } else if (sig1R == 3 || sig2R == 3){
        abort();
    }
    return;
}
void manager2(int sig){
        sig2R++;
    if(sig1R == 1 && sig2R == 1){
        printf("Success\n");
    } else if (sig1R == 2 || sig2R == 2){
        printf("Error\n");
    } else if (sig1R == 3 || sig2R == 3){
        abort();
    }
    return;
}

int main(){
    signal(SIGUSR1, manager1);
    signal(SIGUSR2, manager2);
    printf("Pid = %d\n", getpid());
    
    return 0;
}