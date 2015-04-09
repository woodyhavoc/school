#include <iostream>
#include "DFA.h"

using namespace std;

int main(int argc, char* argv[])
{
	DFA *dfa = new DFA(argv[1]);

	dfa->runTestStrings();
	
	delete(dfa);

	return 0;
}
