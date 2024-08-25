using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Delta_control_system
{
    public partial class Login : Form
    {
        public Login()
        {
            InitializeComponent();
        }
        SqlConnection Con = new SqlConnection(@"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=C:\Users\Yuki\Documents\DCSdata.mdf;Integrated Security=True;Connect Timeout=30");
        private void label7_Click(object sender, EventArgs e)
        {
            Account acc = new Account();
            acc.Show();
            this.Hide();
        }
        private static string AccNumber;
        private void LoginBtn_Click(object sender, EventArgs e)
        {
            Con.Open();
            SqlDataAdapter sda = new SqlDataAdapter("select count(*) from AccountTb1 where AccNum = '" + AccNumTb.Text +"' and Pin = '"+PinTb.Text+"' ", Con);
            DataTable dt = new DataTable();
            sda.Fill(dt);
            if (dt.Rows[0][0].ToString() == "1")
            {
                HOME home = new HOME();
                home.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("您输入的用户名或密码错误，请重新输入！");
            }
            Con.Close();
        }

        private void Login_Load(object sender, EventArgs e)
        {

        }
    }
}
