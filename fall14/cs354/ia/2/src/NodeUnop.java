/**
 * 
 * @author awoods
 *
 * This class extends NodeFact and is used to handle any
 * situations in which a unary minus operator arises.
 */
public class NodeUnop extends NodeFact
{
	NodeFact fact;

	/**
	 * Creates a NodeUnop object.
	 * 
	 * @param pos The program position
	 * @param fact NodeFact object because one of these is required to follow
	 *             a unary minus operator
	 */
	public NodeUnop(int pos, NodeFact fact)
	{
		this.fact = fact;
	}
	
	/**
	 * Returns the result of eval() on env, multiplied by -1
	 */
	public double eval(Environment env) throws EvalException
	{
		return -1 * fact.eval(env);
	}
}
