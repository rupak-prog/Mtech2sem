#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    int pipefd[2];
    int pid;
    char recv[32];

    pipe(pipefd);

    switch(pid=fork()){
        case -1:
            perror("unable to deliver baby");
            exit(1);
        case 0:
            close(pipefd[0]); //closing read pipe corrosponding to child
            FILE *out = fdopen(pipefd[1],"w");
            fprintf(out,"hello world\n");
            break;
        default:
            close(pipefd[1]); //closing read pipe for parent
            FILE *in = fdopen(pipefd[0],"r");
            fscanf(in,"%s",recv);
            printf("%s",recv);
            break;
    }
return 0;
}