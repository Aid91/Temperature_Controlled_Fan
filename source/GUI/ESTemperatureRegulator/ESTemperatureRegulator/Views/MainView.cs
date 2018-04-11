using ESTemperatureRegulator.Constants;
using ESTemperatureRegulator.Controllers;
using ESTemperatureRegulator.Interfaces;
using Graph;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ESTemperatureRegulator.Views
{
    class MainView : Panel, IReceiver
    {
        private TemperatureView _lowTemperatureView;

        private TemperatureView _highTemperatureView;

        private TemperatureController _lowTemperatureController;

        private TemperatureController _highTemperatureController;

        private Button _startButton;

        private Button _stopButton;

        private TrackBar _motorSpeedTrackBar;

        private ChartView _chartView;

        private AbstractSender _sender;

        private MainController _mainController;

        private Label _motorSpeedLabel;

        private CheckBox _automaticMotorSpeedCheckBox;

        private Label _referenceTemperaturesLabel;

        private Label _automaticFanSpeedRegulationLabel;

        private Label _fanSpeedLabel;

        private ValidationView _validationView;

        private Timer _connectionStatusTimer;

        private Panel _connectionStatusPanel;

        private ConnectionStatus _connectionStatus;

        public TrackBar MotorSpeedTrackBar
        {
            get
            {
                return _motorSpeedTrackBar;
            }

            set
            {
                _motorSpeedTrackBar = value;
            }
        }

        public int InitialTemperatureLow
        {
            get
            {
                return _lowTemperatureView.InitialTemperature;
            }

        }

        public int InitialTemperatureHigh
        {
            get
            {
                return _highTemperatureView.InitialTemperature;
            }

        }

        public Label MotorSpeedLabel
        {
            get
            {
                return _motorSpeedLabel;
            }

            set
            {
                _motorSpeedLabel = value;
            }
        }

        public AbstractSender Sender
        {
            get
            {
                return _sender;
            }

            set
            {
                _sender = value;
            }
        }

        public Button StartButton
        {
            get
            {
                return _startButton;
            }

            set
            {
                _startButton = value;
            }
        }

        public Button StopButton
        {
            get
            {
                return _stopButton;
            }

            set
            {
                _stopButton = value;
            }
        }

        internal ChartView ChartView
        {
            get
            {
                return _chartView;
            }

            set
            {
                _chartView = value;
            }
        }

        public ConnectionStatus ConnectionStatus
        {
            get
            {
                return _connectionStatus;
            }

            set
            {
                _connectionStatus = value;
            }
        }

        public MainView(AbstractSender sender)
        {
            Sender = sender;

            _referenceTemperaturesLabel = new Label()
            {
                Location = new Point(520, 10),
                Font = new Font("Arial", 15.0F),
                AutoSize = true,
                Text = "Reference temperatures:"
            };

            Controls.Add(_referenceTemperaturesLabel);

            // TODO: change locations dynamically
            _lowTemperatureView = new TemperatureView(22)
            {
                Location = new Point(450, 35)
            };
            Controls.Add(_lowTemperatureView);

            _highTemperatureView = new TemperatureView(22)
            {
                Location = new Point(650, 35)
            };
            Controls.Add(_highTemperatureView);

            StartButton = new Button();
            StartButton.BackColor = Color.White;
            StartButton.Location = new Point(570, 300);
            StartButton.Text = "Start";
            StartButton.Size = new Size(100, 30);
            StartButton.FlatStyle = FlatStyle.Flat;
            StartButton.Click += StartButton_Click;

            StopButton = new Button();
            StopButton.BackColor = Color.White;
            StopButton.Location = new Point(700, 300);
            StopButton.Text = "Stop";
            StopButton.Size = new Size(100, 30);
            StopButton.FlatStyle = FlatStyle.Flat;
            StopButton.Enabled = false;
            StopButton.Click += StopButton_Click;

            Controls.Add(StartButton);
            Controls.Add(StopButton);

            _automaticFanSpeedRegulationLabel = new Label
            {
                Location = new Point(530, 155),
                AutoSize = true,
                Text = "Automatic fan speed regulation:",
                Font = new Font("Arial", 10.0F)
            };

            Controls.Add(_automaticFanSpeedRegulationLabel);

            _automaticMotorSpeedCheckBox = new CheckBox()
            {
                Checked = false,
                Location = new Point(750, 150),
                FlatStyle = FlatStyle.Flat,
                Size = new Size(30,30)
            };

            _automaticMotorSpeedCheckBox.CheckedChanged += AutomaticMotorSpeedCheckBox_CheckedChanged;
            Controls.Add(_automaticMotorSpeedCheckBox);

            _fanSpeedLabel = new Label()
            {
                Location = new Point(490, 200),
                Font = new Font("Arial", 15.0F),
                AutoSize = true,
                Text = "Fan speed:"
            };

            Controls.Add(_fanSpeedLabel);

            MotorSpeedTrackBar = new TrackBar
            {
                Minimum = 0,
                Maximum = 100,
                Value = 20,
                SmallChange = 0,
                LargeChange = 20,
                TickFrequency = 20,
                Size = new Size(270, 40),
                TickStyle = TickStyle.Both,
                Location = new Point(480, 230),
                
            };

            MotorSpeedTrackBar.ValueChanged += MotorSpeedTrackBar_ValueChanged;
            MotorSpeedTrackBar.MouseWheel += MotorSpeedTrackBar_MouseWheel;
            MotorSpeedTrackBar.MouseUp += MotorSpeedTrackBar_MouseUp;
            Controls.Add(MotorSpeedTrackBar);

            MotorSpeedLabel = new Label()
            {
                AutoSize = true,
                Location = new Point(750, 230),
                ForeColor = System.Drawing.Color.Black,
                Text = MotorSpeedTrackBar.Value.ToString() + "%",
                Font = new Font("Arial", 20.0F)
            };

            Controls.Add(MotorSpeedLabel);

            ChartView = new ChartView()
            {
                Location = new Point(10, 10),
                Size = new Size(400, 300)
            };
            Controls.Add(ChartView);

            _lowTemperatureController = new TemperatureController(_lowTemperatureView);
            _highTemperatureController = new TemperatureController(_highTemperatureView);

            _connectionStatusTimer = new Timer
            {
                Interval = 500,
                Enabled = true
            };

            _connectionStatusTimer.Tick += ConnectionStatusTimer_Tick;

            _connectionStatusPanel = new Panel
            {
                Location = new Point(810, 358),
                Size = new Size(20, 20)
            };

            Controls.Add(_connectionStatusPanel);
        }

        private void ConnectionStatusTimer_Tick(object sender, EventArgs e)
        {
            if (ConnectionStatus == ConnectionStatus.Disconnected || !Sender.SerialPort.IsOpen)
            {

                _connectionStatusPanel.BackColor = Color.Red;
                Invalidate();

                string portId = Helpers.Helpers.AutodetectArduinoPort();
                if (portId != null)
                {
                    Sender.SerialPort.PortName = portId;
                }

                if (!Sender.SerialPort.IsOpen)
                {
                    try
                    {
                        Sender.SerialPort.Open();
                        ConnectionStatus = Constants.ConnectionStatus.Connected;
                    }
                    catch (Exception ex)
                    {
                        ConnectionStatus = Constants.ConnectionStatus.Disconnected;
                    }
                }
                else
                {
                    MessageBox.Show("The Serial Port is already open!");
                } 
            }
            else
            {
                _connectionStatusPanel.BackColor = Color.Green;
                Invalidate();
            }
        }

        private void AutomaticMotorSpeedCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            _mainController.DoSpeedCheckBoxCheckedChanged(!_automaticMotorSpeedCheckBox.Checked);
        }

        private void MotorSpeedTrackBar_MouseUp(object sender, MouseEventArgs e)
        {
            _mainController.DoMotorSpeedTrackBarMouseUp();
        }

        private void MotorSpeedTrackBar_MouseWheel(object sender, MouseEventArgs e)
        {
            _mainController.DoMotorSpeedTrackBarMouseWheel(e);
        }

        private void MotorSpeedTrackBar_ValueChanged(object sender, EventArgs e)
        {
            _mainController.DoMotorSpeedTrackbarValueChanged();
        }

        public void SetController(MainController mainController)
        {
            _mainController = mainController;
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            _mainController.DoStopButtonClick();
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
             if (_highTemperatureView.InitialTemperature < _lowTemperatureView.InitialTemperature)
            {

                if (_validationView != null)
                {
                    Controls.Remove(_validationView);
                    _validationView = null;
                }

                _validationView = new ValidationView()
                {
                    Size = new Size(Width, 20),
                    Location = new Point(0, 360)
                };

                Controls.Add(_validationView);
                _validationView.Activate("First temperature cannot be higher than second one");

            }
            else if (_highTemperatureView.InitialTemperature - _lowTemperatureView.InitialTemperature < 3)
            {

                if (_validationView != null)
                {
                    Controls.Remove(_validationView);
                    _validationView = null;
                }

                _validationView = new ValidationView()
                {
                    Size = new Size(Width, 20),
                    Location = new Point(0, 360)
                };

                Controls.Add(_validationView);
                _validationView.Activate("Difference between temperatures must be larger than 3");

            }
            else if (_connectionStatus == ConnectionStatus.Disconnected)
            {

                if (_validationView != null)
                {
                    Controls.Remove(_validationView);
                    _validationView = null;
                }

                _validationView = new ValidationView()
                {
                    Size = new Size(Width, 20),
                    Location = new Point(0, 360)
                };

                Controls.Add(_validationView);
                _validationView.Activate("Application is disconnected!");
            }
            else
            {
                if(_validationView != null)
                {
                    Controls.Remove(_validationView);
                    _validationView = null;
                }

                // TODO: handle dynamic creation of the control
                Controls.Remove(_chartView);

                int highValue = _highTemperatureView.InitialTemperature + 20;
                if (_highTemperatureView.InitialTemperature + 20 < 100)
                {
                    highValue = 100;
                }

                ChartView = new ChartView(0F, highValue, 5)
                {
                    Location = new Point(10, 10),
                    Size = new Size(400, 300)
                };

                Controls.Add(ChartView);

                _mainController.DoStartButtonClick(_lowTemperatureView.InitialTemperature, _highTemperatureView.InitialTemperature
                    , _motorSpeedTrackBar.Value, _automaticMotorSpeedCheckBox.Checked);

            }
        }

        public void Receive(float point, float point2, string time)
        {
            float difference = _highTemperatureView.InitialTemperature - _lowTemperatureView.InitialTemperature;
            ChartView.DrawPoint(point2, point , time);
        }
    }
}
