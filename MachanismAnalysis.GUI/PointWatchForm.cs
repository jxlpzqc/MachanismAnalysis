using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MachanismAnalysis.GUI
{
    public partial class PointWatchForm : Form
    {
        private int selectedPoint;

        public class DoubleConverter : TypeConverter
        {

            public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
            {
                return true;
            }

            public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
            {
                return 0;
            }

            public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
            {
                
                double num = (double)value;
                return num.ToString("F6");
            }
        }

        public class PointInfoDisplay
        {
            [Description("该点在X轴的位置")]
            [Category("运动学信息")]
            [TypeConverter(typeof(DoubleConverter))]
            public double PositionX { get; internal set; } = 0;

            [Description("该点在Y轴的位置")]
            [Category("运动学信息")]
            [TypeConverter(typeof(DoubleConverter))]
            public double PositionY { get; internal set; } = 0;

            [Description("该点在X轴的速度")]
            [TypeConverter(typeof(DoubleConverter))]
            [Category("运动学信息")]
            public double VelocityX { get; internal set; } = 0;

            [Description("该点在Y轴的速度")]
            [TypeConverter(typeof(DoubleConverter))]
            [Category("运动学信息")]
            public double VelocityY { get; internal set; } = 0;

            [Description("该点在X轴的加速度")]
            [TypeConverter(typeof(DoubleConverter))]
            [Category("运动学信息")]
            public double AccelerationX { get; internal set; } = 0;

            [Description("该点在Y轴的加速度")]
            [TypeConverter(typeof(DoubleConverter))]
            [Category("运动学信息")]
            public double AccelerationY { get; internal set; } = 0;

            [Description("该点在X轴的约束反力分量")]
            [TypeConverter(typeof(DoubleConverter))]
            [Category("动力学信息")]
            public double ForceX { get; internal set; } = 0;

            [Description("该点在Y轴的约束反力分量")]
            [TypeConverter(typeof(DoubleConverter))]
            [Category("动力学信息")]
            public double ForceY { get; internal set; } = 0;

        }

        private PointInfoDisplay info = new PointInfoDisplay();

        private Core.Caculator cac;

        private bool _lock = false;

        public bool Lock
        {
            get => _lock;
            set
            {
                _lock = value;
                if (value)
                {
                    Opacity = 1.0;
                }
                else
                {
                    Opacity = 0.7;
                }
            }
        }


        public PointWatchForm(int n, Core.Caculator c)
        {
            InitializeComponent();

            selectedPoint = n;
            pgMain.SelectedObject = info;
            cac = c;
            Text += " - 点" + n.ToString();

        }

        public void UpdateInfo()
        {
            var p = cac.GetPointsPosition(selectedPoint);
            var vp = cac.GetPointsVelocity(selectedPoint);
            var ap = cac.GetPointsAcceleration(selectedPoint);
            var fr = cac.GetPointForce(selectedPoint);
            info.PositionX = p.x;
            info.PositionY = p.y;
            info.VelocityX = vp.x;
            info.VelocityY = vp.y;
            info.AccelerationX = ap.x;
            info.AccelerationY = ap.y;
            info.ForceX = fr.x;
            info.ForceY = fr.y;
            pgMain.Refresh();
            

        }

     

        private void PointWatchForm_GotFocus(object sender, System.EventArgs e)
        {
            Lock = true;
        }
    }
}
