//TODO: Verify this class works
public class NodeStmtWrite extends NodeStmt
{
	private NodeExpr expr;
	
	public NodeStmtWrite(NodeExpr expr)
	{
		this.expr = expr;
	}
	
	//Need to determine what should be returned here
	public double eval(Environment env) throws EvalException
	{
		System.out.println(expr.eval(env));
		
		return 1.0;
	}
}
