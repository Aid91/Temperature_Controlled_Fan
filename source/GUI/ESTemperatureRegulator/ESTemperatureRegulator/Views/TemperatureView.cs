using ESTemperatureRegulator.Controllers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ESTemperatureRegulator.Views
{
    class TemperatureView : Panel, ITemperatureView
    {
        //TODO: change colors, handle track bar control

        private Button _plusButton = new Button();
        private Button _minusButton = new Button();
        private Label _temperatureLabel = new Label();
        private int _initialTemperature;
        private TemperatureController _temperatureController;

        public int InitialTemperature
        {
            get { return _initialTemperature; }
            set { _initialTemperature = value; }
        }

        public TemperatureView(int initalTemperature)
        {
            _initialTemperature = initalTemperature;
            Initialize(_initialTemperature);

            Controls.Add(_temperatureLabel);
            Controls.Add(_minusButton);
            Controls.Add(_plusButton);
        }

        public void SetController(TemperatureController temperatureController)
        {
            _temperatureController = temperatureController;
        }

        private void Initialize(int initialTemperature)
        {
            Size = new System.Drawing.Size(150, 100);
            BackColor = System.Drawing.Color.White;

            _plusButton.Location = new System.Drawing.Point(105, 10);
            _plusButton.Name = "_plusButton";
            _plusButton.RightToLeft = System.Windows.Forms.RightToLeft.No;
            _plusButton.Size = new System.Drawing.Size(40, 30);
            _plusButton.TabIndex = 0;
            _plusButton.Text = "+";
            _plusButton.UseVisualStyleBackColor = true;
            _plusButton.Click += PlusButton_Click;
            _plusButton.FlatStyle = FlatStyle.Flat;

            _minusButton.Location = new System.Drawing.Point(105, 55);
            _minusButton.Name = "_minusButton";
            _minusButton.Size = new System.Drawing.Size(40, 30);
            _minusButton.TabIndex = 1;
            _minusButton.Text = "-";
            _minusButton.UseVisualStyleBackColor = true;
            _minusButton.Click += MinusButton_Click;
            _minusButton.FlatStyle = FlatStyle.Flat;

            _temperatureLabel.AutoSize = true;
            _temperatureLabel.Location = new System.Drawing.Point(0, 20);
            _temperatureLabel.Font = new System.Drawing.Font("Arial", 36);
            _temperatureLabel.ForeColor = System.Drawing.Color.Black;
            _temperatureLabel.Name = "_temperatureLabel";
            _temperatureLabel.Text = initialTemperature.ToString();
            _temperatureLabel.Size = new System.Drawing.Size(0, 20);
            _temperatureLabel.TabIndex = 2;
        }

        private void MinusButton_Click(object sender, EventArgs e)
        {
            _temperatureController.DoMinusButtonClick();
        }

        private void PlusButton_Click(object sender, EventArgs e)
        {
            _temperatureController.DoPlusButtonClick();
        }

        public void SetTemperature()
        {
            _temperatureLabel.Text = InitialTemperature.ToString();
        }
    }
}
