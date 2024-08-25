using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace Delta_control_system
{
    public partial class PinSetting : Form
    {
        public PinSetting()
        {
            InitializeComponent();
        }
        SqlConnection Con = new SqlConnection(@"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=C:\Users\Yuki\Documents\DCSdata.mdf;Integrated Security=True;Connect Timeout=30");
        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void guna2Button1_Click(object sender, EventArgs e)
        {
            if (Pin1Tb.Text == "" || Pin2Tb.Text == "")
            {
                MessageBox.Show("请输入和确定新密码！");
            }
            else if (Pin2Tb.Text != Pin1Tb.Text)
            {
                MessageBox.Show("新密码和确认密码不一致，请重新输入！");
            }
            else
            {
                try
                {
                    Con.Open();
                    string query = "update AccountTb1 set Pin= '"+Pin1Tb.Text+"' ";
                    SqlCommand cmd = new SqlCommand(query,Con);
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("密码修改成功！");
                    Con.Close();
                    Login log = new Login();   
                    log.Show();
                    this.Hide();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void label11_Click(object sender, EventArgs e)
        {
            Setting setting = new Setting();
            setting.Show();
            this.Hide();
        }
    }
}
