#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <climits>
#include <iomanip>

using namespace std;
int VCID=0;
#define N 100

// declaring prototypes of function defines
void printPath(int path[][N], int v, int u, vector<int> &pathforfile);
void printfirstSolution(int cost[N][N], int path[N][N]);
void printsecondSolution(int cost[N][N], int path[N][N], int v, int u);
void FloydWarshell(int, int, bool);
void initmatrix();
void genSecondPaths();
void read_topology();
void read_connection();
void provideConn();
void printConnDetails();
void fillVcidList();
void compNprintFwdTab();
void calcShortP(vector<vector<vector<int>>> &p, string filename);
void print(vector<vector<vector<int>>> &p);
void createRoutingTable();
void compPathDelays();
void createPathFile();


// topology is read as struct
struct top
{
	int delay;
	double capaciti;
};
// connection is read as struct
struct conn
{
	double minbw, avgbw, maxbw;
	int  source, desti, cid;
};

//filenames passed are stored in fnames vector
vector<string> fnames;
int vertices, edges, requests, numOfConnEst;
map< pair<int, int>, top > topology;
map<int, vector<int> > conid2nodes;
map< pair<int, int>, pair<int, int> > pathCosts;
vector<int> connSuccess;//(requests, 0);
map< pair<int, int>, double >capacities;
map< pair<int, int>, pair<int, int> > vcidList;
vector< vector< vector<int> > > fpath;
vector< vector< vector<int> > > spath;
vector<conn> connection;
// some global variables defined here
int path1Delays[N][N];
int path2Delays[N][N];
int adjmat[N][N];
bool isDist=true, pflag=true;
string s1file="firstShortestPaths.txt";
string s2file="secondShortestPaths.txt";

// this funtion sets the vcidlist for different connections
void fillVcidList(){
	int s, d, tmpl;
	for (int i = 0; i < requests; i++)
	{
		s = connection[i].source;
		d = connection[i].desti;
		if(connSuccess[i]==1){
			tmpl = fpath[s][d].size()-1;
		}
		else if(connSuccess[i]==2){
			tmpl = spath[s][d].size()-1;
		}
		VCID = VCID + tmpl;
		vcidList.insert({ {s, d} , {tmpl, VCID} });
	}
}

//shortest file is read in 3d vectors 
void calcShortP(vector<vector<vector<int>>> &p, string filename)
{
	string abc;
	ifstream fi(filename);
	stringstream ss;
	int tmp;
	
	p.assign(vertices, vector<vector<int>>(vertices, vector<int>()));
	
	while(getline(fi, abc))
	{
		ss.clear();
		ss << abc;
		int u, v, x, y;
		ss >> u >> v >> x >> y;
		while(ss.rdbuf()->in_avail())
		{
			ss >> tmp;
			p[u][v].push_back(tmp);
		}
	}
	fi.close();
}

