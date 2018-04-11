using ESTemperatureRegulator.Interfaces;
using ESTemperatureRegulator.Views;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Timers;
using ESTemperatureRegulator.Controllers;
using System.IO.Ports;

namespace ESTemperatureRegulator
{
    // FIRST VERSION
    public partial class TemperatureControlledFanForm : Form
    {
        private IReceiver _mainView;

        private AbstractSender _sender;

        private Receiver _receiver;

        private SerialPort _serialPort;

        public TemperatureControlledFanForm()
        {
            InitializeComponent();
            BackColor = Color.White;

            _serialPort = new SerialPort
            {
                BaudRate = 9600,
                DtrEnable = true,
                RtsEnable = true,
                DataBits = 8
            };

            string portId = Helpers.Helpers.AutodetectArduinoPort();
            if(portId != null)
            {
                _serialPort.PortName = portId;
            }

            /*_sender = new Sender()
            {
                SerialPort = _serialPort
            };*/

            _sender = new RTOSSender()
            {
                SerialPort = _serialPort
            };

            _mainView = new MainView(_sender)
            {
                Size = new Size(Width, Height),
                Location = new Point(0, 0)
            };

            _receiver = new Receiver(_mainView, _serialPort);
           
            Controls.Add((Panel)_mainView);

            if (!_serialPort.IsOpen)
            {
                try
                {
                    _serialPort.Open();
                    ((MainView)_mainView).ConnectionStatus = Constants.ConnectionStatus.Connected;
                }
                catch (Exception e)
                {
                    ((MainView)_mainView).ConnectionStatus = Constants.ConnectionStatus.Disconnected;
                } 
            }
            else
            {
               MessageBox.Show("The Serial Port is already open!");
            }

            MainController mainController = new MainController((MainView)_mainView);
        }
    }
}
