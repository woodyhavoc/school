//TODO: Verify this class works
public class NodeRelop extends Node
{
	private String relop;
	
	public NodeRelop(String relop)
	{
		this.relop = relop;
	}
	
	public String toString()
	{
		return relop;
	}
}
