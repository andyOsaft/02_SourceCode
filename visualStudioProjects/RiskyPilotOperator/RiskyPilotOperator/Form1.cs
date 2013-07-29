using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace RiskyPilotOperator
{
    public partial class riskyPilotOperatorForm : Form
    {
        public riskyPilotOperatorForm()
        {
            InitializeComponent();

            //assign delegate to related function
            updateComLogBoxDelegate = new updateComLogBox(updateComLogBoxMethod);

            portNumberComboBox.SelectedIndex = 5;
            serialPort1.BaudRate = 9600;
            serialPort1.ReadTimeout = 100;
        }



    //***********************************************************************************************************
    // updateComLogBoxMethod()
    // -----------------------	
    // method to read the incoming data from the serial port 
    // serialPort.ReadLine() is executed in an extra thread and can therefore not
    // access the components of form1
    //
    //***********************************************************************************************************
    public void updateComLogBoxMethod()
    {
	    string tempInputBuffer;

	    //read the serial port until less than 2 Bytes are left in the internal input buffer.
		if (serialPort1.BytesToRead > 2)
        {
            try
		    {

			    tempInputBuffer = serialPort1.ReadLine();
                comLogBox.AppendText("@#: " + tempInputBuffer + Environment.NewLine);

            }
            catch (TimeoutException)
		    {
                /* if there is now complete line received (with \r\n at the end) 
                    -> try to read the existing data in the input buffer 
                    -> may be data coming from the xbee module itself */
                try
                {
                    tempInputBuffer = serialPort1.ReadExisting();
                    comLogBox.AppendText("@#: " + tempInputBuffer + Environment.NewLine);
                }
                catch (Exception)
                {
                    comLogBox.AppendText("[ERROR]: serial port read data timeout " + Environment.NewLine);
                    //clear internal serial port input buffer
                    serialPort1.DiscardInBuffer();
                }
		    }
        }
    }

                



    private void portNumberComboBox_SelectedIndexChanged(object sender, EventArgs e)
    {
        serialPort1.PortName = portNumberComboBox.Text;
    }

    private void serialPortConnectButton_Click(object sender, EventArgs e)
    {
        try
        {
            if (serialPort1.IsOpen == true)
            {
                comLogBox.AppendText("@#: Serial port is already open." + Environment.NewLine);
            }
            else
            {
                serialPort1.Open();
                comLogBox.AppendText("@#: Serial port is open with " + serialPort1.BaudRate + " baud." + Environment.NewLine);
                
                //comLogBox.AppendText("@#: requesting xBee baud rate..." + Environment.NewLine);
                //System.Threading.Thread.Sleep(1100);
                ////enter Xbee command mode (observe guard times before and afterwards)
                //serialPort1.Write("+++");
                //System.Threading.Thread.Sleep(1100);
                ////read current baud rate of Xbee module
                //serialPort1.Write("ATBD\r");
            }
        }
        catch(Exception)
        {
            comLogBox.AppendText("@#: Error, cannot open serial port." + Environment.NewLine);
        }
    }


    private void serialPortDisconnectButton_Click(object sender, EventArgs e)
    {
        try
        {
            if (serialPort1.IsOpen == false)
            {
                comLogBox.AppendText("@#: Serial port is already closed." + Environment.NewLine);
            }
            else
            {
                serialPort1.Close();
                comLogBox.AppendText("@#: Serial port is closed." + Environment.NewLine);
            }
        }
        catch (Exception)
        {
            comLogBox.AppendText("[@#: Error, cannot close serial port." + Environment.NewLine);
        }

    }


    private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
    {
        //call delegate to transfer read data to textBox
		try
		{
			Invoke(updateComLogBoxDelegate);
		}
		catch (Exception)
		{
					
		}
        
    }

    private void mode1SubMenuButton_Click(object sender, EventArgs e)
    {
        serialPort1.Write("s");
    }

    private void comLogBoxClearButton_Click(object sender, EventArgs e)
    {
        comLogBox.Clear();
    }


 }
}
