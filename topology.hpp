#include "Node.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "utilities.hpp"

using namespace std;

class Topology {
	
public:
	Topology() {};
	
	Topology(string filename) {
		string contents = file_get_contents(filename);

		vector<string> lines = explode("\n", contents);
		for(unsigned int i = 0; i < lines.size(); i++) {
			vector<string> nodes = explode(" ", lines.at(i));
			
			vector<string> node = explode(",", nodes.at(0));
			Node me = Node().host(node[0]).port(atoi(node[1].c_str()));
			
			for(unsigned int j = 1; j < nodes.size(); j++) {
				vector<string> node = explode(",", nodes.at(j));
				Node n = Node().host(node[0]).port(atoi(node[1].c_str()));
				me.neighbor(n);
			}
			
			this->add(me);
		}
	}	
	
	Topology& add(Node n) { _nodes.push_back(n); return *this; }

	Topology& disable(Node n) {
		n.online(false);
		return *this;
	}
	
	string print() {
		stringstream out;
		for(unsigned int i = 0; i < _nodes.size(); i++) {
			Node n = _nodes.at(i);
			out <<  n.print() << endl;
		}
		
		return out.str();
	}
	
private:	
	vector<Node> _nodes;
	
};