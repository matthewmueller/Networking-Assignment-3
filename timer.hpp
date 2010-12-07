#include <string>
#include <iostream>
#include <map>
#include <sstream>
#import "utilities.hpp"

using namespace std;

class Timer {
	public:
		Timer() {}
		
		void add(string name, int wait) {
			long long timeStart = getMilliTime();
			_timers[name]["start"] = timeStart;
			_timers[name]["wait"] = wait;
		}
		
		void remove(string name) {
			_timers.erase(name);
		}
		
		bool isDone(string name) {
			if(this->timeLeft(name) < 0) {
				return true;
			} else {
				return false;
			}
		}
		
		long long timeLeft(string name) {
			if(_timers.count(name)) {
				long long timeStart = _timers[name]["start"];
				int wait = _timers[name]["wait"];
				long long elapsed = getElapsedTime(timeStart);
				return wait-elapsed;
			} else {
				return false;
			}
		}
		
	private:
		map<string, map<string, long long > > _timers;
};