#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

string line(string file , string spec){
	ifstream dato (file);
	if(dato.is_open())
	{
		string sentence;
		while(getline(dato,sentence))
		{
			if(!sentence.find(spec)){
				return sentence;
			}
		}
	return "not find spec";	
		
	}else{return "no abrio";}
}



int extractnum(string sentence){
	stringstream ss;
	ss<<sentence;
	string temp;
	int number;
	while(!ss.eof()){
	ss>>temp;
	if(stringstream(temp)>> number){
		return number;
	temp="";
	}	
}
}

int extractnum(string sentence,int n ){
	stringstream ss;
	ss<<sentence;
	string temp;
	int number, v=0;
	while(!ss.eof()){
	ss>>temp;
	if(stringstream(temp)>> number  ){
		v++;
		if(n==v){
			return number;
		}
	
	temp="";
	}	
}
}



int main()
{
	
	while(true){
	int total = extractnum(line("/proc/meminfo","MemTotal"));
	int free = extractnum(line("/proc/meminfo","MemFree")) ;
	float timecpu[7];
	float totaltime=0;
	for(int j=1;j<8;j++){
		timecpu[j-1]=extractnum(line("/proc/stat","cpu"),j);
		totaltime=totaltime+timecpu[j-1];
	}

		cout<<(free*100)/total<<"% de memoria libre"<<endl;
		cout<<timecpu[0]*100/totaltime<<"% user"<<endl;
		cout<<timecpu[2]*100/totaltime<<"% system"<<endl;
		cout<<timecpu[3]*100/totaltime<<"% idle"<<endl;
		cout<<extractnum(line("/proc/stat","ctxt"))<<" number of context switches"<<endl;
		cout<<extractnum(line("/proc/stat","processes"))<<" number of processes"<<endl;
	
		cout<<"----------------------------------------"<<endl;	
		sleep_for(seconds(1));
		//cout<<i<<" "<<endl;
	}
}
