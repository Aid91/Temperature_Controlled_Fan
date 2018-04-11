using Graph;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ESTemperatureRegulator.Views
{
    class ChartView : Panel
    {
        private LineGraph _chart;

        public ChartView()
        {
            Initalize(0F, 50F, 5);
        }

        public ChartView(float min, float max, int yInterval)
        {
            Initalize(min, max, yInterval);
        }

        private void Initalize(float min, float max, int yInterval)
        {
            _chart = new LineGraph();
            _chart.Dock = DockStyle.Fill;
            _chart.BackgroundColor = Color.Black;

            _chart.BackgroundColor = System.Drawing.Color.White;
            _chart.BufferSize = 4000;
            _chart.Graph1Name = "Fan speed";
            _chart.Graph2Name = "Temperature";
            _chart.GridSpacingX = 20;
            _chart.GridSpacingY = 37;
            _chart.IsBWVisible = false;
            _chart.IsLogarithmic = false;
            _chart.IsSDVisible = false;
            _chart.IsSMAVisible = false;
            _chart.LogarithmicIncFactor = 4F;
            _chart.LogarithmicMax = max;
            _chart.LogarithmicMin = 0.1F;
            _chart.Max = max;
            _chart.Min = min;
            _chart.Name = "_chartView";
            _chart.NoOfGraphs = 3;
            _chart.NoOfYGrids = (int)(max - min)/yInterval;
            _chart.PlotLine1Color = System.Drawing.Color.CornflowerBlue;
            _chart.PlotLine1Width = 2F;
            _chart.PlotLine2Color = System.Drawing.Color.OrangeRed;
            _chart.PlotLine2Width = 2F;
            _chart.TabIndex = 0;
            _chart.XAxisBackColor = System.Drawing.Color.Transparent;
            _chart.XAxisHeader = "Time";
            _chart.XGridColor = System.Drawing.Color.SeaGreen;
            _chart.XValueDisplayInterval = 5;
            _chart.YAxisBackColor = System.Drawing.Color.Transparent;
            _chart.YAxisHeader = "T\ne\nm\np\ne\nr\na\nt\nu\nr\ne\n";
            _chart.YGridColor = System.Drawing.Color.SeaGreen;
            _chart.YGridInterval = yInterval;

            _chart.ForeColor = Color.Black;

            Controls.Add(_chart);
        }

        public void DrawPoint(float point1, float point2, string time)
        {
            _chart.Add(point1, point2, time);
        }

        public void SetTemperatureRangeLength(float difference)
        {
            float pixelTemperatureRatio = (_chart.Height / _chart.Max);
            _chart.PlotLine1Width = pixelTemperatureRatio * difference - pixelTemperatureRatio;
        }
    }
}
