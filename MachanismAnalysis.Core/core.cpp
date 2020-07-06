#include "pch.h"
#include "math.h"
#include "stdio.h"

/******上古程序代码核心，外部用wrapper包装到.net托管代码******/


/***************bark  *******************************************/
void bark(int n1, int n2, int k, double r1, double gam, double t[], double w[], double e[], double p[][3], double vp[][3], double ap[][3])
{
	double rx2, ry2;

	rx2 = r1 * cos(t[k] + gam);
	ry2 = r1 * sin(t[k] + gam);
	p[n2][1] = p[n1][1] + rx2;
	p[n2][2] = p[n1][2] + ry2;
	vp[n2][1] = vp[n1][1] - ry2 * w[k];
	vp[n2][2] = vp[n1][2] + rx2 * w[k];
	ap[n2][1] = ap[n1][1] - ry2 * e[k] - rx2 * w[k] * w[k];
	ap[n2][2] = ap[n1][2] + rx2 * e[k] - ry2 * w[k] * w[k];
}



/************rrrk*********************************************/
void rrrk(int m, int n1, int n2, int n3, int k1, int k2, double r1, double r2, double t[], double w[], double e[], double p[][3], double vp[][3], double ap[][3])
{
	double delx, dely, phi, ssq, s, test, csn, alpha, tht, q, ea, eb;
	double	dx31, dy31, dx32, dy32, dvx21, dvy21;
	delx = p[n2][1] - p[n1][1];
	dely = p[n2][2] - p[n1][2];
	phi = atan2(dely, delx);
	ssq = delx * delx + dely * dely;
	s = sqrt(ssq);
	test = s - (r1 + r2);
	if (test > 0)
	{
		printf("\n RRR can't be assembled.\n");
	}
	else
	{
		test = fabs(r1 - r2) - s;
		if (test > 0)
		{
			printf("\n RRR can't be assembled.\n");
		}
		else
		{
			csn = (r1 * r1 + ssq - r2 * r2) / (2.0 * r1 * s);
			alpha = atan2(sqrt(1.0 - csn * csn), csn);
			if (m <= 0)
			{
				tht = phi - alpha;
			}
			else
			{
				tht = phi + alpha;
			}
			p[n3][1] = p[n1][1] + r1 * cos(tht);
			p[n3][2] = p[n1][2] + r1 * sin(tht);
			dx31 = p[n3][1] - p[n1][1];
			dy31 = p[n3][2] - p[n1][2];
			dx32 = p[n3][1] - p[n2][1];
			dy32 = p[n3][2] - p[n2][2];
			t[k1] = atan2(dy31, dx31);
			t[k2] = atan2(dy32, dx32);
			q = dy31 * dx32 - dy32 * dx31;
			dvx21 = vp[n2][1] - vp[n1][1];
			dvy21 = vp[n2][2] - vp[n1][2];
			w[k1] = -(dvx21 * dx32 + dvy21 * dy32) / q;
			w[k2] = -(dvy21 * dy31 + dvx21 * dx31) / q;
			vp[n3][1] = vp[n1][1] - r1 * w[k1] * sin(t[k1]);
			vp[n3][2] = vp[n1][2] + r1 * w[k1] * cos(t[k1]);
			ea = ap[n2][1] - ap[n1][1] + (vp[n3][2] - vp[n1][2]) * w[k1];
			ea = ea - (vp[n3][2] - vp[n2][2]) * w[k2];
			eb = ap[n2][2] - ap[n1][2] - (vp[n3][1] - vp[n1][1]) * w[k1];
			eb = eb + (vp[n3][1] - vp[n2][1]) * w[k2];
			e[k1] = -(ea * dx32 + eb * dy32) / q;
			e[k2] = -(ea * dx31 + eb * dy31) / q;
			ap[n3][1] = ap[n1][1] - e[k1] * dy31 - w[k1] * w[k1] * dx31;
			ap[n3][2] = ap[n1][2] + e[k1] * dx31 - w[k1] * w[k1] * dy31;
		}
	}
}
/****************rrpk********************************************/
void rrpk(int m, int  n1, int  n2, int n3, int  k1, int  k2, int  k3, double  r1, double* r2,
	double* vr2, double* ar2, double* t, double* w, double* e, double p[][3], double vp[][3], double ap[][3])
{
	double dx12, dy12, dx31, dy31, dx32, dy32;
	double ssq, phi, ep, u, fp, cb, sb, ct, st, q, ev, fv, ea, fa;
	t[k2] = t[k3];
	dx12 = p[n1][1] - p[n2][1];
	dy12 = p[n1][2] - p[n2][2];
	ssq = dx12 * dx12 + dy12 * dy12;
	phi = atan2(dy12, dx12);
	ep = sqrt(ssq) * cos(phi - t[k3]);
	u = sqrt(ssq) * sin(phi - t[k3]);
	if ((r1 - fabs(u)) < 0)
	{
		printf("\n RRP can't be assembled.\n");
	}
	else
	{
		fp = sqrt(r1 * r1 - u * u);
		if (m > 0)
		{
			*r2 = ep + fp;
		}
		else
		{
			*r2 = ep - fp;
		}
		cb = cos(t[k3]);
		sb = sin(t[k3]);
		p[n3][1] = p[n2][1] + (*r2) * cb;
		p[n3][2] = p[n2][2] + (*r2) * sb;
		dx31 = p[n3][1] - p[n1][1];
		dy31 = p[n3][2] - p[n1][2];
		dx32 = p[n3][1] - p[n2][1];
		dy32 = p[n3][2] - p[n2][2];
		t[k1] = atan2(dy31, dx31);
		ct = cos(t[k1]);
		st = sin(t[k1]);
		q = dy31 * sb + dx31 * cb;
		ev = vp[n2][1] - vp[n1][1] - (*r2) * w[k3] * sb;
		fv = vp[n2][2] - vp[n1][2] + (*r2) * w[k3] * cb;
		w[k1] = (-ev * sb + fv * cb) / q;
		*vr2 = -(ev * dx31 + fv * dy31) / q;
		vp[n3][1] = vp[n1][1] - r1 * w[k1] * st;
		vp[n3][2] = vp[n1][2] + r1 * w[k1] * ct;
		ea = ap[n2][1] - ap[n1][1] + w[k1] * w[k1] * dx31 - w[k3] * w[k3] * (*r2) * cb;
		ea = ea - 2.0 * w[k3] * (*vr2) * sb - e[k3] * dy32;
		fa = ap[n2][2] - ap[n1][2] + w[k1] * w[k1] * dy31 - w[k3] * w[k3] * (*r2) * sb;
		fa = fa + 2.0 * w[k3] * (*vr2) * cb - e[k3] * dx32;
		e[k1] = (-ea * sb + fa * cb) / q;
		*ar2 = -(ea * dx31 + fa * dy31) / q;
		ap[n3][1] = ap[n1][1] - r1 * w[k1] * w[k1] * ct - r1 * e[k1] * st;
		ap[n3][2] = ap[n1][2] - r1 * w[k1] * w[k1] * st + r1 * e[k1] * ct;
		w[k2] = w[k3];
		e[k2] = e[k3];
	}
}
/***************rprk*******************************************/
void rprk(int m, int  n1, int n2, int  k1, int  k2, double  r1, 
	double* r2,	double* vr2, double* ar2, double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3])
{
	double dx21, dy21, test, phi, alpha, ct, st, q, vx21, vy21, ea, fa;
	dx21 = p[n2][1] - p[n1][1];
	dy21 = p[n2][2] - p[n1][2];
	test = dx21 * dx21 + dy21 * dy21 - r1 * r1;
	if (test < 0)
	{
		printf("\n RPR can't be assembled.\n");
	}
	else
	{
		*r2 = sqrt(test);
		phi = atan2(dy21, dx21);
		alpha = atan(r1 / (*r2));
		if (m > 0)
		{
			t[k1] = phi + alpha;
		}
		else
		{
			t[k1] = phi - alpha;
		}
		t[k2] = t[k1];
		ct = cos(t[k1]);
		st = sin(t[k1]);
		q = dx21 * ct + dy21 * st;
		vx21 = vp[n2][1] - vp[n1][1];
		vy21 = vp[n2][2] - vp[n1][2];
		w[k1] = (vy21 * ct - vx21 * st) / q;
		w[k2] = w[k1];
		*vr2 = (vy21 * dy21 + vx21 * dx21) / q;
		ea = ap[n2][1] - ap[n1][1] + w[k1] * w[k1] * dx21 + 2.0 * w[k1] * (*vr2) * st;
		fa = ap[n2][2] - ap[n1][2] + w[k1] * w[k1] * dy21 - 2.0 * w[k1] * (*vr2) * ct;
		e[k1] = -(ea * st - fa * ct) / q;
		e[k2] = e[k1];
		*ar2 = (ea * dx21 + fa * dy21) / q;
	}
}

