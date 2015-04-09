/**
 * Author: Aaron Woods
 */

/**
 * Class FileAccessBoss
 *
 * This class is responsbile for creating the threads to read
 * the file, and join them back together after the read operation
 * has been completed.
 */
public class FileAccessBoss
{	
	/**
	 * Main method, creates an array of threads, and then has them
	 * all start working on reading the file.  After this has been
	 * completed, the threads are joined back together and the program
	 * exits.
	 */
	public static void main(String[] args) throws InterruptedException
	{
		int nproc = Runtime.getRuntime().availableProcessors();
		
		int numThreads = nproc * 2;
		
		Thread[] threads = new Thread[numThreads];
		
		for(int i = 0; i < numThreads; i++)
		{
			threads[i] = new Thread(new FileAccess(nproc, i+1));
			threads[i].start();
		}
		
		for(int i = 0; i < numThreads; i++)
		{
			threads[i].join();
		}
		
		System.exit(0);
	}
}
