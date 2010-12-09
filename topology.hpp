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
			Node n = Node().host(node[0]).port(atoi(node[1].c_str())).online(true);
			this->add(n);
			
			Node &me = this->get(n);
			
			for(unsigned int j = 1; j < nodes.size(); j++) {
				vector<string> node = explode(",", nodes.at(j));
				// Node n = Node().host(node[0]).port(atoi(node[1].c_str())).online(true);
				int p = atoi(node[1].c_str());
				this->add(node[0], p);
				Node &neighb = this->get(node[0], p);
				
				me.neighbor(neighb);
			}
			
		}
	};	
	
	Node& get(Node n) {
		return this->get(n.host(), n.port());
	}
	
	Node& get(string host, int port) {
		string key = this->getKey(host, port);
				
		if(_nodes.count(key)) {
			return _nodes[key];
		}
		
		cout << "Emulator node not present in topology table" << endl;
		exit(1);
	};
	
	Topology& add(Node n) { 
		string key = this->getKey(n);
		
		if(!_nodes.count(key))
			_nodes[key] = n;

		return *this;
	};
	
	Topology& add(string host, int port) { 
		Node n = Node(host, port, 1);
		this->add(n);

		return *this;
	};

	Topology& disable(string host, int port) {
		Node &n = this->get(host, port);
		this->disable(n);
		return *this;
	}

	Topology& disable(Node n) {
		string key = this->getKey(n);
		// Pull key from mapping
		Node &node = _nodes[key];

		node.online(false);
		//offline.online(false);
		return *this;
	};
	
	Topology& enable(string host, int port) {
		Node &n = this->get(host, port);
		this->enable(n);
		return *this;
	}
	
	Topology& enable(Node n) {
		string key = this->getKey(n);
		Node &node = _nodes[key];
		
		node.online(true);
		
		return *this;
	}
		
	string getNeighborsForLSP(Node n) {
		stringstream out;
		
		vector<Node> _neighbors = n.neighbors();

		for(unsigned int i = 0; i < _neighbors.size(); i++) {
			Node n = _neighbors.at(i);
			n = this->get(n);
			out << n.host() << ":" << n.port() << ":" << n.online();
			out << ",";				
		}
		
		string output = out.str();
		if(!output.empty()) {
			output = output.erase(output.size()-1);
		}
		
		return output;
	}
	
	vector<Node> neighborsFromString(string neighbors) {
		vector<Node> out;
		vector<string> nodes = explode(",", neighbors);

		for(unsigned int i = 0; i < nodes.size(); i++) {
			string node = nodes.at(i);
			vector<string>pieces = explode(":", node);

			if(!pieces[0].empty() && !pieces[1].empty()) {
				Node n = Node(pieces[0], atoi(pieces[1].c_str()), atoi(pieces[2].c_str()));
				out.push_back(n);		
			}
			
		}
		
		return out;
	}
	
	string print() {
		stringstream out;
		map<string, Node>::iterator it;
		
		for(it = _nodes.begin(); it != _nodes.end(); ++it) {
			Node n = it->second;
			n = this->get(n);
			
			out << "Node ( " << n.host() << " - " << n.port() << " - " << n.online() << " ) --> "; 
			out << "{ ";
			
			vector<Node>neighbors = n.neighbors();
			for(int unsigned i = 0; i < neighbors.size(); i++) {
				Node neighbor = neighbors.at(i);
				neighbor = this->get(neighbor);
				out << "( " << neighbor.host() << " - " << neighbor.port() << " - " << neighbor.online() << " ) ";
		
				// if((it2)->first.compare(_neighbors.end()->first) == 0) break;
		
				out << ", ";
			}
			
			out << " }" << endl;
			
		}
		
		return out.str();
	};
	
private:
	string getKey(string host, int port) {
		stringstream key;
		key << host << ":" << port;
		return key.str();
	}
	
	string getKey(Node n) {
		stringstream key;
		string host = n.host();
		int port = n.port();
		key << host << ":" << port;
		return key.str();
	}
	
	map<string, Node> _nodes;
	
};

