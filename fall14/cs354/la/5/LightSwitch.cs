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
	 * Class: LightSwitch extends Form
	 *
	 * This class is responsible for the creation of the GUI
	 * element, and for gathering the binary count information
	 * from the BinaryCounter class.
	 */
	public class LightSwitch : Form
	{
		// Private Instance Variables
		private ToggleButton button;
		private BinaryCounter counter;
		private Label label;

		/**
		 * Constructor, accepts no parameters and initializes the 
		 * instance variables.  It also positions the text label to the
		 * left of the button so it can be seen.  Additionally, it sets
		 * the name of the window.  Finally, it establishes an event
		 * handler for when the button is clicked. 
		 */
		public LightSwitch()
		{
			Text = "Light Switch";
			button = new ToggleButton("off", "on");
			counter = new BinaryCounter(0);
			label = new Label();
			label.Text = "0";
			label.Location = new Point(label.Location.X + button.getButton().Width, label.Location.Y);
			Controls.AddRange(new System.Windows.Forms.Control[] {this.button.getButton()});
			Controls.AddRange(new System.Windows.Forms.Control[] {this.label});
			button.getButton().Click += lightButtonClick;
		}
		
		/**
		 * This is the event handler that is executed whenever the button
		 * is clicked.  It calls the appropriate methods to increment
		 * the binary counter, and set the binary string equal to that value.
		 *
		 * @param sender The object that is causing the event to be handled
		 * @param e The arguments associated with the event
		 */
		private void lightButtonClick(object sender, System.EventArgs e)
		{
			counter.incrementCount();
			label.Text = counter.getBinaryCount();
		}

		/**
		 * Main method, creates a new LightSwitch object
		 * and tells the Application to run that object.
		 *
		 * @param args Command-line arguments, not used in this program.
		 */
		public static void Main(string[] args)
		{
			LightSwitch light = new LightSwitch();
			Application.Run(light);
		}
	}
}
