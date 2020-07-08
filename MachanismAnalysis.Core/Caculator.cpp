#include "pch.h"
#include "Caculator.h"
#include "core.h"
#include <cstdio>
#include <cmath>

using namespace MachanismAnalysis::Core;

void MachanismAnalysis::Core::Caculator::BarKinematic(int n1, int n2, int k, double r1, double gam)
{
	bark(n1, n2, k, r1, gam, rodsAngularDisplacement,
		rodsAngularVelocity, rodsAngularAcceleration,
		pointsPosition, pointsVelocity, pointsAcceleration);
}

void MachanismAnalysis::Core::Caculator::RRRKinematic(int m, int n1, int n2, int n3, int k1, int k2, double r1, double r2)
{
	rrrk(m, n1, n2, n3, k1, k2, r1, r2, rodsAngularDisplacement,
		rodsAngularVelocity, rodsAngularAcceleration,
		pointsPosition, pointsVelocity, pointsAcceleration);
}

void MachanismAnalysis::Core::Caculator::RPRKinematic(int m, int n1, int n2, int k1, int k2, double r1, double% r2, double% vr2, double% ar2)
{
	double r2l, vr2l, ar2l;
	rprk(m, n1, n2, k1, k2, r1, &r2l, &vr2l, &ar2l, rodsAngularDisplacement,
		rodsAngularVelocity, rodsAngularAcceleration,
		pointsPosition, pointsVelocity, pointsAcceleration);
	r2 = r2l;
	vr2 = vr2l;
	ar2 = ar2l;
}

void MachanismAnalysis::Core::Caculator::RRPKinematic(int m, int n1, int n2, int n3, int k1, int k2, int k3, double r1, double% r2, double% vr2, double% ar2)
{
	double r2l, vr2l, ar2l;
	rrpk(m, n1, n2, n3, k1, k2, k3, r1, &r2l, &vr2l, &ar2l, rodsAngularDisplacement,
		rodsAngularVelocity, rodsAngularAcceleration,
		pointsPosition, pointsVelocity, pointsAcceleration);
	r2 = r2l;
	vr2 = vr2l;
	ar2 = ar2l;
}

void MachanismAnalysis::Core::Caculator::BasicPointKinematic(int nBasic, int nToCompute, int p, double len)
{
	double c = cos(rodsAngularDisplacement[p]);
	double s = sin(rodsAngularDisplacement[p]);

	// cos(a + 90) -> -sin a 
	double cv = -s;
	// sin(a + 90) -> cos a
	double sv = c;

	//求位置 
	pointsPosition[nToCompute][1] = pointsPosition[nBasic][1] + c * len;
	pointsPosition[nToCompute][2] = pointsPosition[nBasic][2] + s * len;

	//求速度 v = v_0 + w_t
	pointsVelocity[nToCompute][1] = pointsVelocity[nBasic][1] + cv * (rodsAngularVelocity[p]) * len;
	pointsVelocity[nToCompute][2] = pointsVelocity[nBasic][2] + sv * (rodsAngularVelocity[p]) * len;

	//求加速度 a = a_0 + a_t + a_n
	pointsAcceleration[nToCompute][1] = pointsAcceleration[nBasic][1] + cv * (rodsAngularAcceleration[p]) * len - c * pow(rodsAngularVelocity[p], 2) * len;
	pointsAcceleration[nToCompute][2] = pointsAcceleration[nBasic][2] + sv * (rodsAngularAcceleration[p]) * len - s * pow(rodsAngularVelocity[p], 2) * len;
	
	// TODO : Use it if a bug appears.
	// !!! Warning: Reinvent the wheel !!!
	// Just as bark, I have written so many codes in vain. 
	// Migrate to core C library like this.
	//this->BarKinematic(nBasic, nTocompute, p, len, 0);

}

void MachanismAnalysis::Core::Caculator::SetPointsPosition(int n, double x, double y)
{
	pointsPosition[n][1] = x;
	pointsPosition[n][2] = y;
}

void MachanismAnalysis::Core::Caculator::SetPointsVelocity(int n, double x, double y)
{
	pointsVelocity[n][1] = x;
	pointsVelocity[n][2] = y;
}

void MachanismAnalysis::Core::Caculator::SetPointsAcceleration(int n, double x, double y)
{
	pointsAcceleration[n][1] = x;
	pointsAcceleration[n][2] = y;
}


