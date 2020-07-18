using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace MachanismAnalysis.GUI
{
    public partial class CommonChartForm : Form
    {
        public CommonChartForm(string title,string[] series,double min,double max)
        {

            //StartPosition = FormStartPosition.Manual;


            //Location = new Point(1200, 20);

            InitializeComponent();

            InitializeChart(series,min,max);
            Text = title;

        }

        public void AddPoint(double angle,double[] values,bool clear = false)
        {

            if (clear) Clear();

            if (values.Length != chartMain.Series.Count) return;

            for (int i = 0; i < values.Length; i++)
            {
                chartMain.Series[i].Points.AddXY(angle, values[i]);
                  
            }

        }

        private void Clear()
        {
            for (int i = 0; i < chartMain.Series.Count; i++)
            {
                chartMain.Series[i].Points.Clear();
            }
        }

        private void InitializeChart(string[] series,double min,double max)
        {
            chartMain.ChartAreas[0].AxisX.Minimum = 0;
            chartMain.ChartAreas[0].AxisX.Maximum = 2 * Math.PI;
            chartMain.ChartAreas[0].AxisX.Name = "角度 (rad)";

            chartMain.ChartAreas[0].AxisY.Crossing = 0;
            chartMain.ChartAreas[0].AxisX.MajorGrid.Enabled = false;
            chartMain.ChartAreas[0].AxisX.Interval = 1;
            chartMain.ChartAreas[0].AxisY.MajorGrid.Enabled = false;



            chartMain.ChartAreas[0].AxisY.Minimum = min;
            chartMain.ChartAreas[0].AxisY.Maximum = max;

            chartMain.Series.Clear();

            foreach (var item in series)
            {
                chartMain.Series.Add(item);
            }

            for (int i = 0; i < series.Length; i++)
            {
                chartMain.Series[i].ChartType = SeriesChartType.Line;

                chartMain.Series[i].BorderWidth = 3;      

            }


        }
    }
}
