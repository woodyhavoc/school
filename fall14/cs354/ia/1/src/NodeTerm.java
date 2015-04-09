/**
 * 
 * @author awoods
 *
 * This class extends Node and keeps track
 * of all the terms found throughout the program.
 */
public class NodeTerm extends Node {

	private NodeFact fact;
	private NodeMulop mulop;
	private NodeTerm term;

	/**
	 * Creates a NodeTerm object with format 'fact mulop term'
	 * 
	 * @param fact NodeFact object to be stored in this NodeTerm object
	 * @param mulop NodeMulop object to be stored in this NodeTerm object
	 * @param term NodeTerm object to be stored in this NodeTerm object
	 */
	public NodeTerm(NodeFact fact, NodeMulop mulop, NodeTerm term) {
		this.fact=fact;
		this.mulop=mulop;
		this.term=term;
	}

	/**
	 * If this object's NodeTerm is null, it sets the value of this mulop
	 * to that of the passed NodeTerm parameter.  Next it sets it's
	 * NodeTerm object equal to the passed parameter.  If this object's
	 * NodeTerm is not null, it simply appends the passed parameter to
	 * itself.
	 * 
	 * @param term NodeTerm object to either have information extracted,
	 * 			   or be appended to this NodeTerm object.
	 */
	public void append(NodeTerm term) {
		if (this.term==null) {
			this.mulop=term.mulop;
			this.term=term;
			term.mulop=null;
		} else
			this.term.append(term);
	}

	/**
	 * If the NodeTerm object is null, the result of the evaluation
	 * of the NodeFact object are returned.  Otherwise the results
	 * of the multiplication operation on the NodeTerm and NodeFact
	 * objects are returned.
	 */
	public double eval(Environment env) throws EvalException {
		return term==null
				? fact.eval(env)
						: mulop.op(term.eval(env),fact.eval(env));
	}

}
