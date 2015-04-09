/**
 * 
 * @author awoods
 * 
 * This class handles all exceptions that may arise
 * during the interpretation of the program.
 */
@SuppressWarnings("serial")
public class EvalException extends Exception {

    private int pos;
    private String msg;

    /**
     * Creates an EvalException object with the position counter
     * of the program, and a message about the error.
     * 
     * @param pos The position counter of the program.
     * @param msg A message about the error that has occurred.
     */
    public EvalException(int pos, String msg) {
	this.pos=pos;
	this.msg=msg;
    }

    /**
     * Returns a meaningful string representation of the exception.
     */
    public String toString() {
	return "eval error"
	    +", pos="+pos
	    +", "+msg;
    }

}
