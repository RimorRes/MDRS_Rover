using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace @interface
{
    public partial class Rover_Interface : Form
    {
        public Rover_Interface()
        {
            InitializeComponent();
            Settings_groupbox.Hide();
            Settings_groupbox.Enabled = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Settings_groupbox.Show();
            Settings_groupbox.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void port_comboBox_SelectionChangeCommitted(object sender, EventArgs e)
        {
            /* Connect with port. */
        }
    }
}

namespace Communication
{
    partial class Connectivity
    {
        static public void Search()
        {
            string[] ports = SerialPort.GetPortNames();
            for(int i; i < ports.Length; i++)
            {
                // Fill port_comboBox with all ports. 
            }
        }
    }
}