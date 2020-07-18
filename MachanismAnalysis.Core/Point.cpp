#include "pch.h"
#include "Point.h"
#include <cmath>

MachanismAnalysis::Core::Point::Point(double x, double y):x(x),y(y){ }

MachanismAnalysis::Core::Point::Point(){ }

double MachanismAnalysis::Core::Point::Size::get() {
	return sqrt(x * x + y * y);
}