// this function is used to compute and 
// print forwarding table in file
void compNprintFwdTab(){
	int s, d, tmpl, inport, opport, rtid, vcid1;
	
	ofstream ftfile;
	ftfile.open(fnames[3], ios::out);

	for (int i = 0; i < requests; i++)
	{
		s = connection[i].source;
		d = connection[i].desti;
		// this condition checks if first shortest
        // is valid for connection
		if(connSuccess[i]==1){
			tmpl = fpath[s][d].size();
			inport=-1;
			rtid=fpath[s][d][0];
			opport=fpath[s][d][1];
			vcid1=vcidList.find({s,d})->second.second;
			ftfile<<rtid<<" | "<<inport<<" | -1 | "<<opport<<" | "<<vcid1<<"\n";
			//cout<<rtid<<" | "<<inport<<" | -1 | "<<opport<<" | "<<vcid1<<"\n";
			for (int ind = 1; ind < tmpl-1; ind++)
			{
				inport=fpath[s][d][ind-1];
				rtid=fpath[s][d][ind];
				opport=fpath[s][d][ind+1];
				ftfile<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | "<<vcid1<<"\n";
				//cout<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | "<<vcid1<<"\n";
			}
			inport=fpath[s][d][tmpl-2];
			rtid=fpath[s][d][tmpl-1];
			opport=-1;
			ftfile<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | -1\n";
			//cout<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | -1\n";
		}
        // this condition checks if second shortest
        // is valid for connection
		else if(connSuccess[i]==2){
			// pathVec = secondSpaths.find({s, d})->second;
			// pathVec = spath[s][d];
			tmpl = spath[s][d].size();
			//cout<<s<<" "<<d<<" "<<tmpl<<endl;
			inport=-1;
			rtid=spath[s][d][0];
			opport=spath[s][d][1];
			vcid1=vcidList.find({s,d})->second.second;
			ftfile<<rtid<<" | "<<inport<<" | -1 | "<<opport<<" | "<<vcid1<<"\n";
			//cout<<rtid<<" | "<<inport<<" | -1 | "<<opport<<" | "<<vcid1<<"\n";
			for (int ind = 1; ind < tmpl-1; ind++)
			{
				inport=spath[s][d][ind-1];
				rtid=spath[s][d][ind];
				opport=spath[s][d][ind+1];
				ftfile<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | "<<vcid1<<"\n";
				//cout<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | "<<vcid1<<"\n";
			}
			inport=spath[s][d][tmpl-2];
			rtid=spath[s][d][tmpl-1];
			opport=-1;
			ftfile<<rtid<<" | "<<inport<<" | "<<vcid1<<" | "<<opport<<" | -1\n";
        }
	}
	ftfile.close();
}

// this function computes the path delays
// for all node pairs
void compPathDelays()
{
	path1Delays[N][N] = {0};
	path2Delays[N][N] = {0};
	vector<int> lpath1;
	vector<int> lpath2;
	int len1, len2;
	for (int v = 0; v < vertices; v++)
	{
		for (int u = 0; u < vertices; u++)
		{
			if(v!=u)
            {
				lpath1 = fpath[v][u];
				len1=lpath1.size();
				for (int i = 0; i < len1-1; i++)
				{
					path1Delays[v][u]+=topology.find(make_pair(lpath1[i], lpath1[i+1]))->second.delay;
				}
				lpath2 = spath[v][u];
				len2=lpath2.size();
				for (int i = 0; i < len2-1; i++)
				{
					path2Delays[v][u]+=topology.find(make_pair(lpath2[i], lpath2[i+1]))->second.delay;
				}
			}
			
		}
		
	}
	
}

void createRoutingTable()
{
    // iterating through first shortest paths
	ofstream f1;
	f1.open(fnames[2]);
	f1<<"First shortest paths:\n";
	vector<int> lpath1;
	int len1;
	for (int v = 0; v < vertices; v++)
	{
		f1<<"Source: "<<v<<"\n";
		for (int u = 0; u < vertices; u++)
		{
			if(v!=u){
				f1<<u<<" | ";
				lpath1 = fpath[v][u];
				len1=lpath1.size();
				for (int i = 0; i < len1; i++)
				{
					f1<<lpath1[i]<<" ";
				}
				f1<<" | "<<path1Delays[v][u]<<" | "<<len1<<endl;;
			}
			
		}
		
	}

    // iterating through second shortest paths
	f1<<"\nSecond shortest paths:\n";
	for (int v = 0; v < vertices; v++)
	{
		f1<<"Source: "<<v<<"\n";
		for (int u = 0; u < vertices; u++)
		{
			if(v!=u){
				f1<<u<<" | ";
				lpath1 = spath[v][u];
				len1=lpath1.size();
				for (int i = 0; i < len1; i++)
				{
					f1<<lpath1[i]<<" ";
				}
				f1<<" | "<<path2Delays[v][u]<<" | "<<len1<<endl;;
			}
			
		}
		
	}
	f1.close();
}

void createPathFile()
{
	ofstream fptr;
	fptr.open(fnames[4]);
	fptr<<requests<<" "<<numOfConnEst<<endl;
	fptr.close();
}

// this recursive functions trace and stores the path 
void printPath(int path[][N], int v, int u, vector<int> &pathforfile)
{
	if (path[v][u] == v)
		return;

	printPath(path, v, path[v][u], pathforfile);
	//cout << path[v][u] << " ";
	pathforfile.push_back(path[v][u]);
}


