#include "pch.h"
#include "Caculator.h"
#include "core.h"
#include <stdio.h>

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

MachanismAnalysis::Core::Caculator::Caculator()
{
	Caculator(10, 20);
}

MachanismAnalysis::Core::Caculator::Caculator(int pNum, int rNum)
{
	pointsPosition = new double[pNum][3];
	pointsVelocity = new double[pNum][3];
	pointsAcceleration = new double[pNum][3];
	rodsAngularAcceleration = new double[rNum];
	rodsAngularDisplacement = new double[rNum];
	rodsAngularVelocity = new double[rNum];
	pointsNum = pNum;
	rodsNum = rNum;

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
