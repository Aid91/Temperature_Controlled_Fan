using ESTemperatureRegulator.Views;

namespace ESTemperatureRegulator.Controllers
{
    class TemperatureController
    {
        private TemperatureView _temperatureView;

        public TemperatureController(TemperatureView temperatureView)
        {
            _temperatureView = temperatureView;
            _temperatureView.SetController(this);
        }

        internal void DoMinusButtonClick()
        {
            // TODO: add logic for lower bound

            if (_temperatureView.InitialTemperature > Constants.Constants.MINIMAL_TEMPERATURE)
            {
                _temperatureView.InitialTemperature--;
                _temperatureView.SetTemperature();
            }
        }

        internal void DoPlusButtonClick()
        {
            // TODO: add logic for upper bound

            if(_temperatureView.InitialTemperature < Constants.Constants.MAXIMAL_TEMPERATURE)
            {
                _temperatureView.InitialTemperature++;
                _temperatureView.SetTemperature();
            }
        }
    }
}
