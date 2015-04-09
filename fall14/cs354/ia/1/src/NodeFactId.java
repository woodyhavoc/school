/**
 * 
 * @author awoods
 * 
 * This class extends NodeFact and is used to 
 * keep track of all the id's that are found
 * throughout the program.
 */
public class NodeFactId extends NodeFact {

	private String id;

	/**
	 * Creates a NodeFactId object.
	 * 
	 * @param pos The current position number of the program.
	 * @param id The id found at the current position.
	 */
	public NodeFactId(int pos, String id) {
		this.pos=pos;
		this.id=id;
	}

	/**
	 * Gets the value that is associated with the id passed as a parameter,
	 * and returns this value.
	 */
	public double eval(Environment env) throws EvalException {
		return env.get(pos,id);
	}

}
