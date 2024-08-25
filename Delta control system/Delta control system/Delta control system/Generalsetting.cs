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
    public partial class Generalsetting : Form
    {
        public Generalsetting()
        {
            InitializeComponent();
        }

        private void label11_Click(object sender, EventArgs e)
        {
            Setting setting = new Setting();
            setting.Show();
            this.Hide();
        }
    }
}
