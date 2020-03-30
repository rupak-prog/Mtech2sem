#include<bits/stdc++.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<cstdlib>
#include<sys/wait.h> 
#include<fcntl.h>
using namespace std;

vector<int> playerid;
int findNewValue(vector<pair<int,int>> snake,vector<pair<int,int>> ladder,int input,int curr,int endpoint){
    int currpos = input+curr+1, flag1=0, flag2=0;
    if(currpos == endpoint){
        return input;
    }else if(currpos > endpoint){
        return -1;
    }
    //find current position in the snake list
    while(1){
        for(int i=0; i<snake.size(); i++){
            if(currpos<snake[i].first){
                break;
            }else{
                if(snake[i].first == currpos){
                    //snake will bit and player position will go down
                    cout<<"snake found\n";
                    currpos = snake[i].second;
                    i=0;
                    flag1 = 1;
                }
            }
        }
        for(int j=0; j<ladder.size();j++){
            if(currpos<ladder[j].first){
                break;
            }else{
                if(ladder[j].first == currpos){
                    //ladder will take player to higher value
                    cout<<"ladder found\n";
                    currpos = ladder[j].second;
                    flag2 = 1;
                }
            }
        }
        if(flag1==0 && flag2==0){
            break;
        }
        flag1 = 0;
        flag2 = 0; 
    }
return currpos;
}


int main(int argc,char *argv[]){
    int ppid = getppid(), scount=0, lcount=0,f=1,pipeNo;
    int chance=0, p=0,flag=0;
    int* ptr = &chance;
    int n = atoi(argv[2]), endPoint = atoi(argv[1]);
    FILE *fp = fopen(argv[0],"r");
    vector<pair<int,int>> snake, ladder;
    vector<pair<int,int>> points;
    for(int k=0; k<n; k++){
        points.push_back(make_pair(k,0));
    }
    char temp[100];
    char ch;
    int total = 2*n;
    int fp1[total][2];

    //module1: store command line argument values to vector
    string line;
    ifstream myfile (argv[3]);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            string arr[3];
            int i = 0;
            stringstream ssin(line);
            while (ssin.good() && i < 3){
                ssin >> arr[i];
                ++i;
            }
            int start = stoi(arr[1]), end = stoi(arr[2]);
            if(arr[0] == "S"){
                snake.push_back(make_pair(start,end));
                scount++;
            }else if(arr[0] == "L"){
                ladder.push_back( make_pair(start,end) );
                lcount++;
            }else{
                cout<<"File contain invalid inputs";
                return 0;
            }
        }
        sort(snake.begin(),snake.end());
        sort(ladder.begin(),ladder.end());
        myfile.close();
    }

    //module2: create two unidirectional pipes for each process
    for(int i=0; i<n; i++){
            pipe(fp1[i]);
    }

    //module3: create number of players
    int pid = fork();
    if(pid==0){
        exit(0);
    }
    int count=0;
    int ppid1 = getppid();
    for(int i=0; i<=n-1; i++){
        if(ppid1 == ppid){
            //fork child process
            pid = fork();
            pipeNo = count;
            if(pid != 0){
                playerid.push_back(pid);
            }
            ppid1 = getppid();
            count++;
        }
    }
    //fcntl(fp1[p*2][0], F_SETFL, O_NONBLOCK)>0 && 
    p=0;
    while(1){
        if(ppid == ppid1){
            //select random process
            // if(fcntl(fp1[p*2][0], F_SETFL, O_NONBLOCK)>0 && f==0){
            //     close(fp1[2*p][0]);
            //     close(fp1[2*p+1][1]);
            //     srand(time(0));
            //     sprintf(line,"%d",1);
            //     write(fp1[2*p][1],line,10);
            //     f=1;
            // }else if(f==1){              
            int input;
            close(fp1[2*p][0]);
            //close(fp1[2*p+1][1]);
            read(fp1[2*p+1][0],&input,sizeof(int));  
            int temp = findNewValue(snake,ladder,input,points[p].second,atoi(argv[1]));
            if(temp == -1){
                cout<<"Player"<<p<<" won the game.....congratulation!!!!"<<endl;
                for(int i=0; i<n; i++){
                    kill(playerid[i],SIGSTOP);
                }
                wait(NULL);
                exit(0);
            }      
            points[p].second = temp;
            cout<<"player="<<p<<" position="<<points[p].second<<endl;
            if(input!=6){
                p = (p+1)%n;
            }
            //}
        }else{
            close(fp1[2*pipeNo][1]);
            //close(fp1[2*pipeNo+1][0]);
            srand(time(NULL)+getpid());
            int num = (rand()+getpid())%6+1;
            cout<<num;
            int* a = &num;
            write(fp1[2*pipeNo+1][1],a,sizeof(int));
        } 
    }
return 0;
}