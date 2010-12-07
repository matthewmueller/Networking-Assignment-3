#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#import "linkstatepacket.hpp"
#import "utilities.hpp"

using namespace std;

class LSPacket;

class Node {
	public:
		Node() {
			_online = false;
		}

		Node(string host, int port) {
			_host = host;
			_port = port;
			_online = true;
		}
		
		Node(string host, int port, bool online) {
			_host = host;
			_port = port;
			_online = online;
		}
		
		Node& host(string host) { _host = host; return *this; }
		string host() { return _host; }
		
		Node& port(int port) { _port = port; return *this; }
		int port() { return _port; }
		
		Node& online(bool online) { _online = online; return *this; }
		bool online() { return _online; }

		
		Node& neighbor(Node neighbor) {
			string key = this->getKey(neighbor);
			
			if(!_neighbors.count(key)) {
				_neighbors[key] = neighbor;
				neighbor.neighbor(*this);
			}

		 	return *this;
		}
		
		Node& neighbors(vector<Node> neighbors) {
			
			for(unsigned int i = 0; i < neighbors.size(); i++) {
				this->neighbor(neighbors.at(i));
			}
		
			return *this;
		}
		
		vector<Node> neighbors() {
			map<string, Node>::iterator it;
			vector<Node> neighbors;
			
			for(it = _neighbors.begin(); it != _neighbors.end(); ++it) {
				Node n = it->second;
				neighbors.push_back(n);
			}
			
			return neighbors;
		}

		int compare(Node n) {
			if(this->host().compare(n.host()) == 0 && this->port() == n.port()) {
				return 0;
			} else {
				return -1;
			}
		}

		int isNeighbor(Node neighbor) {
			
			
			return -1;
		}
				
		string print() {
			stringstream out;
			map<string, Node>::iterator it;
			
			out << "Node ( " << _host << " - " << _port << " - " << _online << " ) --> "; 
			out << "{ ";
			
			for(it = _neighbors.begin(); it != _neighbors.end(); ++it) {
				Node n = it->second;
				
				out << "( " << n.host() << " - " << n.port() << " - " << n.online() << " ) ";
		
				// if((it2)->first.compare(_neighbors.end()->first) == 0) break;
		
				out << ", ";
			}
			
			// Slight hack job to remove last comma
			string output = out.str();
			output.erase(output.size()-2);
			
			output += "}";
			
			return output;
		}
		
	private:
		string _host;
		int _port;
		bool _online;
		map<string, Node> _neighbors;
		map<string, LSPacket> _lastPacket;
		
		string getKey(Node n) {
			stringstream key;
			string host = n.host();
			int port = n.port();
			key << host << ":" << port;
			return key.str();
		}
		
		string neighborsToString() {
			stringstream out;
			map<string, Node>::iterator it;
			
			for(it = _neighbors.begin(); it != _neighbors.end(); ++it) {
				Node n = it->second;
				out << n.host() << ":" << n.port() << ":" << n.online();
				out << ",";
			}
			string output = out.str();
			
			output.erase(output.size()-1);
			
			return output;
		}
		
		vector<Node> neighborsFromString(string neighbors) {
			vector<Node> out;
			
			vector<string> nodes = explode(",", neighbors);
			for(unsigned int i = 0; i < nodes.size(); i++) {
				string node = nodes.at(i);
				vector<string>pieces = explode(":", node);
				Node n = Node(pieces[0], atoi(pieces[1].c_str()), atoi(pieces[2].c_str()));
				out.push_back(n);
			}
			
			return out;
		}
};
