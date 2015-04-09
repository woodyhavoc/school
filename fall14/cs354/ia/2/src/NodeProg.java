
public class NodeProg extends Node 
{
	private NodeBlock block;
	
	public NodeProg(NodeBlock block)
	{
		this.block = block;
	}
	
	public double eval(Environment env) throws EvalException
	{
		return block.eval(env);
	}
}
