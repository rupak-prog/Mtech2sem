#include <iostream> 
#include <chrono> 
#include <random> 
#include<unistd.h>
#include <queue>
using namespace std; 

long double max(long double a,long double b){
    return a>b?a:b;
}

long double Exponential(long double x)
{
    long double z; 
    do 
    {
        z = ((long double) rand()/RAND_MAX);
    }
    while ((z==0) || (z==1));
    return((-x)*log(z));
}
  
void caseA(){
    long int arrivalRate,departRate, population,duration;
    cout<<"\nEnter the Arrival Rate:";
    cin>>arrivalRate;
    cout<<"\nEnter the Departure Rate:";
    cin>>departRate;
    cout<<"\nEnter the Duration:";
    cin>>duration;

    population = duration * arrivalRate;
    
    long double AT[population],TSB[population],TSE[population],waitQ[population],waitS[population],SI[population],IAT[population], ST[population];
    long int pre[3],countWQ[3]={0},x=0,z=0;
    long double iat=0,at=0,st=0,tsb=0,tse=0,waitq=0,waits=0,si=0,resT=0;
    

    //generating poisson distributed random variables
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    poisson_distribution<long int> distribution1 (3600/arrivalRate); 
  
    for (long int i=0; i<population; ++i){
        long double a = distribution1(generator1); 
        //cout<<"arrival="<<a<<endl;
        IAT[i] = a;
    }

    //departure rate must be greater than arrival rate
    if(departRate<(arrivalRate/3)){
        cout<<"Departure rate of the passenger must be greater than arrival rate\n";
        return;
    }

    //generating exponential random variable using departure rate
    for (long int i = 0; i < population; ++i)
    {
        long double number = Exponential(3600/departRate);
        ST[i] = number;
        //cout<<"depart="<<number<<endl;
        chrono::duration<long double> period (number);     
    } 

    //find out arrival time of each passenger
    AT[0] = 0;
    for(long int i=1; i<population; i++){
        AT[i] = AT[i-1]+IAT[i];
    }
    vector<long int> q[3];

    //every passenger selects a queue and enter long into queue
    for(long int j=0; j<population; j++){
        srand(time(NULL)+j);
        long int qno = rand()%3;
        q[qno].push_back(j);
    }

    //every server have separate queue so process one queue at a time
    long int counter[3] = {0};
    for(long int i=0; i<3; i++){
        if(q[i].size()==0){  //if queue is empty
            break;
        }
        long int j=0;
        for(;j<q[i].size();j++){ //process all the passengers inside particular queue
            long int cno = q[i][j];  //passenger number who get service
            if(j!=0 && TSE[pre[i]]>AT[i]){  //here passenger need to wait 
                counter[i]++;
            }
            if(j==0){  //for first passenger in the queue
                TSB[cno] = 0;
                TSE[cno] = ST[cno];
                waitQ[cno] = 0;
                waitS[cno] = ST[cno] + waitQ[cno];
                SI[cno] = 0;
                pre[i] = cno;
            }else{
                //if arrival time is greater than time service ends of previous passenger then system is idle
                if(AT[cno]>TSE[pre[i]]){
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

            //find out number of passenger waiting in queue
            long int k = j;
            while(k<q[i].size() && TSE[cno]>AT[q[i][k]]){
                countWQ[i]++;
                k++;
            }
        }
    }
    
    for(long int i=0; i<3; i++){
        x += (counter[i]/q[i].size());
        z += (countWQ[i]/q[i].size());
    }

    //cout<<"cno"<<" "<<"IAT"<<" "<<"AT"<<" "<<"ST"<<" "<<"TSB"<<" "<<"TSE"<<" "<<"WaitQ"<<" "<<"waitS"<<" "<<"SI"<<endl;
    for(long int j=0; j<population; j++){
        iat+=IAT[j];at+=AT[j];st+=ST[j];tsb+=TSB[j];tse+=TSE[j];waitq+=waitQ[j];waits+=waitS[j];si+=SI[j],resT+=(TSE[j]-AT[j]);
        //cout<<j+1<<" "<<IAT[j]<<" "<<AT[j]<<" "<<ST[j]<<" "<<TSB[j]<<" "<<TSE[j]<<" "<<waitQ[j]<<" "<<waitS[j]<<" "<<SI[j]<<endl;
    }
    cout<<"Average number of passenger waiting in the queue="<<z<<endl;
    cout<<"Average waiting time of queue="<<waitq/population<<endl;
    cout<<"Average response time of System="<<resT/population<<endl;
    cout<<"Average number of passenger getting inspected"<<x<<endl;
    //average number of passenger waiting in queue find number of passenger whose arrival time is less than TSE of current customer**
}

long int findInspecNo(long double* s,long double arrT){
    long int a=0,b=0,c=0,flag=0,co=0;
    for(long int i=0; i<3; i++){
        if(s[0]==0){
            return 0;
        }else if(s[1]==0){
            return 1;
        }else if(s[2]==0){
            return 2;
        }
        if(s[i]<=arrT){
            if(i==0){a=1;}
            if(i==1){b=1;}
            if(i==2){c=1;}
            flag=1;
            co++;
        }
    }
    //we have to select among idle ones
    if(flag==0){
        return -1;
    }else{
        if(co==1){
            if(a==1){
                return 0;
            }else if(b==1){
                return 1;
            }else if(c==1){
                return 2;
            }
        }else if(co==2){
            if(a==1 && b==1){
                sleep(.01);
                srand(time(NULL));
                return (rand()%2);
            }else if(b==1 && c==1){
                sleep(0.02);
                srand(time(NULL));
                return (rand()%2+1);
            }else if(c==1 && a==1){
                sleep(0.03);
                srand(time(NULL));
                return rand()%2==0?0:2;
            }
        }else if(co==3){
            sleep(0.04);
            srand(time(NULL));
            return (rand()%3);
        }
    }
return -1;
}

//if all the server are busy then this function is called to select which server to select
long int findSmallest(long double* s){
    long double min = s[0];
    long int a,b,c,count=0;
    //find minimum s value
    for(long int j=1;j<3;j++){
        if(min>s[j]){
            min = s[j];
        }
    }

    //code for checking which is the server having minimum s value
    for(long int i=0; i<3; i++){
        if(min == s[i]){
            count++;
            if(i==0){
                a=1;
            }else if(i==1){
                b=1;
            }else{
                c=1;
            }
        }
    }
    if(count==1){
        if(a==1){
            return 0;
        }else if(b==1){
            return 1;
        }else if(c==1){
            return 2;
        }
    }else if(count==2){
        if(a==1 && b==1){
            srand(time(NULL));
            return (rand()%2);
        }else if(b==1 && c==1){
            srand(time(NULL));
            return (rand()%2+1);
        }else if(c==1 && a==1){
            srand(time(NULL));
            return rand()%2==0?0:2;
        }
    }else if(count==3){
        srand(time(NULL));
        return (rand()%3);
    }
}

void caseB(){
    long int arrivalRate,departRate,populationSize,duration;
    cout<<"\nEnter the Arrival Rate:";
    cin>>arrivalRate;
    cout<<"\nEnter the Departure Rate:";
    cin>>departRate;
    cout<<"\nEnter the Duration:";
    cin>>duration;

    populationSize = duration * arrivalRate;
    
    long double IAT[populationSize],ST[populationSize],que[populationSize],AT[populationSize],DT[populationSize],respT[populationSize],waitQ[populationSize];
    long int busy1=0,busy2=0,busy3=0,counter=0,countQ=0;
    long double s[3]={0};
    
    //departure rate must be greater than arrival rate
    if(departRate<arrivalRate){
        cout<<"Departure rate of the passenger must be greater than arrival rate\n";
        return;
    }

    //generating poisson distribution using arrival rate
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    poisson_distribution<long int> distribution1 (3600/arrivalRate); 
  
    for (long int i=0; i<populationSize; ++i){
        long double a = distribution1(generator1); 
        //cout<<"arrival="<<a<<endl;
        IAT[i] = a;
    }

    //generating exponential random variable using departure rate
    for (long int i = 0; i < populationSize; ++i)
    {
        long double number = Exponential(3600/departRate);
        ST[i] = number;
        //cout<<"depart="<<number<<endl;
        chrono::duration<long double> period (number);     
    }

    //inserting all the passenger long into a queue and calculating their arrival times from long inter arrival time
    for(long int i=1; i<populationSize; i++){
        que[i]= i;
        AT[i] = AT[i-1]+IAT[i];
    }

    //arrival time of first passenger must be 0
    AT[0] = 0;
    for(long int i=0; i<populationSize; i++){

        //passenger selects which Inspector he wants to go
        long int sno = findInspecNo(s,AT[i]);  

        //sno equals -1 means no inspector is free to give service
        if(sno==-1){
            //we select the inspector whose curr service value is minimum
            long int index = findSmallest(s),max=0;
            
            for(long int k=0; k<3; k++){
                if(max<s[k]){
                    max = s[k];
                }
            }
            long int j = i+1;
            while(max>AT[j]){
                countQ++;
                j++;
            }

            //here the passenger needs to wait in the queue
            waitQ[i] = s[index]- AT[i];
            counter++;   //for average number of passenger inspected
            s[index] += ST[i];  //update time of inspector when he gonna free
            respT[i] = waitQ[i]+ST[i];  //response time is summation of waiting in queue and Service time 
        }else{
            waitQ[i] =  0;
            respT[i] =  ST[i];
            s[sno] = AT[i] + ST[i];
        }
        //cout<<"arrival="<<AT[i]<<" "<<"ST="<<ST[i]<<" "<<"depart="<<respT[i]<<"waitQ="<<waitQ[i]<<endl;
    }

    //calculating the averages
    long double w=0,r=0;
    for(long int i=0; i<populationSize; i++){
        w+=waitQ[i];
        r+=respT[i];
    }
    cout<<"Average number of passenger waiting in queue"<<countQ/populationSize<<endl;
    cout<<"Average waiting time="<<w/populationSize<<endl;
    cout<<"Average number of passenger getting inspected="<<counter/populationSize<<endl;
    cout<<"Average response time="<<r/populationSize<<endl;
}

void caseC(){

    long int arrivalRate,departRate,population,duration;
    cout<<"\nEnter the Arrival Rate:";
    cin>>arrivalRate;
    cout<<"\nEnter the Departure Rate:";
    cin>>departRate;
    cout<<"\nEnter the Duration:";
    cin>>duration;
    population = duration * arrivalRate;
    
    //departure rate must be greater than arrival rate
    if(departRate<(arrivalRate/3)){
        cout<<"Departure rate of the passenger must be greater than arrival rate\n";
        return;
    }

    long double AT[population],TSB[population],TSE[population],waitQ[population],waitS[population],SI[population];
    long double IAT[population],ST[population],respT[population];
    vector<long int> q[3]; //size of queue is fixed which is 10
    vector<long int> chair[3];
    long int dropped[3] = {0},inspect[3] = {0}, pre[3] = {0}, currPoer[3] = {0}, passengerWQ[3] = {0};

    //code for generating piosson distribution
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    poisson_distribution<long int> distribution1 (3600/arrivalRate); 
  
    for (long int i=0; i<population; ++i){
        long double a = distribution1(generator1); 
        IAT[i] = a;
    }

    //code for generating exponential random variable using departure rate
    for (long int i=0; i<population; ++i)
    {
        long double number = Exponential(3600/departRate); //exponential function is declared above the main
        ST[i] = number;
        chrono::duration<long double> period (number);     
    } 

    //put all the passengers long into the temporary queue, the passenger can select queue by his own choice
    for(long int i=0; i<population; i++){
        srand(time(NULL)+i);
        long int x = rand()%3;
        q[x].push_back(i);
    }

    //find arrival time of each passenger we have long inter arrival time
    AT[0] = 0;
    for(long int i=0; i<population; i++){
        AT[i] = AT[i-1] + IAT[i]; 
    }
    
    //iterate for each queue and do calculation for one queue at a time
    for(long int i=0; i<3; i++){
        if(q[i].size()==0){     //check if number of passenger in the queue equals 0
            break;
        }else{
            for(long int j=0; j<q[i].size(); j++){
                long int cno;                            //store customer number
                passengerWQ[i] += chair[i].size();      //sum how many passengers are waiting in queue
                if(chair[i].size()==0 && currPoer[i]<q[i].size()){
                    //send passenger directly for service if nobody ahead of him
                    cno = q[i][currPoer[i]];
                    currPoer[i]++;
                }else if(chair[i].size() != 0){      //if someone present in the queue then give service to that passenger
                    cno = chair[i][0];
                    vector<long int>::iterator it;
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
                while(currPoer[i]<q[i].size() && chair[i].size()<10){      //put customer on the chair whose arrival is less than current serviced passenger 
                    if(AT[q[i][currPoer[i]]] < TSE[cno]){
                        inspect[i]++;
                        chair[i].push_back(q[i][currPoer[i]]);
                        currPoer[i]++;
                    }else{
                        break;
                    }
                }

                //if sits are full and still passengers are arriving then remove them
                if(currPoer[i]<q[i].size() && chair[i].size()==10){
                    while(currPoer[i]<q[i].size() && AT[q[i][currPoer[i]]] < TSE[cno]){ //remove one by one when more than 10 passeger arrives at a time
                        currPoer[i]++;
                        dropped[i]++; //keep track of dropped customer
                    }
                }
            }
        }
    }
    long double w=0,r=0,x=0,y=0;

    //calculating the averages
    for(long int i=0; i<3; i++){
        x += passengerWQ[i]/q[i].size();
        y += (long double)inspect[i]/q[i].size();
    }
    for(long int i=0; i<population; i++){
        w+=waitQ[i];
        r+=respT[i];
    }
    
    cout<<"Average number of passenger waiting in queue="<<x<<endl;
    cout<<"Average waiting time of queue="<<w/population<<endl;
    cout<<"Average response time of System="<<r/population<<endl;
    cout<<"Average number of passenger getting inspected"<<y<<endl;
}


// Driver Program 
int main() 
{ 
    long int casenumber;
    cout<<"1. CASE A \n 2. CASE B \n 3. CASE C\n";
    cout<<"Enter the case number:";
    cin>>casenumber;
    switch(casenumber){
        case 1: caseA();
            break;
        case 2: caseB();
            break;
        case 3: caseC();
            break;
        default:
            cout<<"Enter the correct case number";
    }
  return 0; 
}