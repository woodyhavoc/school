/*
Original Author: Jeff Cope
Maintaining Author: Amit Jain
Class: CS 453
*/

import java.io.*;
import java.util.StringTokenizer;

public class Webstats
{
	private final int MAX_LINE_SIZE = 4096;
	private final int MAX_NUM_FIELDS = 40;
	
	private double local_bytes;
	private double total_bytes;
	private double local_gets;
	private double total_gets;
	private double failed_gets;
	private double local_failed_gets;
	private String url;
	private int requests;

	private String program_name;
	
	/**
	  parse_line(): Parse one line into String tokens separated by the given delimiters.
	  @param line A character array containing the current server log entry line
	  @param delim: A character array containing the delimiters to be used to separate fields in the line
	  @return The number of tokens found in the line.
	*/
	private int parse_line(String line, String delim, String field[])
	{
		String next;
		int cnt = 0;
		StringTokenizer strtok = new StringTokenizer(line, delim);
		while (strtok.hasMoreTokens())
		{
			next = strtok.nextToken();
			if (cnt == MAX_NUM_FIELDS-1) break;
			field[cnt++] = next;
		}
		field[cnt] = null; /* make the field array be null-terminated */
		return cnt;
	}

	/*
	   free_tokens(): Clears spaces allocated for tokens in parse_line
	   @param num An int representing number of tokens in array of strings named field.
	   @param field An array of Strings
	   @return none.
	*/
	private void free_tokens(int num, String field[])
	{
		for (int i = 0; i < num; i++)
		{
			field[i] = "";
		}
	}
	
	/**
	  Webstats(): Initializes the webstats structure.
	  @param new_program_name This programs name.
	  @return none
	*/
	public Webstats(String new_program_name)
	{
		program_name = new_program_name;
		local_bytes = 0;
		total_bytes = 0;
		local_gets = 0;
		total_gets = 0;
		failed_gets = 0;
		local_failed_gets = 0;
	}

	private final int HTTP_STATUS_CODE_FIELD = 8;
	private final int BYTES_DOWNLOADED_FIELD = 9;

	/*
	  update_webstats(): Updates the webstats of the phtread object based on the input fields of current line.
	  @param num The number of fields in the current webserver log entry
	  @param field An array of num Strings representing the log entry
	  @param pthread The pthread object whose share of the webstats will be updated
	  @return none
	*/
	private void update_webstats(int num, String field[], PThread pthread)
	{
		int bytes_downloaded = 0;
		try {
			bytes_downloaded = Integer.parseInt(field[BYTES_DOWNLOADED_FIELD]);
		} catch (NumberFormatException e) {
			//skip lines without downloaded bytes field
		}
		//total_gets++;
		pthread.addTotalGets(1);
		//total_bytes += bytes_downloaded;
		pthread.addTotalBytes(bytes_downloaded);
		if (field[HTTP_STATUS_CODE_FIELD].equals("404"))
		{
			pthread.addFailedGets(1);
		}
			//failed_gets++;

		if ((field[0].indexOf("boisestate.edu") != -1) || (field[0].indexOf("132.178") != -1)) {
			//local_gets++;
			pthread.addLocalGets(1);
			//local_bytes += bytes_downloaded;
			pthread.addLocalBytes(bytes_downloaded);
			if (field[HTTP_STATUS_CODE_FIELD].equals("404"))
			{
				pthread.addLocalFailedGets(1);
			}
				//local_failed_gets++;
		}
	}
	
	/**
	 * Updates the global webstats with the values contained in the pthread object.
	 * Gets each webstat value from the pthread object, and increments the corresponding
	 * global webstat value by that amount.
	 *
	 * @param pthread The PThread object containing a portion of the overall webstat
	 * 				  data.
	 */
	private synchronized void updateGlobalWebstats(PThread pthread)
	{
		local_bytes += pthread.getLocalBytes();
		total_bytes += pthread.getTotalBytes();
		local_gets += pthread.getLocalGets();
		total_gets += pthread.getTotalGets();
		failed_gets += pthread.getFailedGets();
		local_failed_gets += pthread.getLocalFailedGets();
	}

	/*
	  print_webstats(): Print out webstats on standard output.
	  @return none
	*/
	private void print_webstats()
	{
		System.out.printf("%10s %15s   %15s  %15s\n", "TYPE", "gets","failed gets", "MB transferred");

		System.out.printf("%10s  %15.0f  %15.0f  %15.0f\n", "local", local_gets, local_failed_gets, 
													(double) local_bytes/(1024*1024));
		System.out.printf("%10s  %15.0f  %15.0f  %15.0f\n", "total", total_gets, failed_gets, 
													(double) total_bytes/(1024*1024));

	}
	
