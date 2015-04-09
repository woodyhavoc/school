/**
 * 
 * @author awoods
 * 
 * This class extends NodeFact and handles all
 * the numbers found throughout the program.
 */
public class NodeFactNum extends NodeFact {

	private String num;

	/**
	 * Creates a NodeFactNum object that holds a
	 * String representation of a number.
	 * 
	 * @param num The String representation of the
	 * 			  number to be stored in this object.
	 */
	public NodeFactNum(String num) {
		this.num=num;
	}

	/**
	 * Parses the String representation of a number, and returns
	 * it as a double precision number.
	 */
	public double eval(Environment env) throws EvalException {
		return Double.parseDouble(num);
	}

}
