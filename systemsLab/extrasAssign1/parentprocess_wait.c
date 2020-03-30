#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
int main(){
    int pid, status;
    pid = fork();
    if(pid==0){
        system("ps -ef");
        sleep(10);
        system("ps -ef");
        return 3; //exit status is 3 from the child process
    }else{
        sleep(3);
        wait(&status);
        printf("In parent process: exit status from child is decimal %d %0x",status,status);
    }
    return 0;
}