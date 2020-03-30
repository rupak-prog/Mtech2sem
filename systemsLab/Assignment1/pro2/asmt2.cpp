#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <cstdio>


using namespace std;

char pwd[256];
string path="/home/rupa/";


//function prototyping
void init_shell(bool);
void getpwd();
vector<string> getArgs(string);
void shell_sort(string, bool);
time_t get_mtime(string);
string getfilename(string);
int copy(string, bool);
int getCodeFromCmd(string);
int ls(bool);
int cat(vector<string>, bool);



void init_shell(bool shellpath=false){
	if(shellpath){
		string shell="\nShiRu@linux:~";
		shell = shell+(pwd+12)+"$ ";
		cout<<shell;
	}
	else cout<<"\nShiRu@linux:~$ ";
}

void getpwd(){
	getcwd(pwd, 256);
}

vector<string> getArgs(string cmd){
	string tmp;
	vector<string> args;
	int i, ind, len;
	bool flag=false;
	tmp="";
	len = cmd.length();
	for(i=0; i<len; i++){
		if(cmd[i]==' ' && i<len && tmp!="")
		{
			//cout<<i<<" "<<tmp<<endl;
			args.push_back(tmp);
			tmp="";
			flag=false;
		}
		else{
			tmp = tmp + cmd[i];
			flag=false;
		}
		while(cmd[i]==' ' && cmd[i+1]==' ' && i<len){
			i++;
			flag=true;
		}
	}
	if(!flag && tmp!=""){
		//cout<<i<<" "<<tmp<<endl;
		args.push_back(tmp);
	}
	return args;
}

void shell_sort(string cmd, bool revflag=false){
	string tmp;
	vector<string> buf, v;
	v = getArgs(cmd);
	int i, n = v.size();
	for (i = 1; i < n; ++i)
	{
		if(v[i]=="")continue;
		ifstream filevar(v[i]);
		if (filevar.is_open())
		{
			while ( getline (filevar,tmp) )
			{
				//cout<<tmp<<endl;
				buf.push_back(tmp);
			}
			filevar.close();
		}
	  else cout << "Couldn't open file *"<< v[i] <<"*"<<endl;
	}
  	n = buf.size();
  	sort(buf.begin(), buf.end());

  	if(revflag)for (i = n-1; i > 0; --i)cout<<buf[i]<<endl;
  	else for (i = 0; i < n; ++i)cout<<buf[i]<<endl;
}

time_t get_mtime(string path)
{
    struct stat statbuf;
    if ( stat(path.c_str(), &statbuf) == -1 ) {
        return -1;
    }
    return statbuf.st_mtime;
}

string getfilename(string topath)
{
    string tmp;
    tmp = topath.substr(topath.find_last_of('\//') + 1);
    cout<<"file to be copied: *"<<tmp<<"*"<<endl;
    return tmp;
}

int copy(string cmd, bool updateflag=false)
{
    string from, to, tmp;
    int toind, fromind;

    fromind = cmd.find(' ');
    toind = cmd.find_last_of(' ');
    from = cmd.substr(fromind+1, toind-fromind-1);
    to = cmd.substr(toind+1);   
    if(to[to.length()-1]=='/')to=to+getfilename(from);
    else to=to+"/"+getfilename(from);

    cout<<"from: *"<<from<<"*\nto: *"<<to<<"*"<<endl;

    if(updateflag && (get_mtime(to)!=-1) && get_mtime(to)>get_mtime(from) ){
        cout<<"to: "<<get_mtime(to)<<"\nfrom:"<<get_mtime(from)<<endl;
        cout<<"option -u condition violated!\n";
        return 1;
    }

    ifstream in{from};
    remove(const_cast<char*>(to.c_str()));
    ofstream out{to};
    if (!out) {
        cerr << "Could not copy file to " << to << '\n';
        return 1;
    }
    static constexpr size_t buffsize{1024};
    char buffer[buffsize];
    while (in.read(buffer, buffsize)) {
        out.write(buffer, buffsize);
    }
    out.write(buffer, in.gcount()); 

	return 0;
}



