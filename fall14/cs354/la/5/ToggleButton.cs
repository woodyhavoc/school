/**
 * Author: Aaron Woods
 */
using System;
using System.Drawing;
using System.Windows.Forms;

namespace LA5
{
	public class ToggleButton : Form
	{
		private string label1, label2;
		private Button button;

		public ToggleButton(string label1, string label2)
		{
			this.label1 = label1;
			this.label2 = label2;
			button = new Button();
			button.Text = label1;
			button.Name = "button";
			button.Click += buttonClick;
		}
		
		public Button getButton()
		{
			return button;
		}

		private void buttonClick(object sender, System.EventArgs e)
		{
			if(button.Text == label1)
			{
				button.Text = label2;
			}
			else
			{
				button.Text = label1;
			}
		}
	}
}