	/**
	  process_file(): The main function that processes the log file held by the PThread object
	  @param ptr Pointer to log file name.
	  @param pthread The PThread object that holds the file name to be processed
	  @return none
	  output: none
 	*/
	public void process_file(Object ptr, PThread pthread)
	{
		String filename = (String) ptr;
		String linebuffer;
		String field[] = new String[MAX_NUM_FIELDS]; 
		String end_date = "";

		System.err.println(program_name + ": processing log file " + filename);
		try {
			BufferedReader fin = new BufferedReader(new FileReader(new File(filename)));
			try {
				linebuffer = fin.readLine();
				if (linebuffer != null) {
					int num = parse_line(linebuffer, " []\"", field);
					update_webstats(num, field, pthread);
					System.out.println("Starting date: " + field[3]);
					free_tokens(num, field);
					
					while ((linebuffer = fin.readLine()) != null) {
						num = parse_line(linebuffer, " []\"", field);
						end_date = field[3];
						update_webstats(num, field, pthread);
						free_tokens(num, field);
						linebuffer = "";
					}
					System.out.println("Ending date: "+ end_date);
				}
			} catch (IOException e) {
				System.err.println("Cannot read from file "+ filename);
				System.exit(1);
			}
		} catch (FileNotFoundException e) {
			System.err.println("Cannot open file "+ filename);
			System.exit(1);
		}

		updateGlobalWebstats(pthread);
	}


	public static void main(String argv[]) throws InterruptedException
	{
		if (argv.length < 1)
		{
			System.err.println("Usage: java Webstats <access_log_file> {<access_log_file>}");
			System.exit(1);
		}
		
		Webstats ws = new Webstats("Webstats.java");
		
		Thread[] thread = new Thread[argv.length];
		
		for (int i = 0; i < argv.length; i++)
		{
			thread[i] = new Thread(new PThread(ws, argv[i]));
			thread[i].start();
		}
		
		for(int i = 0; i < argv.length; i++)
		{
			thread[i].join();
		}
		
		ws.print_webstats();
		System.exit(0);
	}
}

/**
 * This class is used to create threads that hold a file to
 * be processed for webstat data.
 */
class PThread implements Runnable
{
	private Webstats ws;
	private String filename;

	private double local_bytes;
	private double total_bytes;
	private double local_gets;
	private double total_gets;
	private double failed_gets;
	private double local_failed_gets;

	/**
	 * PThread constructor, accepts a Webstats object and the string of a filename.
	 * Initializes the local webstat data.
	 * @param ws The Webstat object that will be used to process the file
	 * @param filename The name of the file to be processed
	 */
	public PThread(Webstats ws, String filename)
	{
		this.ws = ws;
		this.filename = filename;
		this.local_bytes = 0;
		this.total_bytes = 0;
		this.local_gets = 0;
		this.total_gets = 0;
		this.failed_gets = 0;
		this.local_failed_gets = 0;
	}

	/**
	 * Called by the Thread object which takes a PThread as
	 * an argument in the constructor.
	 */
	@Override
	public void run()
	{
		ws.process_file(filename, this);
	}

	/**
	 * Increments the local bytes
	 * @param add Amount local_bytes will be incremented by
	 */
	public void addLocalBytes(double add)
	{
		this.local_bytes += add;
	}

	/**
	 * Increments the total bytes
	 * @param add Amount total_bytes will be incremented by
	 */
	public void addTotalBytes(double add)
	{
		this.total_bytes += add;
	}

	/**
	 * Increments the local gets
	 * @param add Amount local_gets will be incremented by
	 */
	public void addLocalGets(double add)
	{
		this.local_gets += add;
	}

	/**
	 * Increments total gets
	 * @param add Amount total_gets will be incremented by
	 */
	public void addTotalGets(double add)
	{
		this.total_gets += add;
	}

	/**
	 * Increments failed gets
	 * @param add Amount failed_gets will be incremented by
	 */
	public void addFailedGets(double add)
	{
		this.failed_gets += add;
	}

	/**
	 * Increments local failed gets
	 * @param add Amount local_failed_gets will be incremented by
	 */
	public void addLocalFailedGets(double add)
	{
		this.local_failed_gets += add;
	}

	/**
	 * Returns the value of local bytes
	 * @return value of local_bytes
	 */
	public double getLocalBytes()
	{
		return this.local_bytes;
	}

	/**
	 * Returns the value of total bytes
	 * @return value of total_bytes
	 */
	public double getTotalBytes()
	{
		return this.total_bytes;
	}

	/**
	 * Returns the value of local gets
	 * @param value of local_gets
	 */
	public double getLocalGets()
	{
		return this.local_gets;
	}

	/**
	 * Returns the value of total gets
	 * @return value of total_gets
	 */
	public double getTotalGets()
	{
		return this.total_gets;
	}

	/**
	 * Returns the value of failed tests
	 * @return value of failed_gets
	 */
	public double getFailedGets()
	{
		return this.failed_gets;
	}

	/**
	 * Returns the value of local failed gets
	 * @return value of local_failed_gets
	 */
	public double getLocalFailedGets()
	{
		return this.local_failed_gets;
	}
}

/* vim: set ts=4: */
