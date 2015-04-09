/**
 * Author: Aaron Woods
 */
using System;
using System.Drawing;
using System.Windows.Forms;

/**
 * Project LA5
 */
namespace LA5
{
	/**
	 * Class: BinaryCounter
	 *
	 * The BinaryCounter is repsonsible for updating
	 * the count whenever the button is clicked.  The LightSwitch
	 * invokes the appropriate methods in order to
	 * make the BinaryCounter increment the count,
	 * and update the display with the correct count.
	 */
	public class BinaryCounter
	{
		//Private Instance Variables
		private string binary;
		private int count;

		/**
		 * Constructor, this method accepts a parameter
		 * as the starting value for the count.  It then
		 * converts that number to a binary string and stores
		 * it in the private string object.
		 *
		 * @param val The starting value of the counter.
		 */
		public BinaryCounter(int val)
		{
			count = val;
			binary = Convert.ToString(count, 2);
		}

		/**
		 * Returns the string representation of the binary
		 * number.
		 *
		 * @return binary string of the count.
		 */
		public string getBinaryCount()
		{
			return binary;
		}

		/**
		 * Increments the count by one, and stores
		 * the binary string representation into
		 * the private string variable.
		 */
		public void incrementCount()
		{
			binary = Convert.ToString(++count, 2);
		}
	}
}
