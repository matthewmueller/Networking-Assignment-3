#include <string>
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

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
			if(!this->isNeighbor(neighbor)) {
				_neighbors.push_back(neighbor);
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
		
		vector<Node> neighbors() { return _neighbors; }

		int compare(Node n) {
			if(this->host().compare(n.host()) == 0 && this->port() == n.port()) {
				return 0;
			} else {
				return -1;
			}
		}

		bool isNeighbor(Node neighbor) {
			for(unsigned int i = 0; i < _neighbors.size(); i++) {
				Node n = _neighbors.at(i);
				
				if((n.host().compare(neighbor.host()) == 0) && (n.port() == neighbor.port())) {
					return true;
				}
			}
			
			return false;
		}
				
		string print() {
			stringstream out;
			out << "Node ( " << _host << " - " << _port << " - " << _online << " ) --> "; 
			out << "{ ";
			for(unsigned int i = 0; i < _neighbors.size(); i++) {
				Node n = _neighbors.at(i);
				
				out << "( " << n.host() << " - " << n.port() << " - " << n.online() << " ) ";

				if(i+1 == _neighbors.size()) break;

				out << ", ";
			}
						
			out << " }";
			
			return out.str();
		}
		
	private:
		string _host;
		int _port;
		bool _online;
		vector<Node> _neighbors;
};