void provideConn()
{
    // this is the core function for providing the connections
	for (int i = 0; i < requests; i++)
		connSuccess.push_back(0);
	
	int id, s, d, path1or2=0, len, tmps, tmpd, tmps1, tmpd1, vlen, it2;
	double band;
	numOfConnEst=0;
	// iterating through all the requests
	for (int it1=0; it1<requests; it1++)
	{
		s=connection[it1].source;
		d=connection[it1].desti;
		if(pflag) band = connection[it1].maxbw;
		else{
			band = min(connection[it1].maxbw, connection[it1].avgbw + 0.25*(connection[it1].maxbw - connection[it1].minbw));
		}
        // checks if first shortest path can be assign for connections
		len = fpath[s][d].size();
		path1or2=1;
		int i1=0;
		while(i1<len-1){
			tmps=fpath[s][d][i1];
			tmpd=fpath[s][d][i1+1];
			//cout<<"first src "<< tmps <<" des "<<tmpd<<" cap " << capacities.find({tmps, tmpd})->second<<endl;
			if(capacities.find(make_pair(tmps, tmpd))->second < band){
				path1or2=0;
				break;
			}
			i1++;
		}
        // checks only if first path fails for connections assignment
		if(path1or2==0){
			len=spath[s][d].size();
			path1or2=2;
			int i2=0;
			while(i2<len-1){
				tmps=spath[s][d][i2];
				tmpd=spath[s][d][i2+1];
				//cout<<"second src "<< tmps <<" des "<<tmpd<<" cap " << capacities.find({tmps, tmpd})->second<<endl;
				if(capacities.find(make_pair(tmps, tmpd))->second < band){
					path1or2=0;
					break;
			}
				i2++;
			}
		}
		// connection is possible through any of the shortest paths
		if(path1or2!=0){
			it2=0;
			numOfConnEst++;
			connSuccess[it1]=path1or2;
			if(path1or2==1){
				vlen = fpath[s][d].size();
				while(it2<vlen-1){
					tmps1 = fpath[s][d][it2];
					tmpd1 = fpath[s][d][it2+1];
					//cout<<"deduction "<<"src "<< tmps1 <<" des "<<tmpd1<<" cap " << capacities.find({tmps1, tmpd1})->second<<" ";
					(capacities.find({tmps1, tmpd1})->second) -= band; 
					(capacities.find({tmpd1, tmps1})->second) -= band;
					//cout<<capacities.find({tmps1, tmpd1})->second<<" "<<capacities.find({tmps1, tmpd1})->second<<endl;
					it2++;
				}
			} 
			else if(path1or2==2){
				vlen = spath[s][d].size();
				while(it2<vlen-1){
					tmps1 = spath[s][d][it2];
					tmpd1 = spath[s][d][it2+1];
					//cout<<"deduction "<<"src "<< tmps1 <<" des "<<tmpd1<<" cap " << capacities.find({tmps1, tmpd1})->second<<" ";
					(capacities.find({tmps1, tmpd1})->second) -= band; 
					(capacities.find({tmpd1, tmps1})->second) -= band;
					//cout<<capacities.find({tmps1, tmpd1})->second<<" "<<capacities.find({tmps1, tmpd1})->second<<endl;
					it2++;
				}
			}
		}

	}

}

void printConnDetails()
{
	for (int i = 0; i < requests; i++)
	{
        // connection details are printed for all 
        // accepted connections
		if(connSuccess[i]!=0){
			int s, d, id, cost, len, vid, tmplen;
			vector<int> tmpPath;
			s = connection[i].source;
			d = connection[i].desti;
			id = connection[i].cid;
			if(connSuccess[i]==1){
				cost = pathCosts.find({s,d})->second.first;
				tmpPath = fpath[s][d];
			}
			else if(connSuccess[i]==2){
				cost = pathCosts.find({s,d})->second.second;
				tmpPath = spath[s][d];
			}
			len=tmpPath.size();
			cout<<i+1<<" | "<<s<<" "<<d<<" |  ";
			for (int ind = 0; ind < len; ind++)
			{
				cout<<tmpPath[ind]<<" ";
			}
			cout<<" |  ";
			tmplen= vcidList.find(make_pair(s, d))->second.first;
			vid= vcidList.find(make_pair(s, d))->second.second;
			for (int x = vid-tmplen+1; x <= vid; x++) cout<<x<<" ";
			
			cout<<" |  "<<cost<<endl;
		}
	}
	
}

