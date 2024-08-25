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
using Delta_control_system.Properties;

namespace Delta_control_system
{
    public partial class InternetSetting : Form
    {
        public InternetSetting()
        {
            InitializeComponent();
        }
        SqlConnection Con = new SqlConnection(@"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=C:\Users\Yuki\Documents\DCSdata.mdf;Integrated Security=True;Connect Timeout=30");

        private void ChangeBtn_Click(object sender, EventArgs e)
        {
            int bal = 0;
            if (IPATb.Text == "")
            {
                MessageBox.Show("IP地址不可为空！！！，请重新输入！！！");
            }
            else
            {
                try
                {
                    Con.Open();
                    string query = "update InternetTb1 set IPA=' " + IPATb.Text + " '  , SK= ' " + SKTb.Text + " '  ,  DG=' " + DGTb.Text + " ' ";
                    SqlCommand cmd = new SqlCommand(query, Con);
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("网络修改成功！");
                    Con.Close();
                    Setting setting = new Setting();
                    setting.Show();
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
