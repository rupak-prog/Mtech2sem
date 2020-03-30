#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

//passenger selects which inspector he wants to go 
int findInspecNo(double* s,double arrT){
    int a=0,b=0,c=0,flag=0,co=0;
    for(int i=0; i<3; i++){
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
int findSmallest(double* s){
    double min = s[0];
    int a,b,c,count=0;
    //find minimum s value
    for(int j=1;j<3;j++){
        if(min>s[j]){
            min = s[j];
        }
    }

    //code for checking which is the server having minimum s value
    for(int i=0; i<3; i++){
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

//function for finding exponential distribution
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

int main(){
    int arrivalRate,departRate,populationSize;
    cin>>arrivalRate>>departRate>>populationSize;
    
    double IAT[populationSize],ST[populationSize],que[populationSize],AT[populationSize],DT[populationSize],respT[populationSize],waitQ[populationSize];
    int busy1=0,busy2=0,busy3=0,counter=0,countQ=0;
    double s[3]={0};
    
    //departure rate must be greater than arrival rate
    if(departRate<arrivalRate){
        cout<<"Departure rate of the passenger must be greater than arrival rate\n";
        return 0;
    }

    //generating poisson distribution using arrival rate
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    poisson_distribution<int> distribution1 (3600/arrivalRate); 
  
    for (int i=0; i<populationSize; ++i){
        double a = distribution1(generator1); 
        cout<<"arrival="<<a<<endl;
        IAT[i] = a;
    }

    //generating exponential random variable using departure rate
    for (int i = 0; i < populationSize; ++i)
    {
        double number = Exponential(3600/departRate);
        ST[i] = number;
        cout<<"depart="<<number<<endl;
        chrono::duration<double> period (number);     
    }

    //inserting all the passenger into a queue and calculating their arrival times from inter arrival time
    for(int i=1; i<populationSize; i++){
        que[i]= i;
        AT[i] = AT[i-1]+IAT[i];
    }

    //arrival time of first passenger must be 0
    AT[0] = 0;
    for(int i=0; i<populationSize; i++){

        //passenger selects which Inspector he wants to go
        int sno = findInspecNo(s,AT[i]);  

        //sno equals -1 means no inspector is free to give service
        if(sno==-1){
            //we select the inspector whose curr service value is minimum
            int index = findSmallest(s),max=0;
            
            for(int k=0; k<3; k++){
                if(max<s[k]){
                    max = s[k];
                }
            }
            int j = i+1;
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
    double w=0,r=0;
    for(int i=0; i<populationSize; i++){
        w+=waitQ[i];
        r+=respT[i];
    }
    cout<<"Average number of passenger waiting in queue"<<countQ/populationSize<<endl;
    cout<<"Average waiting time="<<w/populationSize<<endl;
    cout<<"Average number of passenger getting inspected="<<counter/populationSize<<endl;
    cout<<"Average response time="<<r/populationSize<<endl;
    return  0;
}