void printsecondSolution(int cost[N][N], int path[N][N], int v, int u)
{
    // second path is generated for specific source and destination
	ofstream matptr;
	matptr.open(s2file, ios::out|ios::app);
	int len=0;
	if (u != v && path[v][u] != -1) 
	{
		vector<int> pathforfile;
		//cout <<"Second path: "<< v << " to " << u <<" dist: " << cost[u][v] <<" path: (" << v << " ";
		printPath(path, v, u, pathforfile);
		//cout << u << ")" << endl;
		len=pathforfile.size();
		// write to file
		matptr << v << " " << u << " " << cost[v][u] << " " << len+2 << " " << v << " ";
		for (int i = 0; i < len; ++i)
		{
			matptr<< pathforfile[i]<<" ";
		}
		matptr<<u<<"\n";
		
		pathCosts[{v,u}].second = cost[v][u];
		pathforfile.clear();
		vector<int>().swap(pathforfile);
	}
	matptr.close();
}

// Function to print the shortest cost with path 
// information between all pairs of vertices
void printfirstSolution(int cost[N][N], int path[N][N])
{
	int len;
	ofstream matptr;
	matptr.open(s1file);
	for (int v = 0; v < vertices; v++) 
	{
		for (int u = 0; u < vertices; u++) 
		{
			if (u != v && path[v][u] != -1) 
			{
				vector<int> pathforfile;
				//cout << v << " to " << u <<" dist: " << cost[u][v] <<" path: (" << v << " ";
				printPath(path, v, u, pathforfile);
				//cout << u << ")" << endl;
				len=pathforfile.size();
				matptr << v << " " << u << " " << cost[v][u] << " " << len+2 << " " << v << " ";
				for (int i = 0; i < len; ++i)
				{
					matptr<< pathforfile[i]<<" ";	
				}
				matptr<<u<<"\n";
				
				pathCosts[{v,u}].first = cost[v][u];
				pathforfile.clear();
				vector<int>().swap(pathforfile);
			}
		}
	}
	matptr.close();
}

// Function to run Floyd-Warshell algorithm
void FloydWarshell(int v1=0, int v2=0, bool first=true)
{
	// cost[] and parent[] stores shortest-path 
	// (shortest-cost/shortest route) information
	int cost[N][N], path[N][N];

	// initialize cost[] and parent[]
	for (int v = 0; v < vertices; v++) 
	{
		for (int u = 0; u < vertices; u++) 
		{
			// initally cost would be same as weight 
			// of the edge
			cost[v][u] = adjmat[v][u];

			if (v == u)
				path[v][u] = 0;
			else if (cost[v][u] != INT_MAX)
				path[v][u] = v;
			else
				path[v][u] = -1;
		}
	}

	for (int k = 0; k < vertices; k++) 
	{
		for (int v = 0; v < vertices; v++) 
		{
			for (int u = 0; u < vertices; u++) 
			{

				if (cost[v][k] != INT_MAX && cost[k][u] != INT_MAX
					&& cost[v][k] + cost[k][u] < cost[v][u]) 
				{
					cost[v][u] = cost[v][k] + cost[k][u];
					path[v][u] = path[k][u];
				}
			}

			// if diagonal elements become negative, the
			// graph contains a negative weight cycle
			if (cost[v][v] < 0) 
			{
				cout << "Negative Weight Cycle Found!!";
				return;
			}
		}
	}

	// Print the shortest path between all pairs of vertices
	if(first)printfirstSolution(cost, path);
	else printsecondSolution(cost, path, v1, v2);

}

