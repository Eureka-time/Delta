using System;
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
    public partial class HOME : Form
    {
        public HOME()
        {
            InitializeComponent();
        }

        private void guna2Button4_Click(object sender, EventArgs e)
        {
            MessageBox.Show("该软件版本已为稳定版本！" +
                "如需更新最新版本，请去官网下载最新版本安装包！");
        }

        private void guna2Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void HOME_Load(object sender, EventArgs e)
        {
           
        }

        private void label6_Click(object sender, EventArgs e)
        {
            Login log = new Login();
            log.Show();
            this.Hide();
        }

        private void guna2Button1_Click(object sender, EventArgs e)
        {
            Control control = new Control();   
            control.Show();
            this.Hide();
        }

        private void guna2Button2_Click(object sender, EventArgs e)
        {
            Debug debug = new Debug();  
            debug.Show();
            this.Hide();
        }

        private void guna2Button3_Click(object sender, EventArgs e)
        {
            Setting setting = new Setting();
            setting.Show();
            this.Hide();
        }
    }
}
