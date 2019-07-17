namespace BulkLoop
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.DB = new System.Windows.Forms.TextBox();
            this.GO = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.Freq = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.Over = new System.Windows.Forms.Label();
            this.Over2 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.Over5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // DB
            // 
            this.DB.Location = new System.Drawing.Point(12, 12);
            this.DB.Multiline = true;
            this.DB.Name = "DB";
            this.DB.Size = new System.Drawing.Size(768, 330);
            this.DB.TabIndex = 7;
            // 
            // GO
            // 
            this.GO.Location = new System.Drawing.Point(12, 531);
            this.GO.Name = "GO";
            this.GO.Size = new System.Drawing.Size(175, 23);
            this.GO.TabIndex = 8;
            this.GO.Text = "GO";
            this.GO.UseVisualStyleBackColor = true;
            this.GO.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 456);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 9;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(118, 456);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(100, 20);
            this.textBox2.TabIndex = 10;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(41, 430);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Byte 1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(150, 430);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Byte 2";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(356, 421);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(28, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "Freq";
            // 
            // Freq
            // 
            this.Freq.AutoSize = true;
            this.Freq.Location = new System.Drawing.Point(399, 421);
            this.Freq.Name = "Freq";
            this.Freq.Size = new System.Drawing.Size(37, 13);
            this.Freq.TabIndex = 14;
            this.Freq.Text = "Byte 2";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(356, 498);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Over 1ms";
            // 
            // Over
            // 
            this.Over.AutoSize = true;
            this.Over.Location = new System.Drawing.Point(437, 498);
            this.Over.Name = "Over";
            this.Over.Size = new System.Drawing.Size(13, 13);
            this.Over.TabIndex = 16;
            this.Over.Text = "0";
            // 
            // Over2
            // 
            this.Over2.AutoSize = true;
            this.Over2.Location = new System.Drawing.Point(437, 520);
            this.Over2.Name = "Over2";
            this.Over2.Size = new System.Drawing.Size(13, 13);
            this.Over2.TabIndex = 18;
            this.Over2.Text = "0";
            this.Over2.Click += new System.EventHandler(this.label5_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(356, 520);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(52, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "Over 2ms";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // Over5
            // 
            this.Over5.AutoSize = true;
            this.Over5.Location = new System.Drawing.Point(437, 541);
            this.Over5.Name = "Over5";
            this.Over5.Size = new System.Drawing.Size(13, 13);
            this.Over5.TabIndex = 20;
            this.Over5.Text = "0";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(356, 541);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(52, 13);
            this.label7.TabIndex = 19;
            this.label7.Text = "Over 5ms";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(784, 561);
            this.Controls.Add(this.Over5);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.Over2);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.Over);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.Freq);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.GO);
            this.Controls.Add(this.DB);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximumSize = new System.Drawing.Size(800, 600);
            this.MinimumSize = new System.Drawing.Size(800, 600);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "HID_C_Sharp_GUI_BEGINNER";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox DB;
        private System.Windows.Forms.Button GO;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label Freq;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label Over;
        private System.Windows.Forms.Label Over2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label Over5;
        private System.Windows.Forms.Label label7;
    }
}

