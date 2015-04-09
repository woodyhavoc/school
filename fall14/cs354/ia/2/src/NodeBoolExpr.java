//TODO: Verify this class works
public class NodeBoolExpr extends Node
{
	private NodeExpr expr1, expr2;
	private NodeRelop relop;
	
	private final double TRUE = 1.0, FALSE = 0.0;
	
	public NodeBoolExpr(NodeExpr expr1, NodeRelop relop, NodeExpr expr2)
	{
		this.expr1 = expr1;
		this.relop = relop;
		this.expr2 = expr2;
	}
	
	public double eval(Environment env) throws EvalException
	{
		double num1 = expr1.eval(env);
		double num2 = expr2.eval(env);
		double ret = FALSE;
		
		switch(this.relop.toString())
		{
		case "<":  if(num1 < num2) ret = TRUE;
				   break;
		
		case "<=": if(num1 <= num2) ret = TRUE;
				   break;
		
		case ">":  if(num1 > num2) ret = TRUE;
		           break;
		
		case ">=": if(num1 >= num2) ret = TRUE;
				   break;
				   
		case "<>": if(num1 != num2) ret = TRUE;
		           break;
		
		case "==": if(num1 == num2) ret = TRUE;
		           break;
		           
		           //This line should never be reached
		default:   throw new EvalException(pos, "Invalid Boolean operator");
		}
		
		return ret;
	}
}
