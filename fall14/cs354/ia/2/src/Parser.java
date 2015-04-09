
/**
 * 
 * @author awoods
 * 
 * Class Parser
 * 
 * Responsible for incrementing through the program
 * one character at a time while scanning to determine
 * what sort of object each character most closely resembles
 * based on the grammar of the language.
 */
public class Parser
{

	private Scanner scanner;

	/**
	 * Makes a new Token with the passed parameter,
	 * and checks if the String at the current position counter
	 * equals the parameter.
	 * 
	 * @param s String to be compared against the string at
	 * 			the current program position.
	 * @throws SyntaxException Thrown when the passed String object
	 * 						   is not a match.
	 */
	private void match(String s) throws SyntaxException {
		scanner.match(new Token(s));
	}

	/**
	 * Returns the Token at the current program position.
	 * 
	 * @return the Token at the current program position.
	 * @throws SyntaxException Thrown when the Token at the
	 * 						   current position is inaccessible.
	 */
	private Token curr() throws SyntaxException {
		return scanner.curr();
	}

	/**
	 * Returns the current position index value of the program.
	 * @return The current position index value of the program.
	 */
	private int pos() {
		return scanner.pos();
	}

	/**
	 * Parses the Token at the current position to determine which kind
	 * of multiplication operator is being used.
	 * 
	 * @return a new NodeMulop object containing either '*' or '/'
	 * @throws SyntaxException Thrown if an invalid multiplication operator
	 * 						   is given.
	 */
	private NodeMulop parseMulop() throws SyntaxException {
		if (curr().equals(new Token("*"))) {
			match("*");
			return new NodeMulop(pos(),"*");
		}
		if (curr().equals(new Token("/"))) {
			match("/");
			return new NodeMulop(pos(),"/");
		}
		return null;
	}

	/**
	 * Parses the Token at the current position to determine which kind of
	 * addition operator is being used.
	 * 
	 * @return a new NodeAddop object containing either '+' or '-'
	 * @throws SyntaxException Thrown if an invalid addition operator is given.
	 */
	private NodeAddop parseAddop() throws SyntaxException {
		if (curr().equals(new Token("+"))) {
			match("+");
			return new NodeAddop(pos(),"+");
		}
		if (curr().equals(new Token("-"))) {
			match("-");
			return new NodeAddop(pos(),"-");
		}
		return null;
	}

	/**
	 * Parses the Token at the current position to determine which kind of
	 * NodeFact is being used.
	 * 
	 * @return a new NodeFactExpr if the Token is an expression,
	 * 		   a new NodeFactId if the Token is an id,
	 * 		   a new NodeFactNum if the Token is a number.
	 * @throws SyntaxException Thrown if an invalid Token is found at the
	 * 						   current position.
	 */
	private NodeFact parseFact() throws SyntaxException {
		if (curr().equals(new Token("("))) {
			match("(");
			NodeExpr expr=parseExpr();
			match(")");
			return new NodeFactExpr(expr);
		}
		if (curr().equals(new Token("id"))) {
			Token id=curr();
			match("id");
			return new NodeFactId(pos(),id.lex());
		}
		if(curr().equals(new Token("-")))
		{
			match("-");
			//scanner.next();
			return new NodeUnop(pos(), parseFact());
		}
		Token num=curr();
		match("num");
		return new NodeFactNum(num.lex());
	}

	/**
	 * Parses a term to determine if a fact or a mulop is at the
	 * current position.
	 * 
	 * @return a NodeTerm object containing a NodeFact if parseMulop()
	 * 		   returns null, otherwise appends a new NodeTerm object with
	 * 		   a NodeFact and NodeMulop to this NodeTerm, then returns.
	 * @throws SyntaxException Thrown if an invalid term is parsed.
	 */
	private NodeTerm parseTerm() throws SyntaxException {
		NodeFact fact=parseFact();
		NodeMulop mulop=parseMulop();
		if (mulop==null)
			return new NodeTerm(fact,null,null);
		NodeTerm term=parseTerm();
		term.append(new NodeTerm(fact,mulop,null));
		return term;
	}

	/**
	 * Parses an expression to determine if a term or addop is at the current
	 * position.
	 * 
	 * @return a NodeExpr object containing a NodeTerm if parseAddop()
	 * 		   returns null, otherwise appends a new NodeExpr object with
	 * 		   a NodeTerm and NodeAddop to this NodeExpr, then returns.
	 * @throws SyntaxException Thrown if an invalid expression is parsed.
	 */
	private NodeExpr parseExpr() throws SyntaxException {
		NodeTerm term=parseTerm();
		NodeAddop addop=parseAddop();
		if (addop==null)
			return new NodeExpr(term,null,null);
		NodeExpr expr=parseExpr();
		expr.append(new NodeExpr(term,addop,null));
		return expr;
	}

	/**
	 * Stores the current token in a Token variable, then calls
	 * match on 'id' and '='.  Next it stores the result of parseExpr()
	 * in a NodeExpr variable.  Finally creates a new NodeAssn object,
	 * with the number associated with the id as well as the NodeExpr object.
	 * 
	 * @return The newly created NodeAssn object.
	 * @throws SyntaxException Thrown if the Token at the current position
	 * 						   is invalid.
	 */
	private NodeAssn parseAssn() throws SyntaxException {
		Token id=curr();
		match("id");
		match("=");
		NodeExpr expr=parseExpr();
		NodeAssn assn=new NodeAssn(id.lex(),expr);
		return assn;
	}

