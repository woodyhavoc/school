import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * 
 * @author awoods
 * 
 * This class reads NFA information from a file and stores it into
 * various ArrayLists.  These pieces of information include the
 * states, transitions, and test strings.  All of this information
 * is passed to a DFA object, which generates a DFA and runs the
 * tests.
 */
public class FiniteAutomata
{
	private DFA dfa;
	private ArrayList<String> states;
	private ArrayList<String> transitions;
	private ArrayList<String> testStrings;
	private ArrayList<String> symbols;
	private ArrayList<String> acceptStates;
	private String start;
	
	/**
	 * Constructor, initializes all the private
	 * instance variables.
	 */
	public FiniteAutomata()
	{
		dfa = null;
		states = new ArrayList<String>();
		transitions = new ArrayList<String>();
		testStrings = new ArrayList<String>();
		symbols = new ArrayList<String>();
		acceptStates = new ArrayList<String>();
	}
	
	/**
	 * Accepts the name of a file, and processes the file
	 * for NFA information including the start state, ending
	 * states, transitions, symbols, and test strings.
	 * 
	 * @param fileName The name of the file to be processed
	 */
	public void generateDFA(String fileName)
	{
		String line = null;
		Scanner scan = null;
		int lineCount = 0;
		
		try
		{
			scan = new Scanner(new File(fileName));
		}
		catch(FileNotFoundException e)
		{
			e.printStackTrace();
		}
		
		while(scan.hasNextLine())
		{
			lineCount++;
			line = scan.nextLine();
			
			if(lineCount == 1)
			{
				start = line;
				states.add(start);
				continue;
			}
			
			if(lineCount == 2)
			{
				for(int i = 0; i < line.length(); i++)
				{
					if(line.charAt(i) != ' ')
					{
						states.add(line.charAt(i) + "");
						acceptStates.add(line.charAt(i) + "");
					}
				}
				continue;
			}
			
			if(line.length() == 1)
			{
				states.add(line);
			}
			else if(line.length() == 3 && line.charAt(0) != '-')
			{
				transitions.add(line);
				if(line.charAt(1) != 'l')
				{
					symbols.add(line.charAt(1) + "");
				}				
			}
			else if(line.charAt(0) == '-')
			{
				testStrings.add(line);
			}
		}
		
		transitions.sort(null);
			
		dfa = new DFA(start, acceptStates, states, transitions, testStrings, symbols);
	}
	
	/**
	 * Returns the ArrayList states.
	 * @return ArrayList states
	 */
	public ArrayList<String> getStates()
	{
		return states;
	}
	
	/**
	 * Returns the ArrayList transitions
	 * @return transitions
	 */
	public ArrayList<String> getTransitions()
	{
		return transitions;
	}
	
	/**
	 * Returns the ArrayList testStrings
	 * @return testStrings
	 */
	public ArrayList<String> getTestStrings()
	{
		return testStrings;
	}
	
	/**
	 * Tests the strings to see if they are valid for the
	 * DFA.  Does this by calling the DFA method runStrings.
	 */
	public void runTestStrings()
	{
		dfa.runStrings();
	}
	
	/**
	 * Returns a String representation of the DFA including
	 * states, transitions, and results of the string
	 * tests.  The String that is returned is obtained by calling
	 * the toString() method of the DFA object.
	 */
	public String toString()
	{
		return dfa.toString();
	}
}
