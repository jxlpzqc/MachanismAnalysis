#include "pch.h"
#include "Caculator.h"
#include "core.h"
#include <cstdio>
#include <cmath>

using namespace MachanismAnalysis::Core;


void extf(double p[][3], double vp[][3], double ap[][3], double* t, double* w, double* e, int nexf, double fe[][3])
{
	if (vp[nexf][1] > 0 && p[nexf][1] > 0.0625 && p[nexf][1] < 0.2875)
		fe[nexf][1] = -5200;
	else
		fe[nexf][1] = 0;
	fe[nexf][2] = 0.0;
}



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

void MachanismAnalysis::Core::Caculator::BarForce(int n1, int ns1, int nn1, int k1)
{
	double _tb;
	barf(n1, ns1, nn1, k1, pointsPosition, pointsAcceleration, rodsAngularAcceleration, pointsForce, &_tb);
	framesMoment[n1] = _tb;
}

void MachanismAnalysis::Core::Caculator::RPRForce(int n1, int n2, int ns1, int ns2, int nn1, int nn2, int nexf, int k1, int k2)
{
	//TODO: return prismatic position and force
				
	//an unused var
	double fk[20][3];
	double pk[20][3];
	rprf(n1, n2, ns1, ns2, nn1, nn2, nexf, k1, k2, pointsPosition, pointsVelocity, pointsAcceleration
		, rodsAngularDisplacement, rodsAngularVelocity, rodsAngularAcceleration,
		pointsForce, fk, pk, extf);

}

void MachanismAnalysis::Core::Caculator::RRPForce(int n1, int n2, int n3, int ns1, int ns2, int nn1, int nn2, int nexf, int k1, int k2)
{
	rrpf(n1, n2, n3, ns1, ns2, nn1, nn2, nexf, k1, k2,
		pointsPosition, pointsVelocity, pointsAcceleration
		, rodsAngularDisplacement, rodsAngularVelocity, rodsAngularAcceleration,
		pointsForce, extf);
}

void MachanismAnalysis::Core::Caculator::SetPointMass(int n, double mass)
{
	sm[n] = mass;
}

void MachanismAnalysis::Core::Caculator::SetRodJ(int k, double j)
{
	sj[k] = j;
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

void MachanismAnalysis::Core::Caculator::SetPointForce(int n, double x, double y)
{
	pointsForce[n][1] = x;
	pointsForce[n][2] = y;
}

Point^ MachanismAnalysis::Core::Caculator::GetPointForce(int n)
{
	return gcnew Point(pointsForce[n][1], pointsForce[n][2]);
}

double MachanismAnalysis::Core::Caculator::GetFrameMoment(int n)
{
	return framesMoment[n];
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
	pointsForce = new double[pNum][3];
	framesMoment = new double[pNum];

	memset(pointsForce, 0, sizeof(*pointsForce) * pNum);


	rodsAngularAcceleration = new double[rNum];
	rodsAngularDisplacement = new double[rNum];
	rodsAngularVelocity = new double[rNum];
}

MachanismAnalysis::Core::Caculator::~Caculator()
{
	delete[] pointsPosition;
	delete[] pointsVelocity;
	delete[] pointsAcceleration;
	delete[] pointsForce;
	delete[] framesMoment;

	delete[] rodsAngularAcceleration;
	delete[] rodsAngularDisplacement;
	delete[] rodsAngularVelocity;
}
