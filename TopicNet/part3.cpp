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
    int arrivalRate,departRate,population;
    cin>>arrivalRate>>departRate>>population;
    
    //departure rate must be greater than arrival rate
    if(departRate<(arrivalRate/3)){
        cout<<"Departure rate of the passenger must be greater than arrival rate\n";
        return 0;
    }

    double AT[population],TSB[population],TSE[population],waitQ[population],waitS[population],SI[population];
    double IAT[population],ST[population],respT[population];
    vector<int> q[3]; //size of queue is fixed which is 10
    vector<int> chair[3];
    int dropped[3] = {0},inspect[3] = {0}, pre[3] = {0}, currPointer[3] = {0}, passengerWQ[3] = {0};
    
    //code for generating piosson distribution
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    poisson_distribution<int> distribution1 (3600/arrivalRate); 
  
    for (int i=0; i<population; ++i){
        double a = distribution1(generator1); 
        IAT[i] = a;
    }

    //code for generating exponential random variable using departure rate
    for (int i=0; i<population; ++i)
    {
        double number = Exponential(3600/departRate); //exponential function is declared above the main
        ST[i] = number;
        chrono::duration<double> period (number);     
    } 

    //put all the passengers into the temporary queue, the passenger can select queue by his own choice
    for(int i=0; i<population; i++){
        srand(time(NULL)+i);
        int x = rand()%3;
        q[x].push_back(i);
    }

    //find arrival time of each passenger we have Inter arrival time
    AT[0] = 0;
    for(int i=0; i<population; i++){
        AT[i] = AT[i-1] + IAT[i]; 
    }
    
    //iterate for each queue and do calculation for one queue at a time
    for(int i=0; i<3; i++){
        if(q[i].size()==0){     //check if number of passenger in the queue equals 0
            break;
        }else{
            for(int j=0; j<q[i].size(); j++){
                int cno;                            //store customer number
                passengerWQ[i] += chair[i].size();      //sum how many passengers are waiting in queue
                if(chair[i].size()==0 && currPointer[i]<q[i].size()){
                    //send passenger directly for service if nobody ahead of him
                    cno = q[i][currPointer[i]];
                    currPointer[i]++;
                }else if(chair[i].size() != 0){      //if someone present in the queue then give service to that passenger
                    cno = chair[i][0];
                    vector<int>::iterator it;
                    it = chair[i].begin();
                    chair[i].erase(it);        //removes front passenger from the queue
                }else{
                    break;
                }
                
                //check if passenger is first to be serviced
                if(j==0){
                    TSB[cno] = 0; //time service begins
                    TSE[cno] = ST[cno]; //time service ends
                    waitQ[cno] = 0;    //waiting time in queue
                    waitS[cno] = ST[cno] + waitQ[cno]; //waiting time in system
                    respT[cno] = ST[cno]; //response time of passenger
                    SI[cno] = 0; //time for which system remains idle
                    pre[i] = cno; //keep track of previous customer for that queue
                }else{
                    if(AT[cno]>TSE[pre[i]]){//waitS
                        SI[cno] = AT[cno] - waitS[pre[i]];  //if arrival time is greater than TSE of previous process than System remain idle for that time
                    }else{
                        SI[cno] = 0;
                    }
                    TSB[cno] = max(TSE[pre[i]],AT[cno]); //service begin out of TSE and AT which is more
                    TSE[cno] = ST[cno]+TSB[cno];
                    waitQ[cno] = TSB[cno]-AT[cno];
                    waitS[cno] = waitQ[cno]+ST[cno];
                    respT[cno] = waitQ[cno]+ST[cno];
                    pre[i] = cno;
                    }                           
                //if sits are vacant put all the arriving to the chair
                while(currPointer[i]<q[i].size() && chair[i].size()<10){      //put customer on the chair whose arrival is less than current serviced passenger 
                    if(AT[q[i][currPointer[i]]] < TSE[cno]){
                        inspect[i]++;
                        chair[i].push_back(q[i][currPointer[i]]);
                        currPointer[i]++;
                    }else{
                        break;
                    }
                }

                //if sits are full and still passengers are arriving then remove them
                if(currPointer[i]<q[i].size() && chair[i].size()==10){
                    while(currPointer[i]<q[i].size() && AT[q[i][currPointer[i]]] < TSE[cno]){ //remove one by one when more than 10 passeger arrives at a time
                        currPointer[i]++;
                        dropped[i]++; //keep track of dropped customer
                    }
                }
            }
        }
    }
    double w=0,r=0,x=0,y=0;

    //calculating the averages
    for(int i=0; i<3; i++){
        x += passengerWQ[i]/q[i].size();
        y += (double)inspect[i]/q[i].size();
    }
    for(int i=0; i<population; i++){
        w+=waitQ[i];
        r+=respT[i];
    }
    
    cout<<"Average number of passenger waiting in queue="<<x<<endl;
    cout<<"average waiting time of queue="<<w/population<<endl;
    cout<<"average response time of System="<<r/population<<endl;
    cout<<"average number of passenger getting inspected"<<y<<endl;
  return 0; 
}