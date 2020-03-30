#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int mypid, myppid;
    mypid = getpid();
    myppid = getppid();
    printf("mypid= %d\n",mypid);
    printf("mypid= %d\n",myppid);
    system("ps -ef");
return 0;
}