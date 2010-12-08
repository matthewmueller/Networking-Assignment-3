#include <string>
#include <iostream>
#include <map>
#include <sstream>
#import "utilities.hpp"
#import "node.hpp"
#import "linkstatepacket.hpp"

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
			Node me = Node().host(node[0]).port(atoi(node[1].c_str())).online(true);
			
			for(unsigned int j = 1; j < nodes.size(); j++) {
				vector<string> node = explode(",", nodes.at(j));
				Node n = Node().host(node[0]).port(atoi(node[1].c_str())).online(true);
				me.neighbor(n);
			}
			
			this->add(me);
		}
	};	
	
	Node getNode(string host, int port) {
		stringstream key;
		key << host << ":" << port;
				
		if(_nodes.count(key.str())) {
			return _nodes[key.str()];
		}
		
		cout << "Emulator node not present in topology table" << endl;
		exit(1);
	};
	
	Topology& add(Node n) { 
		string key = this->getKey(n);
		_nodes[key] = n;
		
		return *this;
	};

	Topology& disable(Node n) {
		string key = this->getKey(n);
		Node offline = _nodes[key];
		offline.online(false);
		return *this;
	};
	
	string print() {
		stringstream out;
		map<string, Node>::iterator it;
		
		for(it = _nodes.begin(); it != _nodes.end(); ++it) {
			Node n = it->second;
			out <<  n.print() << endl;
		}
		
		return out.str();
	};
	
private:
	string getKey(Node n) {
		stringstream key;
		string host = n.host();
		int port = n.port();
		key << host << ":" << port;
		return key.str();
	}
	
	map<string, Node> _nodes;
	
};

