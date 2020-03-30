#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
int main(){
    int pid;
    int pids[3];
    int status;
    int numprocesses=0, total_process=3;
    while(numprocesses<total_process){
        pid = fork();

        //child process 
        if(pid==0){
            printf("In child process pid=%d\n",pid);
            sleep(5);
            return 4;
        }else{
            pids[numprocesses] = pid;
            numprocesses++;
            printf("In parent process: created process number: %d\n",pid);
        }
    }

    waitpid(pids[total_process-1],&status,0);
    if(WIFEXITED(status) != 0){
        printf("process %d exited normally\n",pids[total_process-1]);
        printf("Exit status from child is %d\n",WEXITSTATUS(status));
    }else{
        printf("process %d not exited normally\n",pids[total_process-1]);
    }
    return 0;
}