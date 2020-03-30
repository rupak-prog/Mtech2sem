#include <iostream> 
#include <chrono> 
#include <random> 
#include <queue>
using namespace std; 

int max(int a,int b){
    return a>b?a:b;
}
  
// Driver Program 
int main() 
{ 
    vector<int> IAT{0,8,6,1,8,3,8,7,2,3};
    vector<int> ST{4,1,4,3,2,4,5,4,5,3};
    // unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    // default_random_engine generator1 (seed); 
  
    // poisson_distribution<int> distribution1 (7.1); 
  
    // cout << "Poisson-distribution(mean=5.0): ";
    // for (int i=0; i<30; ++i) 
    //     IAT.push_back(distribution1(generator1)); 

    // fflush(stdin);

    // int seed1 =  chrono::system_clock::now().time_since_epoch().count(); 
    // default_random_engine generator (seed1); 
  
    // exponential_distribution<double> distribution (4.1); 
     
    // for (int i=0; i<30; ++i)  
    // {      
    //     double number = distribution(generator);
    //     ST.push_back((int)(number*100)); 
    //     chrono::duration<double> period (number);  
    // } 

    // for(int i=0; i<30; i++){
    //     cout<<IAT[i]<<" "<<ST[i]<<endl;
    // }

    //find out arrival time of each passenger
    int AT[10],TSB[10],TSE[10],waitQ[10],waitS[10],SI[10];
    int pre[1];
    AT[0] = 0;
    for(int i=1; i<10; i++){
        AT[i] = AT[i-1]+IAT[i];
    }
    queue<int> q[3]; 
    //every passenger selects a queue and enter into queue
    for(int j=0; j<10; j++){
        //srand(time(NULL));
        //int qno = rand()%3;
        q[0].push(j);
    }
    for(int i=0; i<1; i++){
        if(q[i].size()==0){
            break;
        }
        for(int j=0; j<10; j++){
            int cno = q[0].front();
            if(j==0){
                TSB[cno] = 0;
                TSE[cno] = ST[cno];
                waitQ[cno] = 0;
                waitS[cno] = ST[cno]+waitQ[cno];
                SI[cno] = 0;
                pre[0] = cno;
            }else{
                if(AT[cno]>waitS[pre[i]]){
                    SI[cno] = AT[cno] - waitS[pre[i]];
                }else{
                    SI[cno] = 0;
                }
                TSB[cno] = max(TSE[pre[0]],AT[cno]);
                cout<<TSE[pre[0]]<<" "<<AT[cno]<<endl;
                TSE[cno] = ST[cno]+TSB[cno];
                cout<<ST[cno]<<" "<<TSB[cno]<<endl;
                waitQ[cno] = TSB[cno]-AT[cno];
                waitS[cno] = waitQ[cno]+ST[cno];
                cout<<waitQ[cno]<<" "<<ST[cno]<<endl;
                pre[0] = cno;
            }
            q[0].pop();
        }
    }
    cout<<"cno"<<" "<<"IAT"<<" "<<"AT"<<" "<<"ST"<<" "<<"TSB"<<" "<<"TSE"<<" "<<"WaitQ"<<" "<<"waitS"<<" "<<"SI"<<endl;
    for(int j=0; j<10; j++){
        cout<<j+1<<" "<<IAT[j]<<" "<<AT[j]<<" "<<ST[j]<<" "<<TSB[j]<<" "<<TSE[j]<<" "<<waitQ[j]<<" "<<waitS[j]<<" "<<SI[j]<<endl;
    }
  
  return 0; 
}