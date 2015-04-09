/**
 * 
 * @author awoods
 *
 */
public abstract class Node {

	protected int pos=0;

	/**
	 * Throws an EvalException, fuller implementations of this
	 * method are contained in child classes.
	 * 
	 * @param env Environment object to be evaluated
	 * @return Returns a double value, to be determined by
	 * 		   individual child classes.
	 * @throws EvalException
	 */
	public double eval(Environment env) throws EvalException {
		throw new EvalException(pos,"cannot eval() node!");
	}

}
