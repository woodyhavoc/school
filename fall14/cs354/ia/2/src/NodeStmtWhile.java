//TODO: Verify this class works
public class NodeStmtWhile extends NodeStmt
{
	private NodeBoolExpr bool;
	private NodeStmt stmt;
	
	public NodeStmtWhile(NodeBoolExpr bool, NodeStmt stmt)
	{
		this.bool = bool;
		this.stmt = stmt;
	}
	
	//Need to determine what should actually be returned
	public double eval(Environment env) throws EvalException
	{
		while(bool.eval(env) == 1.0)
		{
			System.out.println(stmt.eval(env));
		}
		
		return 1.0;
	}
}
