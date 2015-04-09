//TODO: Verify this class works
public class NodeStmtBeginEnd extends NodeStmt
{
	private NodeBlock block;
	
	public NodeStmtBeginEnd(NodeBlock block)
	{
		this.block = block;
	}
	
	public double eval(Environment env) throws EvalException
	{
		return this.block.eval(env);
	}
}
