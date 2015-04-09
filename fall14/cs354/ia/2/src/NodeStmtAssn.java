
public class NodeStmtAssn extends NodeStmt
{
	private NodeAssn assn;

	/**
	 * Creates a NodeStmt object which stores
	 * a NodeAssn object.
	 * 
	 * @param assn NodeAssn object to be stored in this
	 * 			   NodeStmt object.
	 */
	public NodeStmtAssn(NodeAssn assn) {
		this.assn=assn;
	}

	/**
	 * Returns the results of the evaluation performed on the
	 * Environment object passed as a parameter.
	 */
	public double eval(Environment env) throws EvalException {
		return assn.eval(env);
	}
}
