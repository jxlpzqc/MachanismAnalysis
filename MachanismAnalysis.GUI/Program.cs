using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MachanismAnalysis.GUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {

            AllocConsole();
            SetConsoleTitle("机械原理课程设计 - 程子丘 - 控制台输出");

            double speed = 48;
            bool error = false;

            //while (true)
            //{
            //    if (error) Console.Write("[输入错误] 请重新输入转速，单位 r/min [默认值 10，回车键确认]");
            //    else Console.Write("请输入转速，单位 r/min，[默认值 10，回车键确认]");

            //    var r = Console.ReadLine();
            //    if (string.IsNullOrEmpty(r))
            //    {
            //        speed = 10;
            //        break;
            //    }

            //    if (double.TryParse(r, out speed))
            //    {
            //        break;       
            //    }
            //    error = true;
            //}



            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm(speed));

            FreeConsole();
        }


        #region 本地Win32代码导入 控制台相关

        [DllImport("kernel32.dll")]
        public static extern bool AllocConsole();
        [DllImport("kernel32.dll")]
        public static extern bool FreeConsole();
        [DllImport("kernel32.dll")]
        public static extern bool SetConsoleTitle(string title);

        #endregion

    }
}
