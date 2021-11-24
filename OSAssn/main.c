#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>

#include <signal.h>

void handlerS1(int signo,siginfo_t *signal_data,void *extra){
    /* struct sigaction sAction;
    sAction.sa_flags=SA_SIGINFO;
    sAction.sa_sigaction=&handlerS1;
    sigaction(SIGTERM, &sAction, NULL);
     */int ret=signal_data->si_value.sival_int;
    printf("S1Random no. is: %u \n\n",ret);
}

int main(){
    pid_t pid;
    int status;
    pid=fork();
    if(pid<0){
        printf("Error while creating a child process \n");
        return 0;
    }
    else if(pid==0){
        struct sigaction sAction;
        sAction.sa_flags=SA_SIGINFO;
        sAction.sa_sigaction=&handlerS1;
        sigemptyset(&sAction.sa_mask);
        sigaction(SIGTERM, &sAction, NULL);
        printf("PID of S1: %d\n",getpid());
        printf("PID of S1's parent: %d\n",getppid());
        while(1){
            sleep(1);
        }
    }
    else if(pid>0){
        pid_t SRpid,STpid;
        SRpid=fork();
        if(SRpid==0){
            char arg1[256];
            sprintf(arg1,"%d",(int)pid);
            char *arg[]={"./SRprocess",arg1,(char*)NULL};
            execv(arg[0],arg);
        }
        else if(SRpid>0){
            waitpid(SRpid,&status,0);
        }
        else {printf("Errorin SRFork\n");}
            waitpid(pid,&status,0);
        }
    else {}
    exit(0);
    
}