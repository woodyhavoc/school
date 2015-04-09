/**
 * 
 * @author awoods
 * 
 * This class extends Node, and is used to handle
 * all of the multiplication operators (* and /) found
 * throughout the program.
 */
public class NodeMulop extends Node {

	private String mulop;

	/**
	 * Creates a NodeMulop object and stores the current
	 * program position along with the multiplication operator
	 * found at that location.
	 *  
	 * @param pos The current position of the program.
	 * @param mulop The multiplication operator to be used in evaluating
	 * 				an expression.
	 */
	public NodeMulop(int pos, String mulop) {
		this.pos=pos;
		this.mulop=mulop;
	}

	/**
	 * Performs a multiplication operation on two numbers and returns the result.
	 * 
	 * @param o1 The first of two numbers to be used in the multiplication operation.
	 * @param o2 The second of two numbers to be used in the multiplication operation.
	 * @return The result of the calculation.
	 * @throws EvalException Thrown if an illegal multiplication operator is given.
	 */
	public double op(double o1, double o2) throws EvalException {
		if (mulop.equals("*"))
			return o1*o2;
		if (mulop.equals("/"))
			return o1/o2;
		throw new EvalException(pos,"bogus mulop: "+mulop);
	}

}
