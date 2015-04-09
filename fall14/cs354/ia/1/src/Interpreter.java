/**
 * 
 * @author awoods
 * This is the driver class that handles the calling
 * of all methods required to Interpret the program.
 */
public class Interpreter {

	/**
	 * Main method of the driver class Interpreter.  Creates a new parser
	 * and environment, then iterates through all the strings contained in
	 * the passed parameter.  Each string is parsed and evaluated.
	 * @param args Command line arguments representing a program to be
	 * 			   interpreted.
	 */
	public static void main(String[] args) {
		Parser parser=new Parser();
		Environment env=new Environment();
		for (String stmt: args)
			try {
				System.out.println(parser.parse(stmt).eval(env));
			} catch (SyntaxException e) {
				System.err.println(e);
			} catch (EvalException e) {
				System.err.println(e);
			}
	}

}