int getCodeFromCmd(string cmd)
{	
	int code, i, len, exist, ind;
	string st, realcmd;
	vector<string> v;
	map <string, int> cmd2code;
	cmd2code.insert({ "ls", 1 });
	cmd2code.insert({ "ls-a", 2 });
	cmd2code.insert({ "cd", 3 });
	cmd2code.insert({ "cat", 4 });
	cmd2code.insert({ "cat-n", 5 });
	cmd2code.insert({ "mkdir", 6 });
	cmd2code.insert({ "mkdir-m", 7 });
	cmd2code.insert({ "cp", 8 });
	cmd2code.insert({ "cp-u", 9 });
	cmd2code.insert({ "sort", 10 });
	cmd2code.insert({ "sort-r", 11 });
	cmd2code.insert({ "grep", 12 });
	cmd2code.insert({ "grep-n", 13 });

	/*string cmd_ar[] = {"ls", "cat", "mkdir", "cp", "sort", "grep", "cd"};
	string flags[]={" -a", " -n", " -m", " -u", " -r", " -n"};
	len = (int)sizeof(cmd_ar)/sizeof(cmd_ar[0]);
	for(i=0; i<len; i++){
		exist = cmd.find(cmd_ar[i]);
		if(!exist)break;
	}

	ind=cmd_ar[i].length();
	if(i<6){
		if(cmd[ind]!=' ' || cmd[ind+1]!='-' || cmd[ind+2]!=flags[i][2])return 0;
	}
	else if(i==6 && cmd[ind]!=' ')return 0;

	if(i<6)realcmd = cmd_ar[i] + flags[i];
	exist = cmd.find(realcmd);
	if(exist)realcmd = cmd_ar[i];
	code = cmd2code[realcmd];
	*/

	v = getArgs(cmd);
	realcmd = v[0]+v[1];
	cout<<"realcmd*"<<realcmd<<"*\n";
	if(cmd2code.find(realcmd)!=cmd2code.end())return cmd2code[realcmd];
	else if(cmd2code.find(v[0])!=cmd2code.end())return cmd2code[v[0]];

	return 0;
}


int ls(bool hid=true)
{
	DIR *dirptr=NULL;
	struct dirent* dirdata;
	//char path[]="/home/shivam/College/Sem2/HPC/";
	getpwd();
	struct stat isfolder;
	dirptr = opendir(pwd);

	if(dirptr == NULL){
		cout<<"Cannot open specified dir";
		return 1;
	}
	while(dirdata = readdir(dirptr)){
		if(strcmp(dirdata->d_name, ".") && strcmp(dirdata->d_name, "..")){
			if(dirdata->d_name[0]=='.' && hid)continue;
			stat(dirdata->d_name,&isfolder);
			if(S_ISDIR(isfolder.st_mode))cout<<"\'"<<dirdata->d_name<<"\'"<<endl;
			else cout<<dirdata->d_name<<endl;
		}
	}
	closedir(dirptr);
	return 0;
}

int cat(vector<string> pat, bool show_lines=false)
{
	int lines=0, len, i;
	ifstream fp;
	string text, curpath;
	len = pat.size();
	//cout<<"\n*in_cat_pat:"<<pat[1]<<"* len"<<len<<endl; 
	
	getpwd();
	curpath = pwd;
	i = show_lines?2:1;
	for (; i < len; ++i)
	{	
		curpath = curpath+"/"+pat[i];
		cout<<"curpath:"<<curpath<<"*"<<endl;
		fp.open(curpath);
		if (!fp.is_open()){
			cout<<"Cannot found specified file.\n";
			return 0;
		}

		if(show_lines)while(getline(fp, text))cout<<setfill(' ')<<setw(6)<<++lines<<" "<<text<<endl;
		else while(getline(fp, text))cout<<text<<endl;
		curpath = pwd;
		fp.close();
	}
	


	return 0;
}

int main(int argc, char const *argv[])
{	
	

	cout<<"\n\n\n\n\n\n\n\n###############*My Terminal*###############\n\n"<<endl;
	init_shell();
	int cmdCode=-1;
	vector<string> arglist;
	string cmd, arg1, arg2;
	while(getline(cin, cmd)){
		arglist = getArgs(cmd);
		cout<<"*"<<cmd<<"*"<<endl;
		if(cmd=="exit")break;
		cmdCode = getCodeFromCmd(cmd);
		switch(cmdCode){
			case 1: ls(); break;
			case 2: ls(false); break;
			case 3: 	break;
			case 4: cat(arglist); break;
			case 5: cat(arglist, true); break;
			case 6:	break;
			case 7: break;
			case 8:	copy(cmd); break;
			case 9: copy(cmd, true); break;
			case 10:	break;
			case 11: 	break;
			case 12:	break;
			case 13: 	break;
			default: cout<<"command not found!"<<endl;
		}

		cout<<"Command is *"<<cmd<<"* code is "<<cmdCode;
		getpwd();
		init_shell();
	}
	cout<<"Exiting...\n";
	return 0;
}