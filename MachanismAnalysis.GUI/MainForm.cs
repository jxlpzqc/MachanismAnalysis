using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MachanismAnalysis.GUI
{
    class MainForm:Form
    {
        private const int initHeight = 600;
        private const int initWidth = 800;
        

        public MainForm()
        {
            Text = "机械原理课程设计 - 图形用户界面 - 程子丘";
            Height = initHeight;
            Width = initWidth;
            BackColor = Color.White;

            DoubleBuffered = true;
            SetStyle(ControlStyles.UserPaint,true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);

            UpdateStyles();

            Graphics = CreateGraphics();

            Timer.Tick += (obj,e) =>
            {
                Draw();

            };
            Timer.Start();
        }

        public Timer Timer { get; set; } = new Timer()
        {
            Interval = 13            
        };

        private int realHeight = 100;
        private int realWidth = 100;

        private Point CreatePoint(double realX, double realY)
        {
            //TODO 修复缩放问题

            return new Point()
            {
                X = (int) ConvertToScreenX(realX),
                Y = (int) ConvertToScreenY(realY)
            };
        }

        private double ConvertToScreenX(double realX)
        {
            return (realX / realWidth * Width);
        }
        private double ConvertToScreenY(double realY)
        {
            
            return (realY / realHeight * Height);
        }


        private static class Pens
        {
            public static Pen MechanismPen = new Pen(Color.Black);

        }

        /// <summary>
        /// 根据图像指定画布大小
        /// </summary>
        private void DetermineCanvasSize()
        {
            // TODO 完成他

        }


        private Graphics Graphics { get; }




        private void DrawFrame(double x,double y)
        {
            Graphics.DrawArc(Pens.MechanismPen, new Rectangle(CreatePoint(x - 10, y - 10), new Size(10, 10)), 0, 360);



        }

        public void Draw()
        {
            Graphics.Clear(Color.White);

            Graphics.DrawLine(new Pen(Color.Red) { 
                Width = 20
            }, CreatePoint(10, 20), CreatePoint(20, 30));


        }


    }
}
