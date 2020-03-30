#include <iostream> 
#include <chrono> 
#include <random> 
#include <queue>
using namespace std; 

double max(double a,double b){
    return a>b?a:b;
}
  
double Exponential(double x)
{
	double z; 
    do 
    {
    	z = ((double) rand()/RAND_MAX);
    }
    while ((z==0) || (z==1));
    return((-x)*log(z));
}
// Driver Program 
int main() 
{ 
    int arrivalRate,departRate;
    int population;
    cin>>arrivalRate>>departRate>>population;
    double AT[population],TSB[population],TSE[population],waitQ[population],waitS[population],SI[population];
    double IAT[population];
    double ST[population];
    
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    poisson_distribution<int> distribution1 (3600/arrivalRate); 
  
    for (int i=0; i<population; ++i){
        double a = distribution1(generator1); 
        cout<<"arrival="<<a<<endl;
        IAT[i] = a;
    }

    // if(mu>lemda){
    //for checking
    // }
    //generating exponential random variable using departure rate

    for (int i = 0; i < population; ++i)
    {
        double number = Exponential(3600/departRate);
        ST[i] = number;
        cout<<"depart="<<number<<endl;
        chrono::duration<double> period (number);     
    } 

    //find out arrival time of each passenger
    int pre[3];
    AT[0] = 0;
    for(int i=1; i<population; i++){
        AT[i] = AT[i-1]+IAT[i];
    }
    vector<int> q[3]; 
    //every passenger selects a queue and enter into queue
    for(int j=0; j<population; j++){
        srand(time(NULL)+j);
        int qno = rand()%3;
        q[qno].push_back(j);
    }
    cout<<q[0].size()<<"c "<<q[1].size()<<"d "<<q[2].size()<<endl;
    for(int i=0; i<3; i++){
        if(q[i].size()==0){
            break;
        }
        int j=0;
        for(;j<q[i].size();j++){
            int cno = q[i][j];
            if(j==0){
                TSB[cno] = 0;
                TSE[cno] = ST[cno];
                waitQ[cno] = 0;
                waitS[cno] = ST[cno] + waitQ[cno];
                SI[cno] = 0;
                pre[i] = cno;
            }else{
                if(AT[cno]>waitS[pre[i]]){
                    SI[cno] = AT[cno] - waitS[pre[i]];
                }else{
                    SI[cno] = 0;
                }
                TSB[cno] = max(TSE[pre[i]],AT[cno]);
                TSE[cno] = ST[cno]+TSB[cno];
                waitQ[cno] = TSB[cno]-AT[cno];
                waitS[cno] = waitQ[cno]+ST[cno];
                pre[i] = cno;
            }
        }
    }
    double iat=0,at=0,st=0,tsb=0,tse=0,waitq=0,waits=0,si=0,resT=0;
    //cout<<"cno"<<" "<<"IAT"<<" "<<"AT"<<" "<<"ST"<<" "<<"TSB"<<" "<<"TSE"<<" "<<"WaitQ"<<" "<<"waitS"<<" "<<"SI"<<endl;
    for(int j=0; j<population; j++){
        iat+=IAT[j];at+=AT[j];st+=ST[j];tsb+=TSB[j];tse+=TSE[j];waitq+=waitQ[j];waits+=waitS[j];si+=SI[j],resT+=(TSE[j]-AT[j]);
        //cout<<j+1<<" "<<IAT[j]<<" "<<AT[j]<<" "<<ST[j]<<" "<<TSB[j]<<" "<<TSE[j]<<" "<<waitQ[j]<<" "<<waitS[j]<<" "<<SI[j]<<endl;
    }
    cout<<"average waiting time of queue="<<waitq/population<<endl;
    cout<<"average response time of System="<<resT/population<<endl;
    
  return 0; 
}