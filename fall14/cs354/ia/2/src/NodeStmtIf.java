//TODO: Verify this class works
public class NodeStmtIf extends NodeStmt
{
	private NodeBoolExpr bool;
	private NodeStmt stmt;
	
	public NodeStmtIf(NodeBoolExpr bool, NodeStmt stmt)
	{
		this.bool = bool;
		this.stmt = stmt;
	}
	
	public double eval(Environment env) throws EvalException
	{		
		if(this.bool.eval(env) == 1.0)
		{
			return this.stmt.eval(env);
		}
		
		return 0.0;
	}
}
