using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ESTemperatureRegulator.Interfaces
{
    public interface IReceiver
    {
        void Receive(float point, float point2, string time);
    }
}
