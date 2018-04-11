using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ESTemperatureRegulator.Views
{
    public class ValidationView : Panel
    {
        private Timer _timer;

        private Label _label;

        private int _counter = 0;

        public ValidationView()
        {
            _timer = new Timer
            {
                Interval = 700
            };

            _timer.Tick += Timer_Tick;

            _label = new Label
            {
                AutoSize = true,
                Font = new System.Drawing.Font("Arial", 12.0F),
                Dock = DockStyle.Fill
            };

            Controls.Add(_label);
        }


        public void Activate(string message)
        {
            _label.Text = message;
            _timer.Enabled = true;
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            _counter++;
            if(_counter > 5)
            {
                _counter = 0;
                _timer.Enabled = false;
            }

            if(_counter % 2 == 0)
            {
                _label.BeginInvoke((Action)(() => { _label.BackColor = System.Drawing.Color.Red; }));
            }
            else
            {
                _label.BeginInvoke((Action)(() => { _label.BackColor = System.Drawing.Color.White; }));
            }
        }
    }
}
