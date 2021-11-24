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
     */
    int ret=signal_data->si_value.sival_int;
    printf("Value recieved in S1 from PID%d is: %u \n\n",signal_data->si_pid,ret);
}

int main(){
    pid_t pid;
    int status;
    pid=fork();
    if(pid<0){
        printf("Error while creating S1 process \n");
        return 0;
    }
    else if(pid==0){
        struct sigaction sAction;
        sAction.sa_flags=SA_SIGINFO;
        sAction.sa_sigaction=&handlerS1;
        sigemptyset(&sAction.sa_mask);
        sigaction(SIGTERM, &sAction, NULL);
        printf("PID of S1: %d\n",getpid());
        printf("PID of S1's parent(overall parent): %d\n\n",getppid());
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
            char *arg[]={"./E1",arg1,(char*)NULL};
            execv(arg[0],arg);
        }
        else if(SRpid>0){
            STpid=fork();
            if(STpid==0){
                char arg1[256];
                sprintf(arg1,"%d",(int)pid);
                char *arg[]={"./E2",arg1,(char*)NULL};
                execv(arg[0],arg);
            }
            else if(STpid>0){
                waitpid(STpid,&status,0);
            }
            else{printf("Errorin STFork\n");}
            waitpid(SRpid,&status,0);
        }
        else {printf("Errorin SRFork\n");}
            waitpid(pid,&status,0);
        }
    else {}
    exit(0);
    
}