/**
 * 
 * @author awoods
 * 
 * This class extends from NodeFact and is used to
 * handle all of the expressions that are found
 * throughout the program.
 */
public class NodeFactExpr extends NodeFact {

	private NodeExpr expr;

	/**
	 * Creates a NodeFactExpr object.
	 * 
	 * @param expr NodeExpr object to be stored in this
	 * 			   NodeFactExpr object.
	 */
	public NodeFactExpr(NodeExpr expr) {
		this.expr=expr;
	}

	/**
	 * Returns the results of evaluating this expression.
	 */
	public double eval(Environment env) throws EvalException {
		return expr.eval(env);
	}

}