void initmatrix()
{
	// it fills the adjacency matrix on the basis of 
    // flag given on command line argument
    for (int i = 0; i < vertices; ++i)
		for (int j = 0; j < vertices; ++j)
			adjmat[i][j] = INT_MAX;

	for (int i = 0; i < vertices; ++i)adjmat[i][i] = 0;

	int v, e, t1, t2, t3, t4;
	string rest="";
	ifstream inp_file;
	inp_file.open(fnames[0]);
	inp_file >> v >> e;
	if(isDist)
	{ //if dist is given
		while(inp_file >> t1 >> t2 >> t3 >> t4 >> rest){
			//adjmat[{t1, t2}] = t3;
			adjmat[t1][t2] = t3;
			adjmat[t2][t1] = t3;
		}
	}
	else
	{ // if hop is given 
		while(inp_file >> t1 >> t2 >> t3 >> t4 >> rest){
			//adjmat[{t1, t2}] = 1;
			adjmat[t1][t2] = 1;
			adjmat[t2][t1] = 1;
		}
	}
	inp_file.close();

}

void genSecondPaths()
{
	// this function removes the first shortest paths and remove it
    // for applying shortest path algo again on network
    int i, lim = vertices*(vertices-1);
	ifstream readpath;
	int src, des, cst, plen, rowind, colind;
	readpath.open(s1file);

	while(readpath >> src >> des >> cst >> plen >> rowind){ // n^2
		initmatrix();
		while(plen-->1){
			readpath>>colind;
			adjmat[rowind][colind]=INT_MAX;
			adjmat[colind][rowind]=INT_MAX;
			rowind=colind;
		}
		FloydWarshell(src, des, false); // n^3 times
	}
	readpath.close();
}

void read_topology()
{
    //this function reads the topology file
	int src, des, del, rel;
	double cap;
	string rest="";
	ifstream inp_file;
	inp_file.open(fnames[0]);
	inp_file >> vertices >> edges;
	while(inp_file >> src >> des >> del >> cap >> rest){
		top tvar;
		tvar.delay = del;
		tvar.capaciti = cap;
		topology.insert({{src,des},tvar});
		// cout<<"rest is "<<rest<<endl;
		capacities.insert({ {src, des}, cap});
		capacities.insert({ {des, src}, cap});
	}
	inp_file.close();
}

void read_connection()
{
    // this function reads the connection files and 
    // updates the connection data structures
	int src, des; 
	double minb, avgb, maxb;
	ifstream inp_file;
	inp_file.open(fnames[1]);
	inp_file >> requests;
	int i=1;
	while(inp_file >> src >> des >> minb >> avgb >> maxb){
		conn cvar;
		cvar.source = src;
		cvar.desti = des;
		cvar.minbw = minb;
		cvar.avgbw = avgb;
		cvar.maxbw = maxb;
		cvar.cid = i;
		connection.push_back(cvar);
		vector<int> tmp;
		tmp.push_back(src);
		tmp.push_back(des);
		conid2nodes.insert(make_pair(i, tmp));//    [i]=vector<int>{src, des};
		i++;
	}
	inp_file.close();
}


int main(int argc, char const *argv[])
{
	// parsing command to extract fnames
	// and storing it into global string array
	remove(s2file.c_str());
	if(argc<15){
		cout<<"Too few arguments.\n"<<endl;
		return 1;
	}
	else if(argc>15){
		cout<<"Too many arguments.\n"<<endl;
		return 1;
	}
	else{
		for (int i = 2; i < argc; i+=2)
		{
			string tmp(argv[i]);
			fnames.push_back(tmp);
		}
	}
	if(argv[1]!="-top" || argv[3]!="-conn" || argv[5]!="-rt" || argv[7]!="-ft" || argv[9]!="-path" || argv[11]!="-flag" || argv[13]!="-p"){
		cout<<"Incorrect flag in arguments!\n"<<endl;
		return 1;
	}

	if(fnames[5]=="hop")isDist=false;
	if(fnames[6]=="0")pflag=false;

	read_topology();
	read_connection();
	
	//printconn();
	initmatrix();
	FloydWarshell();
	genSecondPaths();
	calcShortP(fpath,s1file);
	calcShortP(spath, s2file);
	//printtop();
	provideConn();
	fillVcidList();
	printConnDetails();
	cout<<"\nNUMBER OF CONN EST: "<<numOfConnEst<<endl;
	compNprintFwdTab();
	compPathDelays();
	createRoutingTable();
	createPathFile();

	return 0;
}