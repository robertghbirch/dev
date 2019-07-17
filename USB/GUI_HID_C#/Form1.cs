using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using CyUSB;
using System.Diagnostics;
using System.Runtime.InteropServices;



namespace BulkLoop
{
    public partial class Form1 : Form
    {
        CyHidDevice         oCyHidDevice        = null;
        USBDeviceList       oUSBDeviceList      = null;
        bool doit = false;
        int c = 100;
        int t = 100;
        uint s = 0;
        uint over = 0;
        uint over2 = 0;
        uint over5 = 0;
        uint[] array1 = new uint[5000];
        uint idx = 0;
        

        [DllImport("inpout32.dll")]
        private static extern void Out32(short PortAddress, short Data);

        public Form1()
        {

            InitializeComponent();

       
            // Create a list of CYUSB devices
            oUSBDeviceList = new USBDeviceList(CyConst.DEVICES_HID);

            oCyHidDevice = null;
            this.Text = "No device";

            foreach (CyHidDevice oCyHidDeviceTemp in oUSBDeviceList)
            {
                if ((oCyHidDeviceTemp.VendorID == 0x0483) && (oCyHidDeviceTemp.ProductID == 0x5750))
                {
                    oCyHidDevice = oCyHidDeviceTemp;

                    this.Text = oCyHidDevice.Product + " by " + oCyHidDevice.Manufacturer + " - Connected";
               //     this.Text = "DFDFD";

                    break ;
                } 
            }

            oCyHidDevice.Outputs.DataBuf[1] = 0;
            oCyHidDevice.Outputs.DataBuf[2] = 0;
            oCyHidDevice.Outputs.DataBuf[3] = 0;
            //oCyHidDevice.Outputs.DataBuf[4] = 4;
           // oCyHidDevice.Outputs.DataBuf[5] = 5;
           // oCyHidDevice.Outputs.DataBuf[6] = 6;

            this.Show();


            bool bResult;

            Stopwatch time10kOperations = Stopwatch.StartNew();
            UInt16 iPort = 0x2FF8;
            time10kOperations.Stop();
            Freq.Text = Stopwatch.Frequency.ToString() + " per second";
            while (true)
            {
               Application.DoEvents();

                
                if (!doit)
                {

                    continue;
                }

                ;


                bResult = oCyHidDevice.ReadInput();
                time10kOperations.Stop();



                t = oCyHidDevice.Inputs.DataBuf[7];
                t = (t << 8) + oCyHidDevice.Inputs.DataBuf[6];
                t = (t << 8) + oCyHidDevice.Inputs.DataBuf[5];
                t = (t << 8) + oCyHidDevice.Inputs.DataBuf[4];

                s = oCyHidDevice.Inputs.DataBuf[11];
                s = (s << 8) + oCyHidDevice.Inputs.DataBuf[10];
                s = (s << 8) + oCyHidDevice.Inputs.DataBuf[9];
                s = (s << 8) + oCyHidDevice.Inputs.DataBuf[8];


                array1[idx++] = s;

                if (idx == 4990)
                {

                    idx = 9;
                }

               // continue;

                int tt = (int)oCyHidDevice.Inputs.DataBuf[2] + (oCyHidDevice.Inputs.DataBuf[3] << 8);

                //DB.AppendText(oCyHidDevice.Inputs.DataBuf[2].ToString() + " - " + oCyHidDevice.Inputs.DataBuf[3].ToString() + " - " + (time10kOperations.ElapsedMilliseconds).ToString() + "\r\n");
                DB.AppendText(tt.ToString() + " - " + (time10kOperations.ElapsedTicks).ToString() + " " + (time10kOperations.ElapsedMilliseconds).ToString() + " - " + t + " - " + s.ToString() + "\r\n");

                if (0 != c)
                {
                    c--;
                }
                if (0 == c)
                {
                    
                    if (time10kOperations.ElapsedMilliseconds > 0)
                    {
                        over++;
                        Over.Text = over.ToString();
                    }
                    if (time10kOperations.ElapsedMilliseconds > 1)
                    {
                        over2++;
                        Over2.Text = over2.ToString();
                    }
                    if (time10kOperations.ElapsedMilliseconds > 5)
                    {
                        over5++;
                        Over5.Text = over5.ToString();
                    }
                }

                time10kOperations.Reset();
                time10kOperations.Start();
                
                


                continue;

                oCyHidDevice.Outputs.DataBuf[1] = (byte)time10kOperations.ElapsedMilliseconds;

                if (oCyHidDevice.Outputs.DataBuf[1] > 3)
                {
                    oCyHidDevice.Outputs.DataBuf[2]++;
                }

                if (oCyHidDevice.Outputs.DataBuf[1] > 6)
                {
                    oCyHidDevice.Outputs.DataBuf[3]++;
                }


                oCyHidDevice.Outputs.DataBuf[1] = 0x73;
                try
                {
                    oCyHidDevice.Outputs.DataBuf[2] = Convert.ToByte(textBox1.Text);
                    oCyHidDevice.Outputs.DataBuf[3] = Convert.ToByte(textBox2.Text);
                }
                catch
                {
                    textBox1.Text = "255";
                    textBox2.Text = "255";
                    oCyHidDevice.Outputs.DataBuf[2] = Convert.ToByte(textBox1.Text);
                    oCyHidDevice.Outputs.DataBuf[3] = Convert.ToByte(textBox2.Text);
                }
                //oCyHidDevice.Outputs.DataBuf[4] = 0x0D;
               // oCyHidDevice.Outputs.DataBuf[5] = 0x0A;
                //oCyHidDevice.Outputs.DataBuf[6]++;



                time10kOperations.Reset();
                time10kOperations.Start();

                bResult = oCyHidDevice.WriteOutput();

                
                //Out32((short)iPort, (short)0xFF);

                time10kOperations.Stop();
                // DB.AppendText(time10kOperations.ElapsedMilliseconds.ToString() + "\r\n") ;
                DB.AppendText(time10kOperations.ElapsedMilliseconds.ToString());




                
            }
            //oCyHidDevice.Outputs.DataBuf[2] = (byte)(oCyHidDevice.Inputs.DataBuf[1] + 1);
            //oCyHidDevice.Outputs.DataBuf[3] = (byte)(oCyHidDevice.Inputs.DataBuf[1] + 2);


            // if (bResult)
            // {
            //     bResult = oCyHidDevice.ReadInput();
            //     DB.AppendText(oCyHidDevice.Inputs.DataBuf[1].ToString() + "\r\n");
            // }


        }

        private void button1_Click(object sender, EventArgs e)
        {
            doit = !doit;

            if (doit)
            {
                GO.Text = "STOP";
            } else
            {
                GO.Text = "GO";

            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            doit = false;
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }
    }
}