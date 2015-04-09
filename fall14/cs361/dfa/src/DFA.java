import java.util.ArrayList;
import java.util.Arrays;

/**
 * 
 * @author awoods
 * 
 * This class is used to generate a DFA from an NFA, and then check a series of strings
 * to see if they can be processed by the DFA.
 */
public class DFA
{
	private ArrayList<String> nfaStates;
	private ArrayList<String> nfaTransitions;
	private ArrayList<String> testStrings;
	private ArrayList<String> dfaStates;
	private ArrayList<String> dfaTransitions;
	private ArrayList<String> symbols;
	private ArrayList<String> testResults;
	private ArrayList<String> acceptStates;
	private String start;

	/**
	 * Constructor, creates a DFA object.  Accepts ArrayList<String> objects for
	 * the accept states, states, transitions, test strings, and symbols.  It also
	 * initializes the ArrayList<String> dfaStates, dfaTransitions, and testResults.
	 * 
	 * @param start String containing the start state of the NFA.
	 * @param acceptStates ArrayList<String> containing the accept states of the NFA.
	 * @param states ArrayList<String> containing the valid states of the NFA.
	 * @param transitions ArrayList<String> containing the transitions of the NFA.
	 * @param testStrings ArrayList<String> containing the strings to be tested
	 * 					  against the DFA.
	 * @param symbols ArrayList<String> containing the symbols used in the NFA.
	 */
	public DFA(String start, ArrayList<String> acceptStates, 
			ArrayList<String> states, ArrayList<String> transitions, 
			ArrayList<String> testStrings, ArrayList<String> symbols)
	{
		this.nfaStates = states;
		this.nfaTransitions = transitions;
		this.testStrings = testStrings;
		this.symbols = symbols;
		this.start = start;
		this.acceptStates = acceptStates;
		dfaStates = new ArrayList<String>();
		dfaTransitions = new ArrayList<String>();
		dfaStates.add(nfaStates.get(0));
		testResults = new ArrayList<String>();
		
		this.generateDFAStates();
		this.generateDFATransitions();
	}

	/**
	 * Generates the DFA states based upon the NFA states.
	 */
	private void generateDFAStates()
	{
		String state = "";
		String test = "";
		char[] sortState;
		
		ArrayList<String> emptyTransition = new ArrayList<String>();
		
		/*
		 * Loops through all the nfaTransitions and adds all the states that a
		 * symbol can transition to into a single string.  If the symbol is
		 * the empty string, it is stored in a separate ArrayList to be dealt later.		
		 */
		for(int i = 0; i < nfaTransitions.size(); i++)
		{
			test = nfaTransitions.get(i).substring(0, 2);
			
			if(test.contains("l"))
			{
				emptyTransition.add(nfaTransitions.get(i));
				continue;
			}
			state += test.charAt(0);
			
			for(int j = i; j < nfaTransitions.size(); j++)
			{
				if(nfaTransitions.get(j).substring(0, 2).equals(test))
				{
					if(!state.contains(nfaTransitions.get(j).charAt(2) + ""))
					{
						state += nfaTransitions.get(j).charAt(2);
						i++;
					}					
				}
			}
			
			dfaStates.add(state);
			state = "";
		}
		
		/*
		 * Loops through all the transitions with an empty state and compares the
		 * current state value against the current dfaState current state.  If they
		 * are the same, the dfa state which has that starting value is removed from the
		 * ArrayList, then the transition state which the empty string would go to is
		 * appended to it.  Finally, it is reinserted into the ArrayList at the same
		 * position it previously had.
		 */
		for(int i = 0; i < emptyTransition.size(); i++)
		{
			for(int j = 0; j < dfaStates.size(); j++)
			{
				if(emptyTransition.get(i).charAt(0) == dfaStates.get(j).charAt(0))
				{
					String temp = dfaStates.get(j);
					temp += emptyTransition.get(i).charAt(2);
					sortState = temp.toCharArray();
					
					Arrays.sort(sortState);
					
					temp = new String(sortState);
					dfaStates.remove(j);
					dfaStates.add(j, temp);
				}
			}
		}
		
		dfaStates.sort(null);	
		for(int i = 0; i < dfaStates.size(); i++)
		{
			String temp = dfaStates.remove(i);
			char[] builder = temp.toCharArray();
			Arrays.sort(builder);
			temp = new String(builder);
			dfaStates.add(i, temp);
		}
	}
	
