#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "DFA.h"

using namespace std;

DFA::DFA(string filename)
{
	ifstream input;
	string line;
	
	int lineCount = 0;

	input.open(filename.c_str());
	
	if(input.fail())
	{
		cerr << "Invalid file name. Exiting.\n" << endl;
		exit(EXIT_FAILURE);
	}

	while(getline(input, line))
	{
		stringstream stream(line.c_str());
		line = "";
		if(lineCount == 0)
		{
			stream >> startState;

			cout << startState << endl;
		}
		else if(lineCount == 1)
		{
			while(stream >> line)
			{
				acceptStates.push_back(line);
				cout << line << endl;
			}
		}
		else if(lineCount == 2)
		{
			while(stream >> line)
			{
				states.push_back(line);
				cout << line << endl;
			}
		}
		else
		{
			stream >> line;

			if(line.at(0) == '-')
			{
				testStrings.push_back(line);
			}
			else
			{
				transitions.push_back(line);
			}

			cout << line << endl;
		}
		
		lineCount++;
	}

	input.close();
}

void DFA::runTestStrings()
{

}
