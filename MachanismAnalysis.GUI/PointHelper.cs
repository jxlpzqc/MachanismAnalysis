using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MachanismAnalysis.GUI
{
    class PointHelper
    {
        public static System.Drawing.PointF ToDrawingPointF(Core.Point p)
        {
            return new System.Drawing.PointF((float)p.x, (float)p.y);

        }


        public static System.Drawing.Point ToDrawingPoint(Core.Point p)
        {
            return new System.Drawing.Point((int)p.x, (int)p.y);

        }
    }
}
