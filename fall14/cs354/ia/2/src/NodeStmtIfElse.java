//TODO: Verify this class works
public class NodeStmtIfElse extends NodeStmt
{
	private NodeBoolExpr bool;
	private NodeStmt stmt1, stmt2;
	
	public NodeStmtIfElse(NodeBoolExpr bool, NodeStmt stmt1, NodeStmt stmt2)
	{
		this.bool = bool;
		this.stmt1 = stmt1;
		this.stmt2 = stmt2;
	}
	
	public double eval(Environment env) throws EvalException
	{
		if(bool.eval(env) == 1.0)
		{
			return stmt1.eval(env);
		}
		else
		{
			return stmt2.eval(env);
		}
	}
}
