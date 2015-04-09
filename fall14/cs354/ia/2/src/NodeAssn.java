/**
 * 
 * @author awoods
 *
 * This class handles any assignment statements that are made
 * throughout the program.
 */
public class NodeAssn extends Node {

	private String id;
	private NodeExpr expr;
	
	/**
	 * Creates a NodeAssn object.
	 * 
	 * @param id The id of the assignment operator
	 * @param expr NodeExpr object containing the expression to be
	 * 			   evalutated.
	 */
	public NodeAssn(String id, NodeExpr expr) {
		this.id=id;
		this.expr=expr;
	}

	/**
	 * Evaluates the passed Environment variable, and returns
	 * the result of the evaluation.
	 * 
	 * @param env The Environment variable to be evaluated.
	 * 
	 * @return The result of the evaluation.
	 */
	public double eval(Environment env) throws EvalException {
		return env.put(id,expr.eval(env));
	}

}
