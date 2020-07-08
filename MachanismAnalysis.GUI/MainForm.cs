using MachanismAnalysis.Core;
using MachanismAnalysis.GUI.Properties;
using System;
using System.Drawing;
using System.Threading.Tasks;
using System.Windows.Forms;
using Point = System.Drawing.Point;

namespace MachanismAnalysis.GUI
{
    class MainForm : Form
    {
        #region Size of the canvas

        private const int initHeight = 768;
        private const int initWidth = 1024;

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

        private float maxY = 700;
        private float minY = -200;
        private float maxX = 800;
        private float minX = -400;

        #endregion

        #region Member fields and properties

        private Caculator caculator = new Caculator(6, 6);

        private Bitmap bitmap;
        private Graphics graphics;
        private PointChartForm chart;

        public Timer Timer { get; set; } = new Timer()
        {
            Interval = 20
        };


        #endregion

        #region Some constances of machanism design

        private const double lenO1A = 90.52;
        private const double lenO2B = 552.34;
        private const double rate = 0.32;
        private const double heightDC = (538 + lenO2B) / 2;

        #endregion



        // current angular position
        private double currentPosition = 0;


        /// <summary>
        /// Constructor of main window
        /// </summary>
        /// <param name="speed">主动件运动速度，unit -> r/min</param>
        public MainForm(double speed = 1000)
        {
            #region 初始化窗体属性
            Text = "机械原理课程设计 - 程子丘 - 图形用户界面 ";
            Height = initHeight;
            Width = initWidth;
            BackColor = Color.White;

            DoubleBuffered = true;
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.ResizeRedraw, true);

            Icon = Resources.MainIcon;

            UpdateStyles();


            MaximumSize = new Size(initWidth, initHeight);
            MinimumSize = new Size(initWidth, initHeight);



            var btn = new Button()
            {
                Size = new Size(80, 40),
                Text = "播放/暂停",
                Location = new Point(initWidth - 180, initHeight - 140),

                Anchor = AnchorStyles.Right | AnchorStyles.Bottom,
            };

            Controls.Add(btn);
            btn.Click += (o, e) =>
            {
                Toggle();
            };


            chart = new PointChartForm();
            chart.Show(this);



            #endregion

            // 由 r/min -> rad/s
            var rad = speed / 60 * 2 * Math.PI;

            // 初始化 点信息 和主动件信息
            InitializeInfoes(rad);


            bitmap = new Bitmap(Width, Height);

            graphics = Graphics.FromImage(bitmap);
            
            // 开启抗锯齿
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            // TODO issue 尝试使用多线程可以解决用户消息响应缓慢问题 (application take long time to respond)
            Timer.Tick += (obj, e) =>
            {

                currentPosition = (currentPosition - 0.05) % (2 * Math.PI);
                CaculateKinematicInfo();

                // 开始绘图
                Draw();

                // 双缓冲
                Invalidate(new Rectangle(0, 0, Width, Height));
                Update();

            };


            Console.WriteLine("程序初始化完成");

            Timer.Start();
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            if (bitmap == null) return;
            var screenG = e.Graphics;
            screenG.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
            screenG.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
            screenG.DrawImage(bitmap, new System.Drawing.Point
            {
                X = 0,
                Y = 0
            });

        }


        #region 坐标转换

        private Point CreatePoint(float realX, float realY)
        {
            return new Point()
            {
                X = (int)ConvertToScreenX(realX),
                Y = (int)ConvertToScreenY(realY)
            };
        }

        private PointF CreatePointF(double realX, double realY)
        {
            return new PointF()
            {
                X = ConvertToScreenX(realX),
                Y = ConvertToScreenY(realY)
            };
        }

        private PointF CreatePointF(Core.Point p)
        {
            return CreatePointF(p.x, p.y);
        }

        private float ConvertToScreenY(double realY)
        {
            // 坐标系转换

            // 原点屏幕坐标Y  ( Height - (0-minY) * Height/(maxY-minY))
            // 实际坐标 是 原点偏移 ( Height - (0-minY) * Height/(maxY-minY)) - (realY) * Height/(maxY-minY)

            var ratio = Height / (maxY - minY);
            return Height + (minY - (float)realY) * ratio;

        }

        private float ConvertToScreenX(double realX)
        {

            // 坐标系转换

            // 原点屏幕坐标X  ( (0-minX) * ratio)
            // 实际坐标 是 原点偏移 ( -minX * ratio + realX * ratio)

            var ratio = Width / (maxX - minX);
            return ((float)realX - minX) * ratio;

        }

