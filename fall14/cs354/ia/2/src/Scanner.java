import java.util.*;

/**
 * 
 * @author awoods
 *
 */
public class Scanner
{
	private String program;
	private int pos;
	private Token token;
	private char comment;
	private Set<String> whitespace=new HashSet<String>();
	private Set<String> digits=new HashSet<String>();
	private Set<String> letters=new HashSet<String>();
	private Set<String> legits=new HashSet<String>();
	private Set<String> keywords=new HashSet<String>();
	private Set<String> operators=new HashSet<String>();
	private Set<String> comments = new HashSet<String>();

	/**
	 * This method fills a Set<String> with all the characters
	 * in the range between lo and hi, based on their ASCII value.
	 * 
	 * @param s The Set<String> to be filled
	 * @param lo The starting character
	 * @param hi The ending character
	 */
	private void fill(Set<String> s, char lo, char hi)
	{
		for (char c=lo; c<=hi; c++)
			s.add(c+"");
	}    

	/**
	 * This method adds the characters that are accepted
	 * as whitespace to a HashSet.
	 * 
	 * @param s The HashSet that will hold the whitespace characters.
	 */
	private void initWhitespace(Set<String> s)
	{
		s.add(" ");
		s.add("\n");
		s.add("\t");
	}

	/**
	 * This method fills a HashSet with the characters that are accepted as digits.
	 * 
	 * @param s The HashSet that will be filled with the digits.
	 */
	private void initDigits(Set<String> s)
	{
		fill(s,'0','9');
		s.add(".");
	}

	/**
	 * This method fills a HashSet with the characters that are accepted
	 * as letters.
	 * 
	 * @param s The HashSet that will be filled with letters.
	 */
	private void initLetters(Set<String> s)
	{
		fill(s,'A','Z');
		fill(s,'a','z');
	}

	/**
	 * This method adds the HashSets that are considered legitimate
	 * to another HashSet.
	 * 
	 * @param s The HashSet that will hold the HashSets of legitimate
	 * 			characters.
	 */
	private void initLegits(Set<String> s)
	{
		s.addAll(letters);
		s.addAll(digits);
	}

	/**
	 * This method adds the characters that are accepted as operators
	 * to a Hash Set.
	 * 
	 * @param s The HashSet to be filled with the accepted operators.
	 */
	private void initOperators(Set<String> s)
	{
		s.add("=");
		s.add("+");
		s.add("-");
		s.add("*");
		s.add("/");
		s.add("(");
		s.add(")");
		s.add(";");
		s.add("<");
		s.add("<=");
		s.add(">");
		s.add(">=");
		s.add("<>");
		s.add("==");
	}

	/**
	 * This method adds the characters that are accepted as comment
	 * delimiters to a HashSet.
	 * 
	 * @param s The HashSet to be filled with the accepted comment
	 * 			delimiters.
	 */
	private void initComments(Set<String> s)
	{
		s.add("^");
	}

	/**
	 * This method fills a HashSet with the words that are accepted
	 * as keywords.
	 * 
	 * @param s The HashSet to be filled with the accepted keywords.
	 */
	private void initKeywords(Set<String> s)
	{
		s.add("rd");
		s.add("wr");
		s.add("if");
		s.add("then");
		s.add("else");
		s.add("while");
		s.add("do");
		s.add("begin");
		s.add("end");
	}

	/**
	 * Constructs a Scanner object, and initializes all of the
	 * associated HashSets.
	 * 
	 * @param program A string representation of the program
	 * q			  to be scanned.
	 */
	public Scanner(String program)
	{
		this.program=program;
		pos=0;
		token=null;
		comment='^';
		initWhitespace(whitespace);
		initDigits(digits);
		initLetters(letters);
		initLegits(legits);
		initKeywords(keywords);
		initOperators(operators);
		initComments(comments);
	}

	/**
	 * This method is used to determine if the scanning operation
	 * has been completed.
	 * 
	 * @return true if the program has been completely scanned, false otherwise.
	 */
	public boolean done()
	{
		return pos>=program.length();
	}

	/**
	 * This method advances the position counter of the program
	 * by 1, for every sequential character in the program that
	 * is also contained in the Set<String> passed as the parameter.
	 * 
	 * @param s The Set<String> in which the character at the position 'pos'
	 * 			will be looked for.
	 */
	private void many(Set<String> s)
	{
		while (!done() && s.contains(program.charAt(pos)+""))
		{
			pos++;
		}
	}