	/**
	 * Stores the result of parseAssn() in a NodeAssn variable.
	 * Next it calls match on ';'.  Finally makes a new NodeStmt
	 * object which contains the NodeAssn object.
	 * 
	 * @return The newly created NodeStmt object.
	 * @throws SyntaxException Thrown if match() fails.
	 */
	private NodeStmt parseStmt() throws SyntaxException {
		
		NodeStmt stmt = null;
		
		if(curr().equals(new Token("rd")))
		{
			match("rd");
			stmt = parseStmtRead();
		}
		else if(curr().equals(new Token("wr")))
		{
			match("wr");
			stmt = new NodeStmtWrite(parseExpr());
		}
		else if(curr().equals(new Token("if")))
		{
			match("if");
			NodeBoolExpr bool = parseBoolExpr();
			match("then");
			NodeStmt parse1 = parseStmt();
			
			if(curr().equals(new Token("else")))
			{
				match("else");
				NodeStmt parse2 = parseStmt();
				stmt = new NodeStmtIfElse(bool, parse1, parse2);
			}
			else
			{
				stmt = new NodeStmtIf(bool, parse1);
			}			
		}
		else if(curr().equals(new Token("while")))
		{
			match("while");
			NodeBoolExpr bool = parseBoolExpr();
			match("do");
			NodeStmt parse = parseStmt();
			stmt = new NodeStmtWhile(bool, parse);
		}
		else if(curr().equals(new Token("begin")))
		{
			match("begin");
			stmt = new NodeStmtBeginEnd(parseBlock());
			match("end");
		}
		else
		{
			stmt = new NodeStmtAssn(parseAssn());
		}
		return stmt;
	}

	/**
	 * Creates a scanner using the provided parameter.  Next it
	 * parses the program.  Finally it returns the result of the
	 * computation.
	 * 
	 * @param program String representation of the program to
	 * 				  be parsed.
	 * @return The results of parseStmt()
	 * @throws SyntaxException Thrown if invalid program is provided
	 * 						   as a parameter.
	 */
	public Node parse(String program) throws SyntaxException {
		scanner=new Scanner(program);		scanner.next();
		return parseProg();
	}
	
	//TODO: Verify this method works
	public NodeBlock parseBlock() throws SyntaxException
	{
		NodeStmt stmt = parseStmt();
		
		if(curr().equals(new Token(";")))
		{
			match(";");
			return new NodeBlock(stmt, parseBlock());
		}
		
		return new NodeBlock(stmt);
	}
	
	//TODO: Verify this method works
	public NodeBoolExpr parseBoolExpr() throws SyntaxException
	{	
		if(curr().equals(new Token("==")))
		{
			match("==");
		}
		return new NodeBoolExpr(parseExpr(), parseRelop(), parseExpr());
	}
	
	public NodeProg parseProg() throws SyntaxException
	{
		return new NodeProg(parseBlock());
	}
	
	//TODO: Verify this method works
	public NodeRelop parseRelop() throws SyntaxException
	{
		String token = curr().tok();
		NodeRelop relop = null;
		switch(token)
		{
		case "<":	match("<");
					relop = new NodeRelop("<");
					break;
		
		case "<=":  match("<=");
					relop = new NodeRelop("<=");
					break;
					
		case ">":	match(">");
					relop = new NodeRelop(">");
					break;
					
		case ">=":	match(">=");
					relop = new NodeRelop(">=");
					break;
		
		case "<>":	match("<>");
					relop = new NodeRelop("<>");
					break;
		
		case "==":	match("==");
					relop = new NodeRelop("==");
					break;
		
		default:	throw new SyntaxException(scanner.pos(), new Token(token), new Token(token));
		}
		
		return relop;
	}
	
	//TODO: Verify this method works
	public NodeStmtBeginEnd parseStmtBeginEnd() throws SyntaxException
	{
		return new NodeStmtBeginEnd(parseBlock());
	}
	
	//TODO: Verify this method works
	public NodeStmtIf parseStmtIf() throws SyntaxException
	{
		return new NodeStmtIf(parseBoolExpr(), parseStmt());
	}
	
	//TODO: Verify this method works
	public NodeStmtIfElse parseStmtIfElse() throws SyntaxException
	{
		return new NodeStmtIfElse(parseBoolExpr(), parseStmt(), parseStmt());
	}
	
	//TODO: Verify this method works
	public NodeStmtRead parseStmtRead() throws SyntaxException
	{
		Token id = curr();
		match("id");
		return new NodeStmtRead(id.lex());
	}
	
	//TODO: Verify this method works
	public NodeStmtWhile parseStmtWhile() throws SyntaxException
	{
		return new NodeStmtWhile(parseBoolExpr(), parseStmt());
	}
	
	//TODO: Implement this method
	public NodeStmtWrite parseStmtWrite() throws SyntaxException
	{
		return new NodeStmtWrite(parseExpr());
	}
}
