/**
 * 
 * @author awoods
 * 
 * This class handles all the operations associated with
 * addition and subtraction that are found throughout the
 * program.
 */
public class NodeAddop extends Node {

	private String addop;

	/**
	 * Creates a NodeAddop object.
	 * 
	 * @param pos The program position counter at the
	 * 			  time of creation.
	 * @param addop The addition operator being used (+ or -)
	 */
	public NodeAddop(int pos, String addop) {
		this.pos=pos;
		this.addop=addop;
	}

	/**
	 * Performs the addition operation with the two values passed as
	 * parameters.
	 * @param o1 The first of two double numerical values to be operated on.
	 * @param o2 The second of two double numerical values to be operated on.
	 * @return The result of the add operation.
	 * @throws EvalException Thrown if an invalid add operator is given.
	 */
	public double op(double o1, double o2) throws EvalException {
		if (addop.equals("+"))
			return o1+o2;
		if (addop.equals("-"))
			return o1-o2;
		throw new EvalException(pos,"bogus addop: "+addop);
	}

}