	/**
	 * Generates the DFA Transitions based on the DFA states and the
	 * NFA Transitions.
	 */
	private void generateDFATransitions()
	{
		String builtTrans = "";
		String transition = "";
		String state = "";
		String symbol = "";
		String currentState = "";
		String nfaTrans = "";
		boolean validTrans = false;
		
		// The following group of nested for loops is as vile a block of code
		// as anyone has ever written.  I could not think of a better way to do
		// it.  May Heaven have mercy on my soul.
		
		// Loop through each of the symbols
		for(int i = 0; i < symbols.size(); i++)
		{
			symbol = symbols.get(i);
			// Loop through each of the dfa states
			for(int j = 0; j < dfaStates.size(); j++)
			{
				currentState = dfaStates.get(j);
				transition = "";
				
				// Loop through each character of the current dfa state
				for(int k = 0; k < currentState.length(); k++)
				{
					// Check each dfa state character, combined with a symbol, against 
					// the nfa transitions
					for(int n = 0; n < nfaTransitions.size(); n++)
					{
						nfaTrans = nfaTransitions.get(n);
						
						if(nfaTrans.charAt(1) == 'l')
						{
							/*
							 *  if an 'l' is found in the nfa transition, all of the nfa transitions are checked
							 *  to see if the current state symbol, plus the transition symbol are contained in
							 *  any of the nfaTransitions.  If so, the transfer state is added to the transition
							 *  string being built, otherwise, the loop continues.
							 */
							if(nfaTrans.charAt(0) == currentState.charAt(k))
							{
								for(int trans = 0; trans < nfaTransitions.size(); trans++)
								{
									if(nfaTransitions.get(trans).contains(currentState.charAt(k) + "" + symbol))
									{
										validTrans = true;
										break;
									}
								}
								
								if(validTrans)
								{
									state = nfaTrans.charAt(2) + "";
									if(!transition.contains(state))
									{
										transition += state;
									}
									
									validTrans = false;
								}
								else
								{
									continue;
								}
								
							}
							else
							{
								continue;
							}							
						}
						else if(nfaTrans.substring(0, 2).equals(currentState.charAt(k) + "" + symbol))
						{
							state = nfaTrans.charAt(2) + "";
							if(!transition.contains(state))
							{
								transition += state;
							}
						}
					}
				}
				
				if(transition.length() >= 1)
				{
					char[] builder = transition.toCharArray();
					Arrays.sort(builder);
					transition = new String(builder);
					
					builtTrans = currentState + symbol + transition;
					if(!dfaTransitions.contains(builtTrans))
					{
						dfaTransitions.add(builtTrans);
					}
				}
			}
		}
	}
	
	/**
	 * Tests the strings stored in testStrings to see if they are valid.
	 * As the tests are run, the ArrayList testResults is populated with '-yes'
	 * or '-no' depending on the outcome of the test.
	 */
	public void runStrings()
	{
		for(int i = 0; i < testStrings.size(); i++)
		{
			if(runStrings(start, testStrings.get(i).substring(1, testStrings.get(i).length())))
			{
				testResults.add("-yes");
			}
			else
			{
				testResults.add("-no");
			}
		}
	}
	
	/**
	 * This private method is called by runStrings(), it is the one that is responsible
	 * for actually performing the tests on the string.  It accepts two string values, one
	 * for the current state, and one for the string to be checked.  A recursive call is made
	 * with the new state and the character at index 0 from the test string removed.
	 * @param state The current state of the DFA
	 * @param str The current string to be tested against the DFA
	 * @return true if the string is valid on the DFA, false otherwise
	 */
	private boolean runStrings(String state, String str)
	{
		if(str.equals(""))
		{
			boolean valid = false;
			
			for(int i = 0; i < acceptStates.size(); i++)
			{
				if(state.contains(acceptStates.get(i)))
				{
					valid = true;
					break;
				}
			}
			
			return valid;
		}
		
		String nextState = "";
		String transition = "";
		int subStrIndex = 0;
		
		for(int i = 0; i < dfaTransitions.size(); i++)
		{
			transition = dfaTransitions.get(i);
			
			if(transition.contains(state + str.charAt(0)) && transition.substring(0, state.length()).equals(state))
			{				
				while(transition.charAt(subStrIndex) < 'a' || transition.charAt(subStrIndex) > 'z')
				{
					subStrIndex++;
				}
				
				nextState = transition.substring(++subStrIndex, transition.length());
				
				break;
			}

		}
		
		if(nextState != "")
		{
			return runStrings(nextState, str.substring(1, str.length()));
		}
		else
		{
			return false;
		}		
	}
	
	/**
	 * Builds a string representation of the DFA states, transitions, and results
	 * of the tests, then returns the string.
	 * 
	 * @return String representation of the DFA and test results.
	 */
	public String toString()
	{
		String toString = "";
		
		toString += start + "\n";
		
		for(int i = 0; i < dfaStates.size(); i++)
		{
			if(!dfaStates.get(i).equals(start))
			{
				toString += dfaStates.get(i) + " ";
			}
		}
		
		toString += "\n\n";
		
		for(int i = 0; i < dfaTransitions.size(); i++)
		{
			toString += dfaTransitions.get(i) + "\n";
		}
		
		for(int i = 0; i < testResults.size(); i++)
		{
			toString += testResults.get(i) + "\n";
		}
		
		return toString;
	}
}