/*******************prpk*******************************************/
void prpk(int n1, int n2, int  n3, int  k1, int  k2, int  k3, int k4, double* r1, double* vr1,
	double* ar1, double* r2, double* vr2,
	double* ar2, double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3])
{
	double c3, s3, c4, s4, s43, p211, p212, v211, v212, ww3, ww4, a211, a212;
	c3 = cos(t[k3]);
	s3 = sin(t[k3]);
	c4 = cos(t[k4]);
	s4 = sin(t[k4]);
	t[k1] = t[k3];
	t[k2] = t[k4];
	s43 = sin(t[k4] - t[k3]);
	p211 = p[n2][1] - p[n1][1];
	p212 = p[n2][2] - p[n1][2];
	*r1 = (p211 * s4 - p212 * c4) / s43;
	*r2 = (p211 * s3 - p212 * c3) / s43;
	p[n3][1] = p[n1][1] + (*r1) * c3;
	p[n3][2] = p[n1][2] + (*r1) * s3;
	w[k1] = w[k3];
	w[k2] = w[k4];
	v211 = vp[n2][1] - vp[n1][1] - (*r2) * w[k4] * s4 + (*r1) * w[k3] * s3;
	v212 = vp[n2][2] - vp[n1][2] + (*r2) * w[k4] * c4 - (*r1) * w[k3] * c3;
	*vr1 = (v211 * s4 - v212 * c4) / s43;
	*vr2 = (v211 * s3 - v212 * c3) / s43;
	vp[n3][1] = vp[n1][1] + (*vr1) * c3 - (*r1) * w[k3] * s3;
	vp[n3][2] = vp[n1][2] + (*vr1) * s3 + (*r1) * w[k3] * c3;
	e[k1] = e[k3];
	e[k2] = e[k4];
	ww3 = w[k3] * w[k3];
	ww4 = w[k4] * w[k4];
	a211 = ap[n2][1] - ap[n1][1] - 2.0 * (*vr2) * w[k4] * s4 + 2.0 * (*vr1) * w[k3] * s3;
	a212 = ap[n2][2] - ap[n1][2] + 2.0 * (*vr2) * w[k4] * c4 - 2.0 * (*vr1) * w[k3] * c3;
	a211 = a211 - (*r2) * ww4 * c4 + (*r1) * ww3 * c3 - (*r2) * e[k4] * s4 + (*r1) * e[k3] * s3;
	a212 = a212 - (*r2) * ww4 * s4 + (*r1) * ww3 * s3 + (*r2) * e[k4] * c4 - (*r1) * e[k3] * c3;
	*ar1 = (a211 * s4 - a212 * c4) / s43;
	*ar2 = (a211 * s3 - a212 * c3) / s43;
	ap[n3][1] = ap[n1][1] + (*ar1) * c3 - 2.0 * (*vr1) * w[k3] * s3 - (*r1) * e[k3] * s3;
	ap[n3][2] = ap[n1][2] + (*ar1) * s3 + 2.0 * (*vr1) * w[k3] * c3 + (*r1) * e[k3] * c3;
}

