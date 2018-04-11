using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Timers;
using System.Threading.Tasks;
using System.IO.Ports;
using ESTemperatureRegulator.Views;

namespace ESTemperatureRegulator.Interfaces
{
    public class Receiver
    {
        private IReceiver _communicationFromEmbedded;

        private SerialPort _serialPort;

        public Receiver(IReceiver embedded, SerialPort serialPort)
        {
            _communicationFromEmbedded = embedded;
            _serialPort = serialPort;
            _serialPort.DataReceived += SerialPort_DataReceived;
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // implement reading logic
            string data = _serialPort.ReadTo("\x03");//Read until the EOT code
                                                     //Split into 'date=temparature' formatted text

            string[] dataArray = data.Split(new string[] { "\x02", "$" }, StringSplitOptions.RemoveEmptyEntries);
            // Receive()

            if (dataArray.Length == 2)
            {
                float temperature = 0.0F;
                if (float.TryParse(dataArray[0], out temperature))
                {
                    Console.WriteLine("Received: " + temperature);
                }
                else
                {
                    // TODO log error
                }

                float fanSpeed = 0.0F;
                if (float.TryParse(dataArray[1], out fanSpeed))
                {
                    Console.WriteLine("Received: " + fanSpeed);
                }
                else
                {
                    // TODO log error
                }

                _communicationFromEmbedded.Receive(temperature, fanSpeed, "1000");
            }
            else
            {
                // TODO log error
            }
        }
    }
}
