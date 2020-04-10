#include<bits/stdc++.h>
using namespace std;

int main(int argc,char* argv[]){
    int N,B,T,caseNo = 0;
    float P;
    char sType[3][7] = {"INQ","KOUQ","ISLIP"};
    char *outFile;
    for(int i=1; i<argc; i+=1){
        if(strcmp(argv[i],"-N") == 0){
            N = atoi(argv[i+1]);
        }else if(strcmp(argv[i],"-B") == 0){
            B = atoi(argv[i+1]);
        }else if(strcmp(argv[i],"-P") == 0){
            P = atof(argv[i+1]);
        }else if(strcmp(argv[i],"-queue") == 0){
            if(strcmp(argv[i+1],sType[0]) == 0){
                caseNo = 1;
            }else if(strcmp(argv[i+1],sType[1]) == 0){
                caseNo = 2;
            }else if(strcmp(argv[i+1],sType[2]) == 0){
                caseNo = 3;
            }else{
                cout<<"Invalid Scheduling Type!";
            }
        }else if(strcmp(argv[i],"-K") == 0){
            //not known yet
        }else if(strcmp(argv[i],"-out") == 0){
            outFile = argv[i+1];
        }else if(strcmp(argv[i],"-T") == 0){
            T = atoi(argv[i+1]);
        }
    }
    //cout<<N<<" "<<B<<" "<<P<<" "<<caseNo<<" "<<outFile<<" "<<T<<endl;
    return 0;
}