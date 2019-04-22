#include <iostream>
#include <string>       
#include <sstream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
	string command;
	vector<string> sentence;
	bool running =true;
	while(running)
	{
		cout<<"Sergio_OS& ";
		cin>>command;

		//string s = "split on    whitespace   ";
    	//vector<std::string> result;
    	istringstream iss(command);
    	for(string command; iss >> command; )
        	sentence.push_back(command);

        cout<<sentence[0];
        cout<<endl;
		sentence.clear();
	}
	return 0;
}