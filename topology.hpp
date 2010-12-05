#include "Node.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Topology {
	
public:
	Topology() { }	
	
	Topology& add(Node n) { _nodes.push_back(n); return *this; }

	Topology& disable(Node n) {
		n.online(false);
		return *this;
	}
	
	string printTopology() {
		stringstream out;
		for(unsigned int i = 0; i < _nodes.size(); i++) {
			Node n = _nodes.at(i);
			out << "Node ( " << n.host() << " - " << n.port() << " - " << n.online() << " ) : " << n.printNeighbors() << endl;
		}
		
		return out.str();
	}
	
private:	
	vector<Node> _nodes;
	
};