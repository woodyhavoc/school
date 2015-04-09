/**
 * 
 * @author awoods
 * 
 * This class is used to build expressions from the
 * program, which will later be evaluated.
 */
public class NodeExpr extends Node {

	private NodeTerm term;
	private NodeAddop addop;
	private NodeExpr expr;

	/**
	 * Creates a NodeExpr object.
	 * 
	 * @param term NodeTerm object which can take the form of 'fact mulop term' or just 'fact'
	 * @param addop NodeAddop object which represents either addition or subtraction.
	 * @param expr NodeExpr object which can take the form of 'term addop expr' or just 'term'
	 */
	public NodeExpr(NodeTerm term, NodeAddop addop, NodeExpr expr) {
		this.term=term;
		this.addop=addop;
		this.expr=expr;
	}

	/**
	 * If the NodeExpr object is null, its values are set to those
	 * of the passed parameter, otherwise the passed parameter is
	 * appended to the end of the NodeExpr object.
	 * 
	 * @param expr NodeExpr object either to be copied or appended to
	 * 			   this NodeExpr object.
	 */
	public void append(NodeExpr expr) {
		if (this.expr==null) {
			this.addop=expr.addop;
			this.expr=expr;
			expr.addop=null;
		} else
			this.expr.append(expr);
	}

	/**
	 * Evaluates the passed Environment object.  If this object's
	 * NodeExpr object is null, the term object calls eval on the
	 * env.  Otherwise, addop is called on the eval methods of both
	 * expr and term.
	 * 
	 * @return The results of the evaluation as double precision number.
	 */
	public double eval(Environment env) throws EvalException {
		return expr==null
				? term.eval(env)
						: addop.op(expr.eval(env),term.eval(env));
	}

}