	/**
	 * This method is used to ignore comments.
	 * It accepts a char as a parameter.  It then checks the character
	 * at the current program position.  If the two are equal
	 * to each other, the position is incremented by one, and
	 * continues to increment as long as the character at the
	 * current program position does not match the parameter.
	 * Finally, once there is a second match, the program position
	 * is incremented one more time.
	 * @param c The character to be checked that possibly signals
	 * 			the beginning of a comment.
	 */
	private void past(char c)
	{
		if(!done() && c == program.charAt(pos))
		{
			pos++;
			while (!done() && c!=program.charAt(pos))
			{
				pos++;
			}

			if(!done() && c==program.charAt(pos))
			{
				pos++;
			}
		}
	}

	/**
	 * This method extracts a number from the program by
	 * checking if a string of digits exists at the current
	 * program position.  While it is checking, it is iterating
	 * the program counter.  Finally, a new Token object is made
	 * with the number contained in the substring between the
	 * program counter start and finish positions.
	 */
	private void nextNumber()
	{
		int old=pos;
		many(digits);
		token=new Token("num",program.substring(old,pos));
	}

	/**
	 * This method extracts a keyword from the program by
	 * checking if a string of letters and legits exists at the current
	 * program position.  While it is checking, it is iterating
	 * the program counter.  Finally, a new Token object is made
	 * with the keyword contained in the substring between the
	 * program counter start and finish positions.
	 */
	private void nextKwId()
	{
		int old=pos;
		many(letters);
		many(legits);
		String lexeme=program.substring(old,pos);
		token=new Token((keywords.contains(lexeme) ? lexeme : "id"),lexeme);
	}

	/**
	 * This method is used to extract an operator from the program.
	 * First it checks for any two character operators.  If one is
	 * found, a new Token is made and stored in the token variable.
	 * If not, it then checks for one character operations, makes a
	 * Token out of the operation, and stores that in the token variable.
	 */
	private void nextOp()
	{
		int old=pos;
		
		pos=old+3;
		if(!done())
		{
			String lexeme=program.substring(old,pos);
			if(operators.contains(lexeme))
			{
				token=new Token(lexeme);
				return;
			}
		}
		
		pos=old+2;
		if(!done())
		{
			String lexeme=program.substring(old,pos);
			if(operators.contains(lexeme))
			{
				token=new Token(lexeme);
				return;
			}
		}
		pos=old+1;
		String lexeme=program.substring(old,pos);
		token=new Token(lexeme);
	}

	/**
	 * Checks the character at the current position, and calls the appropriate
	 * method based upon what that character is.
	 * 
	 * @return false if the program has completed execution, true otherwise.
	 */
	public boolean next()
	{
		if(done())
			return false;
		many(whitespace);
		past(comment);
		String c=program.charAt(pos)+"";

		if(digits.contains(c))
		{
			nextNumber();
		}
		else if(letters.contains(c))
		{
			nextKwId();
		}			
		else if(operators.contains(c))
		{
			nextOp();
		}			
		else
		{
			System.err.println("illegal character at position "+pos);
			pos++;
			return next();
		}
		return true;
	}

	/**
	 * Checks to see if the passed parameter equals the current
	 * Token at the program position.
	 * @param t Token object to be tested.
	 * @throws SyntaxException Thrown if t does not match the
	 *                         current Token at the program
	 *                         position.
	 */
	public void match(Token t) throws SyntaxException
	{
		if(!t.equals(curr()))
		{
			throw new SyntaxException(pos,t,curr());
		}
			
		next();
	}

	/**
	 * Returns the current token, based on the program position.
	 * @return the current token, based on the program position.
	 * @throws SyntaxException Thrown if token is null.
	 */
	public Token curr() throws SyntaxException
	{
		if(token==null)
		{
			throw new SyntaxException(pos,new Token("ANY"),new Token("EMPTY"));
		}
			
		return token;
	}

	/**
	 * Gets the current program position.
	 * @return the current program position.
	 */
	public int pos()
	{
		return pos;
	}

	/**
	 * Main method, creates a scanner with args[0]
	 * as a parameter.  Next it checks every character
	 * in the program and prints it out.
	 * 
	 * @param args Typically command line arguments
	 */
	public static void main(String[] args)
	{
		try
		{
			Scanner scanner=new Scanner(args[0]);
			
			while (scanner.next())
			{
				System.out.println(scanner.curr());
			}				
		}
		catch(SyntaxException e)
		{
			System.err.println(e);
		}
	}
}
