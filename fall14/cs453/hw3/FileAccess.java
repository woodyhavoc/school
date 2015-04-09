/**
 * Author: Aaron Woods
 */

/**
 * Class File Access
 *
 * This class implements the interface "Runnable" so that
 * multiple threads can access its methods.  It is used
 * to simulate the reading of a file by multiple threads
 * simultaneously.
 */
public class FileAccess implements Runnable
{
	private static int maxThreads;
	private static int threadsTotal = 0;
	private int threadID;
	
	/**
	 * Constructor
	 *
	 * Accepts two parameters to set the maximum thread id
	 * value, and also the id value of the current thread.
	 *
	 * @param nproc The number of processors on the machine,
	 *              the maximum number of thread ids is one
	 *              less than this amount.
	 *
	 * @param id 	The id value of the current thread
	 */
	public FileAccess(int nproc, int id)
	{
		maxThreads = nproc-1;
		threadID = id;
	}

	/**
	 * This method begins the file access by each thread.
	 * There is a synchronized block in the beginning portion
	 * of the method.  It is used only to determine if the thread
	 * can continue on to file access, hold threads in wait until
	 * the threadTotal is low enough, and finally, increment the
	 * total thread ID count.
	 *
	 * @param id The ID value of the current thread
	 */	
	public void startAccess(int id)
	{
		synchronized(this)
		{
			if(id > maxThreads)
			{
				System.out.println("File Access Denied to Thread ID: " + id + ", ID number too big.");
				System.out.flush();
				return;
			}
			
			while(threadsTotal + id > maxThreads);
			
			threadsTotal += id;
		}
		
		System.out.println("Thread ID: " + id + " starting file access.  Sum of thread IDs: " + threadsTotal + ".");
		System.out.flush();

		endAccess(id);
	}
	
	/**
	 * This method is used to end the access to the file.
	 *
	 * All it does is decrement the total thread id count,
	 * and print a message saying that file access for the
	 * current thread is over.
	 *
	 * @param id The ID value of the current thread.
	 */	
	public synchronized void endAccess(int id)
	{
		threadsTotal -= id;
		System.out.println("Thread ID: " + id + " ending file access.  Sum of thread IDs: " + threadsTotal + ".");
		System.out.flush();
	}
	
	/**
	 * This method is part of the "Runnable" interface.
	 *
	 * It is a required method for multi-threading to work.
	 * It simply calls the startAccess() method.
	 */
	@Override
	public void run()
	{
		this.startAccess(this.threadID);		
	}
}
