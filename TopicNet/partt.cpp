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
    vector<double> IAT;
    vector<double> ST;
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count(); 
    default_random_engine generator1 (seed); 
    int x = 60,y = 80;
    poisson_distribution<int> distribution1 (3600/x); 
  
    cout << "Poisson-distribution(mean=5.0): ";
    for (int i=0; i<30; ++i){
        int a = distribution1(generator1); 
        IAT.push_back(a); 
        cout<<a<<"=rand";
    }
    fflush(stdin);
     
    for (int i=0; i<30; ++i)  
    {      
        double number = Exponential(3600/y);
        ST.push_back(number);
        cout<<"kop"<<number<<endl;
        chrono::duration<double> period (number);  
    } 

    //find out arrival time of each passenger
    double AT[30],TSB[30],TSE[30],waitQ[30],waitS[30],SI[30];
    int pre[3];
    AT[0] = 0;
    for(int i=1; i<30; i++){
        AT[i] = AT[i-1]+IAT[i];
    }
return 0;
}