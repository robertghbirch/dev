using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication3
{
    public partial class Form1 : Form
    {
        /////////////////////////////////////////////////////////////////
        // X Y Graph
        int X = 0, Y = 0;
        int iDir = 1;

        const int MAX_X = 80;
        const int MAX_Y = 60;
        const int MAX_XY = 80;

        int iOldX, iOldY;

        Boolean bX, bY;   // Whether to display or not
        Boolean bChange = false;

        Graphics XY_Graphics;
        BufferedGraphicsContext XY_Context;
        BufferedGraphics XY_Grafx;

        Graphics XY_BackBufferGraphics;
        Graphics XY_FrontBufferGraphics;

        System.Drawing.Bitmap XY_BackBitmap;
        System.Drawing.Bitmap XY_FrontBitmap;

        private void button2_Click(object sender, EventArgs e)
        {
            while (true)
            {
                Application.DoEvents();
                System.Threading.Thread.Sleep(10);
                button1_Click(sender, e);
            }
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int i, j;
            X += iDir;
            Y += iDir;

            if (X > 25)
            {
                iDir = -1;
            }
            if (0 == X)
            {
                iDir = 1;
            }
            bX = true;
            bY = true;
            

            SolidBrush redBrush = new SolidBrush(Color.Red);

            // Copy from picture box
            XY_FrontBufferGraphics.DrawImage(XY_BackBitmap, new Rectangle(0, 0, pictureBox2.Width - 5, pictureBox2.Height - 4), new Rectangle(1, 0, pictureBox2.Width - 5, pictureBox2.Height - 4), GraphicsUnit.Pixel);
            XY_FrontBufferGraphics.DrawLine(new Pen(Color.DarkGray), pictureBox2.Width - 5, 0, pictureBox2.Width - 5, pictureBox2.Height - 5);


            // X Y Graph
            if ((iOldX != -1) && (bX))
            {
                i = (pictureBox2.Height - 5) - ((pictureBox2.Height - 5) * X / MAX_XY);
                j = (pictureBox2.Height - 5) - ((pictureBox2.Height - 5) * iOldX / MAX_XY);
                XY_FrontBufferGraphics.DrawLine(new Pen(Color.Red), pictureBox2.Width - 5 - 1, j, pictureBox2.Width - 5, i);
            }

            if ((iOldY != -1) && (bY))
            {
                i = (pictureBox2.Height - 5) - ((pictureBox2.Height - 5) * Y / MAX_XY);
                j = (pictureBox2.Height - 5) - ((pictureBox2.Height - 5) * iOldY / MAX_XY);
                XY_FrontBufferGraphics.DrawLine(new Pen(Color.Blue), pictureBox2.Width - 5 - 1, j, pictureBox2.Width - 5, i);
            }
            iOldX = X;
            iOldY = Y;

            // X Y Graph
            XY_BackBufferGraphics.DrawImage(XY_FrontBitmap, 0, 0);
            XY_Grafx.Graphics.DrawImage(XY_FrontBitmap, 0, 0);

            XY_Grafx.Render(XY_Graphics);

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            XY_BackBitmap = new Bitmap(pictureBox2.Width, pictureBox2.Height);
            XY_FrontBitmap = new Bitmap(pictureBox2.Width, pictureBox2.Height);

            XY_Context = BufferedGraphicsManager.Current;
            XY_Context.MaximumBuffer = new Size(pictureBox2.Width, pictureBox2.Height);
            XY_Graphics = pictureBox2.CreateGraphics();
            XY_Grafx = XY_Context.Allocate(pictureBox2.CreateGraphics(), new Rectangle(0, 0, pictureBox2.Width, pictureBox2.Height));
            XY_Grafx.Graphics.Clear(Color.DarkGray);


            XY_BackBufferGraphics = Graphics.FromImage(XY_BackBitmap);
            XY_BackBufferGraphics.Clear(Color.DarkGray);

            XY_FrontBufferGraphics = Graphics.FromImage(XY_FrontBitmap);
            XY_FrontBufferGraphics.Clear(Color.DarkGray);


            
        }
    }
}
