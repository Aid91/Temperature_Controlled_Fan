using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ESTemperatureRegulator.Interfaces
{
    public class RTOSSender : AbstractSender
    {
        public override void Start(float startTemperature, float endTemperature, int pwmRation, bool auto = false)
        {
            if (SerialPort != null && SerialPort.IsOpen)
            {
                pwmRation = (auto == true) ? 0 : pwmRation;
                string send = "start," + (int)startTemperature + "," + (int)endTemperature + "," + pwmRation + ",#";
                SerialPort.Write(send);
            }
        }

        public override void Stop()
        {
            if (SerialPort != null && SerialPort.IsOpen)
            {
                SerialPort.Write("stop#");
                Thread.Sleep(100);
            }
        }
    }
}
