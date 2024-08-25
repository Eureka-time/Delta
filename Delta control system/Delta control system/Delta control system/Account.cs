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
    public partial class Account : Form
    {
        public Account()
        {
            InitializeComponent();
        }
        SqlConnection Con = new SqlConnection(@"Data Source=(LocalDB)\MSSQLLocalDB;AttachDbFilename=C:\Users\Yuki\Documents\DCSdata.mdf;Integrated Security=True;Connect Timeout=30");

        private void SubmitBtn_Click(object sender, EventArgs e)
        {   
            int bal = 0;
            if (AccNumTb.Text ==  " " || AccNameTb.Text ==  "" || IDcardTb.Text == "" || PhoneTb.Text == "" || AddressTb.Text == "" || OccupationTb.Text == "" || PinTb.Text == "" )
            {
                MessageBox.Show("信息缺失！！！！");
            }
            else
            {
                try
                { 
                    Con.Open();
                    string query = "insert into AccountTb1 values('" +AccNumTb.Text + "','"+AccNameTb.Text + "','"+IDcardTb.Text+"','"+ PinTb.Text+"','"+ PhoneTb.Text+"','"+ AddressTb.Text + "','" + Education.SelectedItem.ToString() + "','"+ OccupationTb.Text + "',"+bal+")";
                    SqlCommand cmd = new SqlCommand(query, Con);
                    cmd.ExecuteNonQuery();
                    MessageBox.Show("账户注册成功！！！！");
                    Con.Close();
                    Login log = new Login();
                    log.Show();
                    this.Hide();

                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                } 
             }
        }

        private void LogoutLbl_Click(object sender, EventArgs e)
        {
            Login log = new Login();
            log.Show();
           this.Hide();
        }
    }
}
