#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
void exitfunc(){
    printf("called cleanup function\n");
    return;
}

int main(){
    int mypid = getpid();
    int myppid = getppid();
    printf("starting=%d %d\n",mypid,myppid);
    int p_id = fork();
    if(p_id<0){
        printf("process creation is unsuccessfull\n");
        return 1;
    }
    if(p_id==0){
        printf("child process is created\n");
        printf("child id=%d\n",p_id);
        printf("child=%d %d\n",getpid(),getppid());
    }else{
        printf("I am the parent process\n");
        printf("parent id=%d\n",p_id);
        printf("parent=%d %d\n",getpid(),getppid());
        // atexit(exitfunc);
        // exit(p_id);
    }
    return 0;
}