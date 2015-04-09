/**
 * 
 * @author awoods
 * 
 * This class is used to handle all of the possible
 * exceptions that may be thrown while the interpreter
 * is executing.
 */
@SuppressWarnings("serial")
public class SyntaxException extends Exception {

	private int pos;
	private Token expected;
	private Token found;

	/**
	 * Creates a SyntaxException object.
	 * 
	 * @param pos The program position.
	 * @param expected The expected Token at the position.
	 * @param found The Token that was found at the position.
	 */
	public SyntaxException(int pos, Token expected, Token found) {
		this.pos=pos;
		this.expected=expected;
		this.found=found;
	}

	/**
	 * Returns a meaningful representation of the the SyntaxException.
	 */
	public String toString() {
		return "syntax error"
				+", pos="+pos
				+", expected="+expected
				+", found="+found;
	}

}
