﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Delta_control_system
{
    public partial class Loading : Form
    {
        public Loading()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
        int starting = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            starting += 1;
            Myprogress.Value = starting; 
            Percentage.Text = starting + " %";
            if (Myprogress.Value == 100)
            {
                Myprogress.Value = 0;
                timer1.Stop();
                Login log = new Login();   
                log.Show(); 
                this.Hide();
            }
        }

        private void Loading_Load(object sender, EventArgs e)
        {
            timer1.Start();
        }
    }
}