        private PointF RotatePoint(PointF origin, PointF delta, double angle)
        {
            var x = origin.X;
            var y = origin.Y;
            var dx = delta.X;
            var dy = delta.Y;

            var newx = x + dx * Math.Sin(angle) - dy * Math.Cos(angle);
            var newy = y + dy * Math.Sin(angle) + dx * Math.Cos(angle);
            return new PointF((float)newx, (float)newy);

        }

        #endregion

        #region 预置画笔和画刷

        /// <summary>
        /// 画笔静态类，用来画轮廓
        /// </summary>
        private static class Pens
        {
            public static readonly Pen MechanismPen = new Pen(Color.Black, 3);
            public static readonly Pen RodPen = new Pen(Color.FromArgb(180,0,0,0), 15)
            {
                DashStyle = System.Drawing.Drawing2D.DashStyle.Solid,


            };

        }

        /// <summary>
        /// 画刷静态类，用来填充
        /// </summary>
        private static class Brushes
        {

            public static readonly Brush RevolveBrush = new SolidBrush(Color.White);
            public static readonly Brush MoveBrush = new SolidBrush(Color.LightYellow);
            public static readonly Brush TextBrush = new SolidBrush(Color.FromArgb(200, 255, 0, 0));

        }

        #endregion

        ///// <summary>
        ///// 根据图像指定画布大小
        ///// </summary>
        //private void DetermineCanvasSize()
        //{
        //    // TODO 完成他

        //}

        #region 播放状态控制

        public void Pause()
        {
            Timer.Stop();
        }

        public void Play()
        {
            Timer.Start();
        }


        public void Toggle()
        {
            if (Timer.Enabled)
            {
                Pause();
            }
            else
            {
                Play();
            }

        }

        #endregion

        #region 绘图核心

        private void DrawRod(Core.Point a, Core.Point b,Pen pen = null)
        {
            if (pen == null) pen = Pens.RodPen;

            graphics.DrawLine(pen, CreatePointF(a), CreatePointF(b));
        }
              
        private void DrawPrismaticPair(Core.Point p, double angle)
        {


            var points = new PointF[]
            {
                RotatePoint(CreatePointF(p),new PointF(10, 20),angle),
                RotatePoint(CreatePointF(p),new PointF(10, -20),angle),
                RotatePoint(CreatePointF(p),new PointF(-10, -20),angle),
                RotatePoint(CreatePointF(p),new PointF(-10, 20),angle),
                RotatePoint(CreatePointF(p),new PointF(10, 20),angle),

            };

            graphics.DrawPolygon(Pens.MechanismPen, points);

            graphics.FillPolygon(Brushes.MoveBrush, points);


        }

        private void DrawRevolutePair(Core.Point p, int? n = null)
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

