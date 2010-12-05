#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>

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
		cout << "Unable to open input file" << endl;
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
		cout << "Unable to open the file" << endl;
		exit(1);
	}
	
	file << data;
		
	file.close();
	
	return true;
}
