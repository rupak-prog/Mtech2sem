#include<bits/stdc++.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<cstdlib>
#include<fcntl.h>
using namespace std;

int main(){
    int fd[2],i=0,input;
    char line[10];
    pipe(fd);
    int id = fork();
     while(1){
        if(id!=0){
            cout<<"parent"<<endl;
            close(fd[0]);
            sprintf(line,"%d",i++);
            write(fd[1],line,10);
            cout<<"write="<<i-1<<endl;
            //sleep(4);
            if(i==10){
                exit(0);
            }
        }else{
            close(fd[1]);
            cout<<"child"<<endl;
            read(fd[0], line, 10);
            sleep(2);
            sscanf(line,"%d",&input);
            cout<<"read="<<input<<endl;
            sleep(1);
            if(input==9){
                exit(0);
            }
        }
     }
return 0;
}