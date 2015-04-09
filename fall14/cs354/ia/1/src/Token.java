/**
 * 
 * @author awoods
 *
 * Class Token
 * 
 * This class is used to hold token an lexeme information
 * for each character in the interpreted program.
 */
public class Token {

	private String token;
	private String lexeme;

	/**
	 * Creates a Token object.
	 * 
	 * @param token String representation of a character
	 * @param lexeme String representation of a numerical value
	 */
	public Token(String token, String lexeme) {
		this.token=token;
		this.lexeme=lexeme;
	}

	/**
	 * This is an alternate constructor.  It accepts a
	 * String object, and then passes that object as both
	 * parameters to the main constructor.
	 * 
	 * @param token String representation of a character
	 */
	public Token(String token) {
		this(token,token);
	}

	/**
	 * Returns the token
	 * @return the token
	 */
	public String tok() { return token; } 
	
	/**
	 * Returns the lexeme
	 * @return the lexeme
	 */
	public String lex() { return lexeme; }

	/**
	 * Compares two Token objects and returns
	 * the result of the comparison.
	 * @param t the Token object to be compared against this Token object
	 * @return true if the two object are equal, false otherwise.
	 */
	public boolean equals(Token t) {
		return token.equals(t.token);
	}

	/**
	 * Returns a meaningful representation of the Token.
	 */
	public String toString() {
		return "<"+tok()+","+lex()+">";
	}

}
