#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/timeb.h>
#include <cstdlib>

using namespace std;

vector<string> explode( const string &delimiter, const string &str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0; 
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
			
    return arr;
}

string file_get_contents(string filename = "") {
	if (filename.empty()) {
		return "";
	}
	
	ifstream file;
	file.open(filename.c_str());
	
	if(!file.is_open()) {
		cout << "file_get_contents: Unable to open input file" << endl;
		exit(1);
	}
	
	string output = "";
	string line = "";
	
	while(true) {
		getline(file, line);
		output += line;
		
		if(file.eof()) break;
		
		output += "\n";
	}

	// output[output.size() - 1] = NULL;

	file.close();
	
	return output;
}

bool file_put_contents(string filename, string data, string mode = "truncate") {
	
	ofstream file;
	
	if(mode.compare("append") == 0) {
		file.open(filename.c_str(), ios::app);
	} else {
		file.open(filename.c_str(), ios::trunc);
	}
	
	if(!file.is_open()) {
		cout << "file_put_contents: Unable to open the file" << endl;
		exit(1);
	}
	
	file << data;
		
	file.close();
	
	return true;
}

string getArg(string flag, char const **argv) {
	stringstream _args;
	
	if(flag.size() == 1) {
		flag = "-" + flag;
	}
		
	// string sarg = "";
	for(unsigned int i = 0; i < sizeof(argv); i++ ) {
		_args << argv[i] << " ";

	}
	
	string args = _args.str();

	vector<string> argArray = explode(" ", args);
	
	// cout << "SIZE : " << argArray.size();
	for(unsigned int i = 0; i < argArray.size(); i++) {
		string arg = argArray.at(i);
		if(arg.compare(flag) == 0) {
			// cout << "Arg : " << arg << endl;
			// cout << "Next spot : " << i+1 << endl;
			if(argArray.at(i+1)[0] == '-' || argArray.at(i+1).empty()) {
				return "1";
			} else {
				return argArray.at(i+1);
			}
		}
	}
	
	// args << *argv;
	
	// cout << args.str() << endl;
	
	return "";
}

string colorize(string text = "", string color = "") {
	if(color.compare("black") == 0) {
		return "\033[0;30m" + text + "\033[0m\n";
	} 
	else if(color.compare("red") == 0) {
		return "\033[0;31m" + text + "\033[0m\n";
	}
	else if(color.compare("green") == 0) {
		return "\033[0;32m" + text + "\033[0m\n";
	} 	
	else if(color.compare("yellow") == 0) {
		return "\033[0;33m" + text + "\033[0m\n";
	}
	else if(color.compare("blue") == 0) {
		return "\033[0;34m" + text + "\033[0m\n";
	}
	else if(color.compare("pink") == 0) {
		return "\033[0;35m" + text + "\033[0m\n";
	}
	else if(color.compare("sky") == 0) {
		return "\033[0;36m" + text + "\033[0m\n";
	}
	else if(color.compare("grey") == 0) {
		return "\033[0;37m" + text + "\033[0m\n";
	}
	else {
		return text + "\n";		
	}
}

string getMyHost() {
	char myHostName[255];
	gethostname(myHostName,255);
	string hostName(myHostName);
	int strIndex = hostName.find_first_of(".");
	hostName = hostName.substr(0,strIndex);	
	
	return hostName;
}

string getMyIP()
{
	char hostname[255];
	gethostname(hostname, 255);
		
	struct hostent *record;
	
	if((record = gethostbyname(hostname)) == NULL)
	{
		cout << "ERROR: error getting localhost's IP." << endl;
		exit(1);
	}
	
	in_addr *address = (in_addr *)record->h_addr_list[0];
	string IP = inet_ntoa(*address);

	return IP;
}

// Return the system time in format HH:MM:SS.mmm where mmm is the milliseconds
// timeStr - is the array where the return string is stored
// size - is the size of timeStr
string getTime()
{
	struct timeb systime;
	ftime(&systime);
	time_t timet = systime.time;
	struct tm * timeinfo = localtime (&timet);
	char timeStr[20];
	strftime (timeStr,20,"%H:%M:%S",timeinfo);

	int len = strlen(timeStr);
	int milli = systime.millitm;
	milli = milli % 1000;  //should be less than thousand already, but just in case


    timeStr[len] = '.';

	if (milli < 10)
	{
		timeStr[len+1] = '0';
		timeStr[len+2] = '0';
		timeStr[len+3] = "0123456789"[milli];
	}

	else if (milli < 100)
	{
		timeStr[len+1] = '0';
		timeStr[len+2] = "0123456789"[milli / 10];
		timeStr[len+3] = "0123456789"[milli % 10];
	}

	else  //  100 <= milli < 1000
	{
		timeStr[len+1] = "0123456789"[milli / 100];
		timeStr[len+2] = "0123456789"[(milli % 100) / 10];
		timeStr[len+3] = "0123456789"[milli % 10];
	}

	timeStr[len + 4] = '\0';
	
	return string(timeStr);
}

//return system time in milliseconds
long long getMilliTime()
{
	long long retVal;
	struct timeb sysTime;
	ftime(&sysTime);
	retVal = (long long)sysTime.time * 1000;
	retVal = retVal + (long long)sysTime.millitm;
	return retVal;
}

long long getElapsedTime(long long startTime) {
	long long endTime = getMilliTime();
	return endTime - startTime;
}

int rand(int start, int end) {
	srand(getMilliTime());
	int randomNumber = rand()%(end-start)+start;
	return randomNumber;
}


