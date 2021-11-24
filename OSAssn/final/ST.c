#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>

#include <signal.h>
#include <assert.h>
#include <errno.h>


pid_t S1pid;  

void handlerST(int signo){
  unsigned high, low;
  __asm__ __volatile__ ("rdtsc" : "=a"(low), "=d"(high));
  unsigned long long b= ((unsigned long long)low)|( ((unsigned long long)high)<<32) ;
    unsigned long long freq=2.375e9;
    int sec;
	 sec=(b/freq);
   int hh = sec/3600;
    int mm = (sec - hh*3600)/60;
	int ss = sec - hh*3600 - mm*60;
   char buff[256];
   char time[512]="";
   char colon=':';
   
   sprintf(buff,"%d",hh);
   strncat(time,buff,sizeof(int));
   strncat(time,&colon,sizeof(char));
   sprintf(buff,"%d",mm);
   strncat(time,buff,sizeof(int));
   strncat(time,&colon,sizeof(char));
   sprintf(buff,"%d",ss);
   strncat(time,buff,sizeof(int));
   
   key_t key=ftok("shmfile",2021);
  int shmid=shmget(key,1024,0666|IPC_CREAT);
  char* shared_mem=(char*)shmat(shmid,NULL,0);
   strcpy(shared_mem,time);
   
   union sigval val;
   val.sival_ptr=time;
   if(sigqueue(getpid(),SIGTERM,val)==-1)
    perror("Error while queueing for ST from ST ");

   union sigval val1;
   val1.sival_int=-69;
   if(sigqueue(S1pid,SIGTERM,val1)==-1)
    perror("Error while queueing for S1 from ST ");

   shmdt(shared_mem);
   
}

void term_handlerST(int signo,siginfo_t *signal_data,void *extra){
  printf("Timestamp in ST handler: %s\n",signal_data->si_value.sival_ptr);

}


int main(int argc, char const *argv[]){
  
  S1pid=atoi(argv[1]);
  printf("Pid of ST is: %d\n",getpid());
  printf("Pid of S1 recieved in ST is: %d\n\n",S1pid);
  
  struct sigaction sAction;
  sAction.sa_flags=0;
  sAction.sa_handler=&handlerST;
  sigemptyset(&sAction.sa_mask);
  sigaction(SIGALRM, &sAction, NULL);

  struct sigaction sActionTerm;
  sActionTerm.sa_flags=SA_SIGINFO;
  sActionTerm.sa_sigaction=&term_handlerST;
  sigemptyset(&sActionTerm.sa_mask);
  sigaction(SIGTERM,&sActionTerm,NULL);

  struct itimerval sTimer;
  sTimer.it_value.tv_sec=1;
  sTimer.it_value.tv_usec=0;
  sTimer.it_interval.tv_sec=1;
  sTimer.it_interval.tv_usec=0;
  assert(setitimer(ITIMER_REAL,&sTimer,NULL)!=-1);    

  while(1){
        sleep(1);
    }
	 
}