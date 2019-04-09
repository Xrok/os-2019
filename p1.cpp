#include<iostream>
#include<fstream>
using namespace std;

void showline( string file, string spec)
{
 ifstream dato (file);
 if(dato.is_open())
 {
	 string sentence;
	 while(getline(dato,sentence))
	 {
		 if(!sentence.find(spec))
		 {
			 cout<<sentence;
		 }
	 }
	 dato.close();
 }else{cout<<"no abrio"<<endl;}

}


int main(){
	showline("/proc/cpuinfo","model name");
	cout<<endl;
	showline("/proc/meminfo", "MemTotal");
	cout<<endl;
	showline("/proc/version","Linux");
	cout<<endl;
	showline("/proc/stat","btime");
	cout<<" seconds since the Epoch"<<endl;

	
return 0;
}
