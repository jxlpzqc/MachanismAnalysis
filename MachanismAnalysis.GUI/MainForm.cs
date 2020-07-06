using MachanismAnalysis.Core;
using System;
using System.Drawing;
using System.Windows.Forms;
using Point = System.Drawing.Point;

namespace MachanismAnalysis.GUI
{
    class MainForm : Form
    {
        private const int initHeight = 800;
        private const int initWidth = 800;

        private Caculator caculator = new Caculator(4, 3);

        // TODO Local 
        private double ppp = 0;

        public MainForm()
        {
            Text = "机械原理课程设计 - 图形用户界面 - 程子丘";
            Height = initHeight;
            Width = initWidth;
            BackColor = Color.White;

            DoubleBuffered = true;
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);


            UpdateStyles();

            var s = new double[2][];

            caculator.SetPointsPosition(0, 0, 0);
            caculator.SetPointsVelocity(0, 0, 0);
            caculator.SetPointsAcceleration(0, 0, 0);

            caculator.SetPointsPosition(3, 50, 0);
            caculator.SetPointsVelocity(3, 0, 0);
            caculator.SetPointsAcceleration(3, 0, 0);

            caculator.SetRodsAngularDisplacement(0, 0);
            caculator.SetRodsAngularVelocity(0, 20);
            caculator.SetRodsAngularVelocity(0, 0);

            var arr = new int[] { 1, 2, 4 };

            // TODO issue 尝试使用多线程可以解决用户消息响应缓慢问题 (application take long time to respond)

            Timer.Tick += (obj, e) =>
            {
                ppp = (ppp + 0.05) % (2 * Math.PI);

                caculator.SetRodsAngularDisplacement(0, ppp);
                caculator.BarKinematic(0, 1, 0, 20, 0);
                caculator.RRRKinematic(1, 1, 3, 2, 1, 2, 40, 30);


                using (bitmap = new Bitmap(Width, Height))
                {
                    using (graphics = Graphics.FromImage(bitmap))
                    {
                        // 开始绘图
                        Draw();
                        // 应用绘图
                        CreateGraphics().DrawImage(bitmap, new System.Drawing.Point
                        {
                            X = 0,
                            Y = 0
                        });


                    }
                }
            };
            Timer.Start();
        }

        public Timer Timer { get; set; } = new Timer()
        {
            Interval = 20
        };

        private float RealHeight
        {
            get
            {
                return maxY - minY;
            }
        }

        private float RealWidth
        {
            get
            {
                return maxX - minX;
            }
        }

        private float maxY = 80;
        private float minY = -50;
        private float maxX = 80;
        private float minX = -50;


        private System.Drawing.Point CreatePoint(float realX, float realY)
        {
            return new System.Drawing.Point()
            {
                X = (int)ConvertToScreenX(realX),
                Y = (int)ConvertToScreenY(realY)
            };
        }

        private PointF CreatePointF(float realX, float realY)
        {
            return new PointF()
            {
                X = (float)ConvertToScreenX(realX),
                Y = (float)ConvertToScreenY(realY)
            };
        }

        private PointF CreatePointF(Core.Point p)
        {
            return new PointF()
            {
                X = (float)ConvertToScreenX((float)p.x),
                Y = (float)ConvertToScreenY((float)p.y)
            };
        }

        private float ConvertToScreenY(float realY)
        {
            // 坐标系转换

            // 原点屏幕坐标Y  ( Height - (0-minY) * Height/(maxY-minY))
            // 实际坐标 是 原点偏移 ( Height - (0-minY) * Height/(maxY-minY)) - (realY) * Height/(maxY-minY)

            var ratio = Height / (maxY - minY);
            return Height + (minY - realY) * ratio;

        }

        private float ConvertToScreenX(float realX)
        {

            // 坐标系转换

            // 原点屏幕坐标X  ( (0-minX) * ratio)
            // 实际坐标 是 原点偏移 ( -minX * ratio + realX * ratio)

            var ratio = Width / (maxX - minX);
            return (realX - minX) * ratio;

        }


        /// <summary>
        /// 画笔静态类，用来画轮廓
        /// </summary>
        private static class Pens
        {
            public static Pen MechanismPen = new Pen(Color.Black, 3);
            public static Pen RodPen = new Pen(Color.Gray, 15) { 
                
                DashStyle = System.Drawing.Drawing2D.DashStyle.Solid,
                
         
            };

        }

        /// <summary>
        /// 画刷静态类，用来填充
        /// </summary>
        private static class Brushes
        {

            public static Brush RevolveBrush = new SolidBrush(Color.Red);

        }

        /// <summary>
        /// 根据图像指定画布大小
        /// </summary>
        private void DetermineCanvasSize()
        {
            // TODO 完成他

        }



        private Bitmap bitmap;
        private Graphics graphics;


        private void DrawRod(Core.Point a, Core.Point b)
        {

            graphics.DrawLine(Pens.RodPen, CreatePointF(a), CreatePointF(b));
        }


        private void DrawPoint(Core.Point p)
        {

            float x = (float)p.x;
            float y = (float)p.y;

            graphics.DrawArc(Pens.MechanismPen, new Rectangle(new System.Drawing.Point
            {
                X = (int)ConvertToScreenX(x) - 8,
                Y = (int)ConvertToScreenY(y) - 8
            }, new Size(16, 16)), 0, 360);

            graphics.FillEllipse(Brushes.RevolveBrush, new Rectangle(new System.Drawing.Point
            {
                X = (int)ConvertToScreenX(x) - 8,
                Y = (int)ConvertToScreenY(y) - 8
            }, new Size(16, 16)));



        }

        private void DrawFrame(Core.Point p)
        {

            float x = (float)p.x;
            float y = (float)p.y;
            float h = 18f;
            float w = h / 1.13f;

            graphics.DrawPolygon(Pens.MechanismPen, new PointF[]
            {
                new PointF
                {
                    X = ConvertToScreenX(x),
                    Y = ConvertToScreenY(y)
                },
                // 作等边三角形
                new PointF
                {
                    X = ConvertToScreenX(x) + w/2,
                    Y = ConvertToScreenY(y) + h
                },
                new PointF
                {
                    X = ConvertToScreenX(x) - w/2,
                    Y = ConvertToScreenY(y) + h
                },
                new PointF
                {
                    X = ConvertToScreenX(x),
                    Y = ConvertToScreenY(y)
                },

            }
            );


            DrawPoint(p);

            var nx = ConvertToScreenX(x) - w / 2;
            var ny = ConvertToScreenY(y) + h;
            int n = 3;
            for (int i = 0; i <= n; i++)
            {
                graphics.DrawLine(Pens.MechanismPen, new PointF(nx, ny), new PointF(nx - 5, ny + 8));
                nx += h / n;
            }


        }


        private void DrawRRR()
        {

        }


        public void Draw()
        {

            graphics.Clear(Color.White);

            DrawRod(caculator.GetPointsPosition(0), caculator.GetPointsPosition(1));
            DrawRod(caculator.GetPointsPosition(1), caculator.GetPointsPosition(2));
            DrawRod(caculator.GetPointsPosition(2), caculator.GetPointsPosition(3));

            DrawPoint(caculator.GetPointsPosition(1));
            DrawPoint(caculator.GetPointsPosition(2));

            DrawFrame(caculator.GetPointsPosition(0));
            DrawFrame(caculator.GetPointsPosition(3));
        }


    }
}
