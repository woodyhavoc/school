#ifndef __DFA_H
#define __DFA_H

#include <vector>

using namespace std;

class DFA
{
	public:
	
	DFA(string filename);

	void runTestStrings();

	private:
	
	string startState;
	vector<string> acceptStates;
	vector<string> states;
	vector<string> transitions;
	vector<string> testStrings;
};


#endif /* __DFA_H */
