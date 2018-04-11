using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ESTemperatureRegulator.Interfaces
{
    public abstract class AbstractSender
    {
        private SerialPort _serialPort;

        public virtual SerialPort SerialPort
        {
            get
            {
                return _serialPort;
            }
            set
            {
                _serialPort = value;
            }
        }

        public abstract void Start(float startTemperature, float endTemperature, int pwmRation, bool auto=false);

        public abstract void Stop();
    }
}
