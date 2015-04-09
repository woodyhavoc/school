import java.util.ArrayList;

/**
 * 
 * @author awoods
 *
 * This class maintains a list of the variables that
 * are used throughout the program, and their repsective
 * values.
 */
public class Environment
{
	ArrayList<Token> tokens = new ArrayList<Token>();

	/**
	 * Creates a Token object that holds the given parameters.
	 * The Token is then stored in a HashSet.
	 * 
	 * @param var String representation of a variable
	 * @param val Double precision numerical value assigned to the variable.
	 * 
	 * @return The value assigned to the variable.
	 */
    public double put(String var, double val)
    {
    	tokens.add(new Token(var, val+""));
    	return val; 
    }
        
    /**
     * Checks the Token list to see if there is a token that
     * has the same var as the one passed as a parameter.
     * 
     * @param pos Used for EvalException
     * @param var The variable to be looked for in the Token Array List
     * @return The parsed double value of the token if there is a matching var.
     * @throws EvalException
     */
    public double get(int pos, String var) throws EvalException
    {
    	for(Token tok: tokens)
    	{
    		if(tok.tok().equals(var))
    		{
    			return Double.parseDouble(tok.lex());
    		}
    	}
    	return 0;
    }

}