            if (n != null)
            {
                graphics.DrawString(n.ToString(), new Font("Microsoft Yahei", 14), Brushes.TextBrush, new PointF
                {

                    X = (int)ConvertToScreenX(x) + 20,
                    Y = (int)ConvertToScreenY(y)
                });
            }


        }

        private void DrawPrismaticFrame(Core.Point p)
        {

            float x = (float)p.x;
            float y = (float)p.y;

            int len = 100;
            int width = 22;

            graphics.DrawLine(Pens.MechanismPen,
                new PointF
                {
                    X = ConvertToScreenX(x) - len / 2,
                    Y = ConvertToScreenY(y) - width / 2
                },
                // 作等边三角形
                new PointF
                {
                    X = ConvertToScreenX(x) + len / 2,
                    Y = ConvertToScreenY(y) - width / 2
                }
            );


            graphics.DrawLine(Pens.MechanismPen,
                new PointF
                {
                    X = ConvertToScreenX(x) - len / 2,
                    Y = ConvertToScreenY(y) + width / 2
                },
                // 作等边三角形
                new PointF
                {
                    X = ConvertToScreenX(x) + len / 2,
                    Y = ConvertToScreenY(y) + width / 2
                }
            );



            var nx = ConvertToScreenX(x) - len / 2;
            var ny1 = ConvertToScreenY(y) + width / 2;
            var ny2 = ConvertToScreenY(y) - width / 2;
            int n = 10;
            for (int i = 0; i <= n; i++)
            {
                graphics.DrawLine(Pens.MechanismPen, new PointF(nx, ny1), new PointF(nx - 5, ny1 + 8));
                graphics.DrawLine(Pens.MechanismPen, new PointF(nx, ny2), new PointF(nx + 5, ny2 - 8));
                nx += len / n;
            }


        }

        private void DrawFrame(Core.Point p,int? num = null)
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


            DrawRevolutePair(p, num);

            var nx = ConvertToScreenX(x) - w / 2;
            var ny = ConvertToScreenY(y) + h;
            int n = 3;
            for (int i = 0; i <= n; i++)
            {
                graphics.DrawLine(Pens.MechanismPen, new PointF(nx, ny), new PointF(nx - 5, ny + 8));
                nx += h / n;
            }


        }


        #endregion

        #region 绘图重载封装

        private void DrawRod(int n1, int n2)
        {
            DrawRod(caculator.GetPointsPosition(n1), caculator.GetPointsPosition(n2));
        }

        private void DrawRevolutePair(int n)
        {
            DrawRevolutePair(caculator.GetPointsPosition(n), n);
        }

        private void DrawPrismaticFrame(int n)
        {
            DrawPrismaticFrame(caculator.GetPointsPosition(n));
        }

        private void DrawFrame(int n)
        {
            DrawFrame(caculator.GetPointsPosition(n),n);
        }

        private void DrawPrismaticPair(int n,int p)
        {

            DrawPrismaticPair(caculator.GetPointsPosition(n), caculator.GetRodsAngularDisplacement(p));

        }


        #endregion



        private void DrawPointChart(int n)
        {
            var num = Math.Abs(currentPosition);

            chart.AddPoint(num, caculator.GetPointsPosition(n), caculator.GetPointsVelocity(n), caculator.GetPointsAcceleration(n), num < 0.06);

        }







        #region 业务逻辑

        private void InitializeInfoes(double rad)
        {
            caculator.ConfigurePoint(1, 0, 0, 0, 0, 0, 0);

            caculator.ConfigurePoint(2, 0, 400, 0, 0, 0, 0);

            caculator.ConfigurePoint(6, 0, heightDC, 0, 0, 0, 0);

            caculator.ConfigureRod(1, 0, rad, 0);

            caculator.ConfigureRod(6, 0, 0, 0);
        }

        private void CaculateKinematicInfo()
        {
            // 设置值
            caculator.SetRodsAngularDisplacement(1, currentPosition);

            // 主动件
            caculator.BarKinematic(2, 3, 1, lenO1A, 0);


            // RPR杆组
            double r2 = 0, vr2 = 0, ar2 = 0;

            caculator.RPRKinematic(1, 1, 3, 2, 3, 1, ref r2, ref vr2, ref ar2);

            // 求4点

            caculator.BasicPointKinematic(1, 4, 2, lenO2B);

            //var theta = caculator.GetRodsAngularDisplacement(1);
            //var p1 = caculator.GetPointsPosition(0);
            //var OB_2 = lenOB_2;
            //var O_1A = lenO_1A;


            //caculator.SetPointsPosition(3, p1.x + OB_2 * Math.Cos(theta), p1.y + OB_2 * Math.Sin(theta));
            //caculator.SetPointsVelocity(3, p1.x + OB_2 * Math.Cos(theta), p1.y + OB_2 * Math.Sin(theta));
            //caculator.SetPointsAcceleration(3, p1.x + OB_2 * Math.Cos(theta), p1.y + OB_2 * Math.Sin(theta));



            // RRP杆组
            double r2_ = 0, vr2_ = 0, ar2_ = 0;

            caculator.RRPKinematic(1, 4, 6, 5, 4, 5, 6, lenO2B * rate, ref r2_, ref vr2_, ref ar2_);

            caculator.PrintPointInfo(5);

            DrawPointChart(5);
        }

        private void Draw()
        {

            graphics.Clear(Color.White);

            DrawRod(2,3);

            DrawRod(1,4);


            //画水平杆
            var p5 = caculator.GetPointsPosition(5);

            DrawRod(new Core.Point
            {
                x = p5.x - 200,
                y = p5.y
            }, new Core.Point
            {
                x = p5.x + 300,
                y = p5.y
            });


            DrawRod(4,5);

            DrawFrame(1);

            DrawFrame(2);

            DrawPrismaticFrame(6);

            DrawPrismaticPair(3,2);

            DrawRevolutePair(3);

            DrawRevolutePair(4);

            DrawRevolutePair(5);


        }

        #endregion

    }
}
