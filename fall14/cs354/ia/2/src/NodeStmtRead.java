//TODO: Verify this class works
public class NodeStmtRead extends NodeStmt
{
	private String val;
	private String id;
	private java.util.Scanner scan;
	
	
	public NodeStmtRead(String id)
	{
		this.id = id;
		scan = new java.util.Scanner(System.in);
		val = scan.next();
	}
	
	public double eval(Environment env) throws EvalException
	{
		return env.put(id, Double.parseDouble(val));
	}
}