/*********************rrpk*************************************/
void rppk(int n1, int n2, int n3, int  n4, int  k1, int  k2, int k3, double  r1, double gam1, double gam2,
	double* r2, double* vr2, double* ar2, double* r3, double* vr3, double* ar3,
	double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3])

{
	double c1, s1, c2, s2, c3, s3, p121, p122, va, vb, ww, aa, ab;
	t[k1] = t[k3] + gam2 + gam1 - 4.0 * atan(1.0);
	t[k2] = t[k3] + gam2;
	w[k1] = w[k3];
	w[k2] = w[k3];
	e[k1] = e[k3];
	e[k2] = e[k3];
	c1 = cos(t[k1]);
	s1 = sin(t[k1]);
	c2 = cos(t[k2]);
	s2 = sin(t[k2]);
	c3 = cos(t[k3]);
	s3 = sin(t[k3]);
	p121 = p[n1][1] - p[n2][1] + r1 * c1;
	p122 = p[n1][2] - p[n2][2] + r1 * s1;
	*r2 = -(p121 * s3 - p122 * c3) / sin(gam2);
	*r3 = (p121 * s2 - p122 * c2) / sin(gam2);
	p[n3][1] = p[n1][1] + r1 * c1;
	p[n3][2] = p[n1][2] + r1 * s1;
	p[n4][1] = p[n2][1] + (*r3) * c3;
	p[n4][2] = p[n2][2] + (*r3) * s3;
	va = vp[n1][1] - vp[n2][1] - r1 * w[k1] * s1;
	vb = vp[n1][2] - vp[n2][2] + r1 * w[k1] * c1;
	*vr2 = -((va + p122 * w[k3]) * s3 + (p121 * w[k3] - vb) * c3) / sin(gam2);
	*vr3 = ((va + p122 * w[k3]) * s2 + (p121 * w[k3] - vb) * c2) / sin(gam2);
	vp[n3][1] = vp[n1][1] - r1 * w[k1] * s1;
	vp[n3][2] = vp[n1][2] + r1 * w[k1] * c1;
	vp[n4][1] = vp[n2][1] + (*vr3) * c3 - (*r3) * w[k3] * s3;
	vp[n4][2] = vp[n2][2] + (*vr3) * s3 + (*r3) * w[k3] * c3;
	ww = w[k1] * w[k1];
	aa = ap[n1][1] - ap[n2][1] - r1 * e[k1] * s1 - r1 * ww * c1;
	ab = ap[n1][2] - ap[n2][2] + r1 * e[k1] * c1 - r1 * ww * s1;
	*ar2 = (aa - p121 * ww + 2.0 * vb * w[k3] + p122 * e[k3]) * s3;
	*ar2 = *ar2 + (2.0 * va * w[k3] + p121 * e[k3] - ab + p122 * ww) * c3;
	*ar2 = -*ar2 / sin(gam2);
	*ar3 = (aa - p121 * ww + 2.0 * vb * w[k3] + p122 * e[k3]) * s2;
	*ar3 = *ar3 + (2.0 * va * w[k3] + p121 * e[k3] - ab + p122 * ww) * c2;
	*ar3 = *ar3 / sin(gam2);
	ap[n3][1] = ap[n1][1] - r1 * e[k1] * s1 - r1 * ww * c1;
	ap[n3][2] = ap[n1][2] + r1 * e[k1] * c1 - r1 * ww * s1;
	ap[n4][1] = ap[n2][1] - (2.0 * (*vr3) * w[k3] + (*r3) * e[k3]) * s3 +
		(*ar3 - (*r3) * ww) * c3;
	ap[n4][2] = ap[n2][2] + (2.0 * (*vr3) * w[k3] + (*r3) * e[k3]) * c3 +
		(*ar3 - (*r3) * ww) * s3;
}