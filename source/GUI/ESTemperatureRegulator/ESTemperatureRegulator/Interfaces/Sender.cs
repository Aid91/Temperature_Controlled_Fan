using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;

namespace ESTemperatureRegulator.Interfaces
{
    /// <summary>
    /// Implementation of the communication specific logic -> to embedded system
    /// </summary>
    public class Sender : AbstractSender
    {
        public override void Start(float startTemperature, float endTemperature, int pwmRation, bool auto = false)
        {
            if (SerialPort != null && SerialPort.IsOpen)
            {
                // TODO: add sending logic
                SerialPort.Write("start#");

                //Thread.Sleep(50);
                SerialPort.Write(startTemperature + "\n");
                SerialPort.Write(endTemperature + "\n");

                if (auto)
                {
                    SerialPort.Write(0.0 + "\n");
                }
                else
                {
                    SerialPort.Write(pwmRation + "\n");
                }
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
