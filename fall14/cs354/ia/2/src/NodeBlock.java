//TODO: Verify this class works
public class NodeBlock extends Node
{
	private NodeStmt stmt;
	private NodeBlock block;
	
	public NodeBlock(NodeStmt stmt, NodeBlock block)
	{
		this.stmt = stmt;
		this.block = block;
	}
	
	public NodeBlock(NodeStmt stmt)
	{
		this.stmt = stmt;
		this.block = null;
	}
	
	//This is probably wrong
	public void append(NodeBlock block)
	{
		if(this.block == null)
		{
			this.block = block;
			block.block = null;
		}
		else
		{
			this.block.append(block);
		}
	}
	
	//TODO: fix this shit
	public double eval(Environment env) throws EvalException
	{
		double eval = stmt.eval(env);
		
		if(block != null)
		{
			return block.eval(env);
		}
		
		return eval;
	}
}