Point^ MachanismAnalysis::Core::Caculator::GetPointsPosition(int n)
{
	auto p = gcnew Point();
	p->x = pointsPosition[n][1];
	p->y = pointsPosition[n][2];
	return p;
}

Point^ MachanismAnalysis::Core::Caculator::GetPointsVelocity(int n)
{
	auto p = gcnew Point();
	p->x = pointsVelocity[n][1];
	p->y = pointsVelocity[n][2];
	return p;
}

Point^ MachanismAnalysis::Core::Caculator::GetPointsAcceleration(int n)
{
	auto p = gcnew Point();
	p->x = pointsAcceleration[n][1];
	p->y = pointsAcceleration[n][2];
	return p;
}

void MachanismAnalysis::Core::Caculator::SetRodsAngularDisplacement(int k, double x)
{
	rodsAngularDisplacement[k] = x;
}

void MachanismAnalysis::Core::Caculator::SetRodsAngularVelocity(int k, double x)
{
	rodsAngularVelocity[k] = x;
}

void MachanismAnalysis::Core::Caculator::SetRodsAngularAcceleration(int k, double x)
{
	rodsAngularAcceleration[k] = x;
}

double MachanismAnalysis::Core::Caculator::GetRodsAngularDisplacement(int k)
{
	return rodsAngularDisplacement[k];
}

double MachanismAnalysis::Core::Caculator::GetRodsAngularVelocity(int k)
{
	return rodsAngularVelocity[k];
}

double MachanismAnalysis::Core::Caculator::GetRodsAngularAcceleration(int k)
{
	return rodsAngularAcceleration[k];
}

void MachanismAnalysis::Core::Caculator::ConfigurePoint(int n, Point^ pos, Point^ vel, Point^ acc)
{
	SetPointsPosition(n, pos->x, pos->y);
	SetPointsVelocity(n, vel->x, vel->y);
	SetPointsAcceleration(n, acc->x, acc->y);

}

void MachanismAnalysis::Core::Caculator::ConfigurePoint(int n,
	double posX, double posY, double velX, double velY, double accX, double accY)
{
	SetPointsPosition(n, posX, posY);
	SetPointsVelocity(n, velX, velY);
	SetPointsAcceleration(n, accX, accY);
}

void MachanismAnalysis::Core::Caculator::ConfigureRod(int k, double pos, double vel, double acc)
{
	SetRodsAngularDisplacement(k, pos);
	SetRodsAngularVelocity(k, vel);
	SetRodsAngularAcceleration(k, acc);
}

void MachanismAnalysis::Core::Caculator::PrintPointInfo(int n)
{
	char buff[1024];
	sprintf_s<1024>(buff, "点%d\t位置 (%8.2lf,%8.2f)\t速度 (%8.2lf,%8.2lf)\t加速度 (%8.2lf,%8.2lf)\n", n,
		pointsPosition[n][1], pointsPosition[n][2],
		pointsVelocity[n][1], pointsVelocity[n][2],
		pointsAcceleration[n][1], pointsAcceleration[n][2]);

	System::Console::Write(gcnew System::String(buff));

}

void MachanismAnalysis::Core::Caculator::PrintRodInfo(int k)
{

	char buff[1024];
	sprintf_s<1024>(buff, "杆%d\t位置 %.4lf(rad)\t速度 %.4lf(rad/s)\t加速度 %.4lf(rad/s^2)\n", k,
		rodsAngularDisplacement[k], rodsAngularVelocity[k], rodsAngularAcceleration[k]);

	System::Console::Write(gcnew System::String(buff));
}

MachanismAnalysis::Core::Caculator::Caculator()
{
	Caculator(10, 20);
}

MachanismAnalysis::Core::Caculator::Caculator(int pNum, int rNum)
{
	pointsNum = pNum;
	rodsNum = rNum;
	pNum++;
	rNum++;
	pointsPosition = new double[pNum][3];
	pointsVelocity = new double[pNum][3];
	pointsAcceleration = new double[pNum][3];
	rodsAngularAcceleration = new double[rNum];
	rodsAngularDisplacement = new double[rNum];
	rodsAngularVelocity = new double[rNum];
}

MachanismAnalysis::Core::Caculator::~Caculator()
{
	delete[] pointsPosition;
	delete[] pointsVelocity;
	delete[] pointsAcceleration;
	delete[] rodsAngularAcceleration;
	delete[] rodsAngularDisplacement;
	delete[] rodsAngularVelocity;
}
