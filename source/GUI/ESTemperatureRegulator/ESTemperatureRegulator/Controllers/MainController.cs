using ESTemperatureRegulator.Views;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ESTemperatureRegulator.Controllers
{
    class MainController
    {
        private MainView _mainView;

        public MainController(MainView mainView)
        {
            _mainView = mainView;
            _mainView.SetController(this);
        }

        internal void DoMotorSpeedTrackBarMouseUp()
        {
            _mainView.MotorSpeedTrackBar.Value = (int)(Math.Round(_mainView.MotorSpeedTrackBar.Value / 20.0) * 20);
        }

        internal void DoMotorSpeedTrackBarMouseWheel()
        {
            throw new NotImplementedException();
        }

        internal void DoMotorSpeedTrackbarValueChanged()
        {
            _mainView.MotorSpeedLabel.Text = _mainView.MotorSpeedTrackBar.Value.ToString() + "%";
        }

        internal void DoStopButtonClick()
        {
            _mainView.Sender.Stop();
            _mainView.StartButton.Enabled = true;
            _mainView.StopButton.Enabled = false;
        }

        internal void DoStartButtonClick()
        {
            throw new NotImplementedException();
        }

        internal void DoSpeedCheckBoxCheckedChanged(bool enabled)
        {
            _mainView.MotorSpeedTrackBar.Enabled = enabled;
        }

        internal void DoMotorSpeedTrackBarMouseWheel(MouseEventArgs e)
        {
            HandledMouseEventArgs hme = (HandledMouseEventArgs)e;
            hme.Handled = true;
        }
    
        internal void DoStartButtonClick(int lowTemperature, int highTemperature, int fanSpeed, bool @checked)
        {
            _mainView.Sender.Start(lowTemperature, highTemperature, fanSpeed, @checked);

            // set width
            //_mainView.ChartView.SetTemperatureRangeLength((float)(highTemperature - lowTemperature));

            _mainView.StartButton.Enabled = false;
            _mainView.StopButton.Enabled = true;
        }
    }
}
