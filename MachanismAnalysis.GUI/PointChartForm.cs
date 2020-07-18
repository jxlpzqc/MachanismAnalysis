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
    public partial class PointChartForm : Form
    {
        public PointChartForm()
        {

            StartPosition = FormStartPosition.Manual;

            Location = new Point(1200, 20);

            InitializeComponent();

            InitializeChart();

        }

        public void AddPoint(double angle,Core.Point pos,Core.Point vec,Core.Point acc,bool clear = false)
        {
            if (clear) Clear();

            chartMain.Series[0].Points.AddXY(angle, pos.x);
            chartMain.Series[1].Points.AddXY(angle, pos.y);
            chartMain.Series[2].Points.AddXY(angle, vec.x);
            chartMain.Series[3].Points.AddXY(angle, vec.y);
            chartMain.Series[4].Points.AddXY(angle, acc.x);
            chartMain.Series[5].Points.AddXY(angle, acc.y);

        }

        private void Clear()
        {
            for (int i = 0; i < 6; i++)
            {
                chartMain.Series[i].Points.Clear();
            }
        }

        private void InitializeChart()
        {
            chartMain.ChartAreas[0].AxisX.Minimum = 0;
            chartMain.ChartAreas[0].AxisX.Maximum = 2 * Math.PI;
            chartMain.ChartAreas[0].AxisX.Name = "角度 (rad)";

            chartMain.ChartAreas[0].AxisY.Minimum = -5;
            chartMain.ChartAreas[0].AxisY.Maximum = 5;

            chartMain.Series.Clear();
            chartMain.Series.Add("位置X");
            chartMain.Series.Add("位置Y");
            chartMain.Series.Add("速度X");
            chartMain.Series.Add("速度Y");
            chartMain.Series.Add("加速度X");
            chartMain.Series.Add("加速度Y");
            for (int i = 0; i < 6; i++)
            {
                chartMain.Series[i].ChartType = SeriesChartType.Line;

                chartMain.Series[i].BorderWidth = 3;      

            }


        }
    }
}
