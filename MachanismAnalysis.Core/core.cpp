#include "pch.h"
#include "math.h"
#include "stdio.h"

/******上古程序代码核心，外部用wrapper包装到.net托管代码******/

#pragma region SUB_K Part


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


#pragma endregion


#pragma region SUB_F Part

double sm[10], sj[10];
/*************FITI*********************************************/
void fiti(int ns1, int ns2, int k1, int k2, double ap[][3], double  e[], double  fi[][3], double ti[])
{
	if (!((ns1 == 0) || (k1 == 0)))
	{
		fi[ns1][1] = -sm[k1] * ap[ns1][1];
		fi[ns1][2] = -sm[k1] * (ap[ns1][2] + 9.81);
		ti[k1] = -sj[k1] * e[k1];
	}
	if (!((ns2 == 0) || (k2 == 0)))
	{
		fi[ns2][1] = -sm[k2] * ap[ns2][1];
		fi[ns2][2] = -sm[k2] * (ap[ns2][2] + 9.81);
		ti[k2] = -sj[k2] * e[k2];
	}
}
/*****************BARF***********************************************/
void barf(int n1, int ns1, int nn1, int k1,double p[][3],double ap[][3],double *e,double fr[][3],double *tb)
{
	static double fi[20][3], ti[10], f[20][3];
	double ps11x, ps11y, pn11x, pn11y, fim1, fnm1;
	fiti(ns1, 0, k1, 0, ap, e, fi, ti);
	f[nn1][1] = -fr[nn1][1];
	f[nn1][2] = -fr[nn1][2];
	ps11x = p[ns1][1] - p[n1][1];
	ps11y = p[ns1][2] - p[n1][2];
	pn11x = p[nn1][1] - p[n1][1];
	pn11y = p[nn1][2] - p[n1][2];
	fim1 = ps11x * fi[ns1][2] - ps11y * fi[ns1][1];
	fnm1 = pn11x * f[nn1][2] - pn11y * f[nn1][1];
	*tb = -(fim1 + fnm1 + ti[k1]);
	fr[n1][1] = -(fi[ns1][1] + f[nn1][1]);
	fr[n1][2] = -(fi[ns1][2] + f[nn1][2]);
}
/********************RRRF*****************************************/
void rrrf(int n1, int  n2, int  n3, int  ns1, int  ns2, int nn1, int nn2, int  nexf, int  k1, int  k2,
	double p[][3], double vp[][3],double ap[][3],double t[],double w[],double e[],double fr[][3],
	void (*extf) (double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], int nexf, double fe[][3])
)
{
	static double fi[20][3], ti[10], fe[20][3], f[20][3];
	double p23x, p23y, ps23x, ps23y, pn23x, pn23y, p21x, p21y;
	double ps21x, ps21y, pn21x, pn21y, ps11x, ps11y, pn11x, pn11y;
	double si1, si2, si31, si32, si41, si42, sgm1, sgm2, cc, dd, ff;
	fiti(ns1, ns2, k1, k2, ap, e, fi, ti);
	if (nexf != 0)
	{
		extf(p, vp, ap, t, w, e, nexf, fe);
	}
	f[nn1][1] = -fr[nn1][1];
	f[nn1][2] = -fr[nn1][2];
	f[nn2][1] = -fr[nn2][1];
	f[nn2][2] = -fr[nn2][2];
	if (nexf == nn1)
	{
		f[nn1][1] = fe[nexf][1];
		f[nn1][2] = fe[nexf][2];
	}
	if (nexf == nn2)
	{
		f[nn2][1] = fe[nexf][1];
		f[nn2][2] = fe[nexf][2];
	}
	p23x = p[n2][1] - p[n3][1];
	p23y = p[n2][2] - p[n3][2];
	ps23x = p[ns2][1] - p[n3][1];
	ps23y = p[ns2][2] - p[n3][2];
	pn23x = p[nn2][1] - p[n3][1];
	pn23y = p[nn2][2] - p[n3][2];
	p21x = p[n2][1] - p[n1][1];
	p21y = p[n2][2] - p[n1][2];
	ps21x = p[ns2][1] - p[n1][1];
	ps21y = p[ns2][2] - p[n1][2];
	pn21x = p[nn2][1] - p[n1][1];
	pn21y = p[nn2][2] - p[n1][2];
	ps11x = p[ns1][1] - p[n1][1];
	ps11y = p[ns1][2] - p[n1][2];
	pn11x = p[nn1][1] - p[n1][1];
	pn11y = p[nn1][2] - p[n1][2];
	si1 = ps23x * fi[ns2][2] - ps23y * fi[ns2][1];
	si2 = pn23x * f[nn2][2] - pn23y * f[nn2][1] + ti[k2];
	si31 = ps21x * fi[ns2][2] - ps21y * fi[ns2][1];
	si32 = pn21x * f[nn2][2] - pn21y * f[nn2][1] + ti[k1];
	si41 = ps11x * fi[ns1][2] - ps11y * fi[ns1][1];
	si42 = pn11x * f[nn1][2] - pn11y * f[nn1][1] + ti[k2];
	sgm1 = -(si1 + si2);
	sgm2 = -(si31 + si32 + si41 + si42);
	cc = sgm2 * p23y - sgm1 * p21y;
	dd = sgm2 * p23x - sgm1 * p21x;
	ff = p21x * p23y - p23x * p21y;
	fr[n2][1] = dd / ff;
	fr[n2][2] = cc / ff;
	fr[n1][1] = -(fi[ns1][1] + f[nn1][1] + fi[ns2][1] + f[nn2][1] + fr[n2][1]);
	fr[n1][2] = -(fi[ns1][2] + f[nn1][2] + fi[ns2][2] + f[nn2][2] + fr[n2][2]);
	fr[n3][1] = -(fr[n2][1] + fi[ns2][1] + f[nn2][1]);
	fr[n3][2] = -(fr[n2][2] + fi[ns2][2] + f[nn2][2]);
}
/*********************RRPF***************************************/
void rrpf(int n1, int n2, int n3, int  ns1, int ns2, int  nn1, int nn2, int  nexf, int  k1, int k2,
	double p[20][3], double vp[20][3], double ap[20][3],
	double* t, double* w, double* e, double fr[][3],
	void (*extf) (double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], int nexf, double fe[][3]))
{
	static double fi[20][3], ti[10], fe[20][3], f[20][3], fk[20][3],
		pk[10][2];
	double p13x, p13y, ps13x, ps13y, pn13x, pn13y, si1, si2, sgm1, sgm2, fim13;
	double fnm13, ff, cc, dd, ps23x, ps23y, pn23x, pn23y, fim23, fnm23, aa, pk3x,
		pk3y;
	fiti(ns1, ns2, k1, k2, ap, e, fi, ti);
	if (nexf != 0)
	{
		extf(p, vp, ap, t, w, e, nexf, fe);
	}
	f[nn1][1] = -fr[nn1][1];
	f[nn1][2] = -fr[nn1][2];
	f[nn2][1] = -fr[nn2][1];
	f[nn2][2] = -fr[nn2][2];
	if (nexf == nn1)
	{
		f[nn1][1] = fe[nexf][1];
		f[nn1][2] = fe[nexf][2];
	}
	if (nexf == nn2)
	{
		f[nn2][1] = fe[nexf][1];
		f[nn2][2] = fe[nexf][2];
	}
	p13x = p[n1][1] - p[n3][1];
	p13y = p[n1][2] - p[n3][2];
	ps13x = p[ns1][1] - p[n3][1];
	ps13y = p[ns1][2] - p[n3][2];
	pn13x = p[nn1][1] - p[n3][1];
	pn13x = p[nn1][1] - p[n3][1];
	pn13y = p[nn1][2] - p[n3][2];
	si1 = (fi[ns1][1] + f[nn1][1] + fi[ns2][1] + f[nn2][1]) * cos(t[k2]);
	si2 = (fi[ns1][2] + f[nn1][2] + fi[ns2][2] + f[nn2][2]) * sin(t[k2]);
	sgm1 = -(si1 + si2);
	fim13 = ps13x * fi[ns1][2] - ps13y * fi[ns1][1];
	fnm13 = pn13x * f[nn1][2] - pn13y * f[nn1][1];
	sgm2 = -(fim13 + fnm13 + ti[k1]);
	ff = p13x * cos(t[k2]) + p13y * sin(t[k2]);
	cc = sgm1 * p13x - sgm2 * sin(t[k2]);
	dd = sgm1 * p13y + sgm2 * cos(t[k2]);
	fr[n1][1] = cc / ff;
	fr[n1][2] = dd / ff;
	fr[n3][1] = fi[ns1][1] + fr[n1][1] + f[nn1][1];
	fr[n3][2] = fi[ns1][2] + fr[n1][2] + f[nn1][2];
	fk[k2][1] = -(fi[ns2][1] + fr[n3][1] + f[nn2][1]);
	fk[k2][2] = -(fi[ns2][2] + fr[n3][2] + f[nn2][2]);
	ps23x = p[ns2][1] - p[n3][1];
	ps23y = p[ns2][2] - p[n3][2];
	pn23x = p[nn2][1] - p[n3][1];
	pn23y = p[nn2][2] - p[n3][2];
	fim23 = ps23x * fi[ns2][2] - ps23y * fi[ns2][1];
	fnm23 = pn23x * f[nn2][2] - pn23y * f[nn2][1];
	aa = -(fim23 + fnm23 + ti[k2]);
	if (t[k2] == (2.0 * atan(1.0)))
	{
		pk[k2][1] = p[n3][1];
		pk[k2][2] = -aa / fk[k2][1];
	}
	else
	{
		pk3x = aa / (fk[k2][2] - fk[k2][1] * sin(t[k2]) / cos(t[k2]));
		pk3y = pk3x * sin(t[k2]) / cos(t[k2]);
		pk[k2][1] = pk3x + p[n3][1];
		pk[k2][2] = pk3y + p[n3][2];
	}
	fr[n2][1] = fk[k2][1];
	fr[n2][2] = fk[k2][2];
	p[n2][1] = pk[k2][1];
	p[n2][2] = pk[k2][2];
}
/*************************RPRF**************************************/
void rprf(int n1, int n2, int ns1, int ns2, int nn1, int nn2, int nexf, int k1, int k2,
	double p[][3],double vp[][3],double ap[][3],double* t,double* w,double* e,
	double fr[][3],double fk[][3],double pk[][3],
	void (*extf) (double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], int nexf, double fe[][3]))

{
	static double fi[20][3], ti[10], fe[20][3], f[20][3];
	double p21x, p21y, ps21x, ps21y, pn21x, pn21y;
	double ps11x, ps11y, pn11x, pn11y, si21, si22;
	double si11, si12, si1, si2, aa, bb, sgm1, sgm2;
	double ff, cc, dd, ps22x, ps22y, pn22x, pn22y;
	double fsm22, fnm22, fm2, pk2x, pk2y;
	fiti(ns1, ns2, k1, k2, ap, e, fi, ti);
	if (nexf != 0)
	{
		extf(p, vp, ap, t, w, e, nexf, fe);
	}
	f[nn1][1] = -fr[nn1][1];
	f[nn1][2] = -fr[nn1][2];
	f[nn2][1] = -fr[nn2][1];
	f[nn2][2] = -fr[nn2][2];
	if (nexf == nn1)
	{
		f[nn1][1] = fe[nexf][1];
		f[nn1][2] = fe[nexf][2];
	}
	if (nexf == nn2)
	{
		f[nn2][1] = fe[nexf][1];
		f[nn2][2] = fe[nexf][2];
	}
	p21x = p[n2][1] - p[n1][1];
	p21y = p[n2][2] - p[n1][2];
	ps21x = p[ns2][1] - p[n1][1];
	ps21y = p[ns2][2] - p[n1][2];
	pn21x = p[nn2][1] - p[n1][1];
	pn21y = p[nn2][2] - p[n1][2];
	ps11x = p[ns1][1] - p[n1][1];
	ps11y = p[ns1][2] - p[n1][2];
	pn11x = p[nn1][1] - p[n1][1];
	pn11y = p[nn1][2] - p[n1][2];
	si21 = ps21x * fi[ns2][2] - ps21y * fi[ns2][1];
	si22 = pn21x * f[nn2][2] - pn21y * f[nn2][1] + ti[k2];
	si11 = ps11x * fi[ns1][2] - ps11y * fi[ns1][1];
	si12 = pn11x * f[nn1][2] - pn11y * f[nn1][1] + ti[k1];
	si1 = si11 + si12;
	si2 = si21 + si22;
	aa = fi[ns2][1] + f[nn2][1];
	bb = fi[ns2][2] + f[nn2][2];
	sgm1 = -(aa * cos(t[k1]) + bb * sin(t[k1]));
	sgm2 = -(si1 + si2);
	ff = p21x * cos(t[k1]) + p21y * sin(t[k1]);
	cc = p21x * sgm1 - sgm2 * sin(t[k1]);
	dd = p21y * sgm1 + sgm2 * cos(t[k1]);
	fr[n2][1] = cc / ff;
	fr[n2][2] = dd / ff;
	fr[n1][1] = -(fi[ns1][1] + f[nn1][1] + aa + fr[n2][1]);
	fr[n1][2] = -(fi[ns1][2] + f[nn1][2] + bb + fr[n2][2]);
	fk[k2][1] = -(fi[ns2][1] + f[nn2][1] + fr[n2][1]);
	fk[k2][2] = -(fi[ns2][2] + f[nn2][2] + fr[n2][2]);
	ps22x = p[ns2][1] - p[n2][1];
	ps22y = p[ns2][2] - p[n2][2];
	pn22x = p[nn2][1] - p[n2][1];
	pn22y = p[nn2][2] - p[n2][2];
	fsm22 = ps22x * fi[ns2][2] - ps22y * fi[ns2][1];
	fnm22 = pn22x * f[nn2][2] - pn22y * f[nn2][1];
	fm2 = -(fsm22 + fnm22 + ti[k2]);
	pk2x = fm2 / (fk[k2][2] - fk[k2][1] * sin(t[k1]) / cos(t[k1]));
	pk2y = pk2x * sin(t[k1]) / cos(t[k1]);
	pk[k2][1] = p[n2][1] + pk2x;
	pk[k2][2] = p[n2][2] + pk2y;
}
/************************PRPF***************************************/
//void prpf(n1, n2, n3, ns1, ns2, nn1, nn2, nexf, k1, k2, k3, k4, p, vp, ap,
//	t, w, e, fr, fk, pk)
//	int n1, n2, n3, ns1, ns2, nn1, nn2, nexf, k1, k2, k3, k4;
//double p[20][2], vp[20][2], ap[20][2];
//double t[10], w[10], e[10], fr[20][2], fk[20][2], pk[10][2];
//{
//	static double fi[20][2], ti[10], fe[20][2], f[20][2];
//	double  ps13x, ps13y, pn13x, pn13y, ps23x, ps23y, pn23x, pn23y;
//	double bb1, bb2, bb, e3, tan3, si1, si2, sgm1, pk13x, pk13y, si3, si4;
//	double sgm2, e4, tan4, pk23x, pk23y;
//	fiti(ns1, ns2, k1, k2, ap, e, fi, ti);
//	if (nexf != 0)
//	{
//		extf(p, vp, ap, t, w, e, nexf, fe);
//	}
//	f[nn1][1] = -fr[nn1][1];
//	f[nn1][2] = -fr[nn1][2];
//	f[nn2][1] = -fr[nn2][1];
//	f[nn2][2] = -fr[nn2][2];
//	if (nexf == nn1)
//	{
//		f[nn1][1] = fe[nexf][1];
//		f[nn1][2] = fe[nexf][2];
//	}
//	if (nexf == nn2)
//	{
//		f[nn2][1] = fe[nexf][1];
//		f[nn2][2] = fe[nexf][2];
//	}
//	ps13x = p[ns1][1] - p[n3][1];
//	ps13y = p[ns1][2] - p[n3][2];
//	pn13x = p[nn1][1] - p[n3][1];
//	pn13y = p[nn1][2] - p[n3][2];
//	ps23x = p[ns2][1] - p[n3][1];
//	ps23y = p[ns2][2] - p[n3][2];
//	pn23x = p[nn2][1] - p[n3][1];
//	pn23y = p[nn2][2] - p[n3][2];
//	bb1 = (fi[ns1][2] + f[nn1][2] + fi[ns2][2] + f[nn2][2]) * sin(t[k4]);
//	bb2 = (fi[ns1][2] + f[nn1][1] + fi[ns2][1] + f[nn2][1]) * cos(t[k4]);
//	bb = -(bb1 + bb2);
//	e3 = fabs(cos(t[k3]));
//	if (e3 < 0.0001)
//	{
//		fk[k1][2] = 0;
//		fk[k1][1] = bb / cos(t[k4]);
//	}
//	else
//	{
//		tan3 = sin(t[k3]) / cos(t[k3]);
//		fk[k1][2] = bb / (sin(t[k4]) - tan3 * cos(t[k4]));
//		fk[k1][1] = -fk[k1][2] * tan3;
//	}
//	fr[n3][1] = fk[k1][1] + fi[ns1][1] + f[nn1][1];
//	fr[n3][2] = fk[k1][2] + fi[ns1][2] + f[nn1][2];
//	fk[k2][1] = -(fr[n3][1] + fi[ns2][1] + f[nn2][1]);
//	fk[k2][2] = -(fr[n3][2] + fi[ns2][2] + f[nn2][2]);
//	si1 = ps13x * fi[ns1][2] - ps13y * fi[ns1][1];
//	si2 = pn13x * f[nn1][2] - pn13y * f[nn1][1];
//	sgm1 = -(si1 + si2 + ti[k1]);
//	if (e3 < 0.0001)
//	{
//		pk13x = 0;
//		pk13y = -sgm1 / fk[k1][1];
//	}
//	else
//	{
//		pk13x = sgm1 / (fk[k1][2] - fk[k1][1] * tan3);
//		pk13y = pk13x * tan3;
//	}
//	pk[k1][1] = p[n3][1] + pk13x;
//	pk[k1][2] = p[n3][2] + pk13y;
//	si3 = ps23x * fi[ns2][2] - ps23y * fi[ns2][1];
//	si4 = pn23x * f[nn2][2] - pn23y * f[nn2][1];
//	sgm2 = -(si3 + si4 + ti[k2]);
//	e4 = fabs(cos(t[k4]));
//	if (e4 < 0.0001)
//	{
//		pk23x = 0.0;
//		pk23y = -sgm2 / fk[k2][1];
//	}
//	else
//	{
//		tan4 = sin(t[k4]) / cos(t[k4]);
//		pk23x = sgm2 / (fk[k2][2] - fk[k2][1] * tan4);
//		pk23y = pk23x * tan4;
//	}
//	pk[k2][1] = p[n3][1] + pk23x;
//	pk[k2][2] = p[n3][2] + pk23y;
//	fr[n1][1] = fk[k1][1];
//	fr[n1][2] = fk[k1][2];
//	fr[n2][1] = fk[k2][1];
//	fr[n2][2] = fk[k2][2];
//	p[n1][1] = pk[k1][1];
//	p[n1][2] = pk[k1][2];
//	p[n2][1] = pk[k2][1];
//	p[n2][2] = pk[k2][2];
//}
/****************************RPPF***************************************/
//void rppf(n1, n2, n3, n4, ns1, ns2, nn1, nn2, nexf, k1, k2, k3, p, vp, ap, t, w, e,
//	fr, fk, pk)
//	int n1, n2, n3, n4, ns1, ns2, nn1, nn2, nexf, k1, k2, k3;
//double p[20][2], vp[20][2], ap[20][2];
//double t[10], w[10], e[10], fr[20][2], fk[20][2], pk[10][2];
//{
//	static double  fe[20][2], f[20][2], fi[20][2], ti[10];
//	double bb, e3, tan2, tan3, sgm1, sgm2, e2;
//	double si1, si2, si3, si4, si5, si6;
//	double p13x, p13y, ps13x, ps13y, pn13x, pn13y, pk13x, pk13y;
//	double ps24x, ps24y, pn24x, pn24y, pk14x, pk14y, pk24x, pk24y;
//	fiti(ns1, ns2, k1, k2, ap, e, fi, ti);
//	if (nexf != 0)
//	{
//		extf(p, vp, ap, t, w, e, nexf, fe);
//	}
//	f[nn1][1] = -fr[nn1][1];
//	f[nn1][2] = -fr[nn1][2];
//	f[nn2][1] = -fr[nn2][1];
//	f[nn2][2] = -fr[nn2][2];
//	if (nexf == nn1)
//	{
//		f[nn1][1] = fe[nexf][1];
//		f[nn1][2] = fe[nexf][2];
//	}
//	if (nexf == nn2)
//	{
//		f[nn2][1] = fe[nexf][1];
//		f[nn2][2] = fe[nexf][2];
//	}
//	bb = -((fi[ns2][2] + f[nn2][2]) * sin(t[k2]) + (fi[ns2][1] + f[nn2][1])
//		* cos(t[k2]));
//	e3 = fabs(cos(t[k3]));
//	if (e3 < 0.0001)
//	{
//		fk[k2][2] = 0;
//		fk[k2][1] = bb / cos(t[k2]);
//	}
//	else
//	{
//		tan3 = sin(t[k3]) / cos(t[k3]);
//		fk[k2][2] = bb / (sin(t[k2]) - tan3 * cos(t[k2]));
//		fk[k2][1] = -fk[k2][2] * tan3;
//	}
//	fk[k1][1] = -(fk[k2][1] + fi[ns2][1] + f[nn2][1]);
//	fk[k1][2] = -(fk[k2][2] + fi[ns2][2] + f[nn2][2]);
//	fr[n1][1] = fk[k1][1] - fi[ns1][1] - f[nn1][1];
//	fr[n1][2] = fk[k1][2] - fi[ns1][2] - f[nn1][2];
//	p13x = p[n1][1] - p[n3][1];
//	p13y = p[n1][2] - p[n3][2];
//	ps13x = p[ns1][1] - p[n3][1];
//	ps13y = p[ns1][2] - p[n3][2];
//	pn13x = p[nn1][1] - p[n3][1];
//	pn13y = p[nn1][2] - p[n3][2];
//	si1 = p13x * fr[n1][2] - p13y * fr[n1][1];
//	si2 = ps13x * fi[ns1][2] - ps13y * fi[ns1][1];
//	si3 = pn13x * f[nn1][2] - pn13y * f[nn1][1];
//	sgm1 = si1 + si2 + si3 + ti[k1];
//	e2 = fabs(cos(t[k2]));
//	if (e2 < 0.0001)
//	{
//		pk13x = 0;
//		pk13y = -sgm1 / fk[k1][1];
//	}
//	else
//	{
//		tan2 = sin(t[k2]) / cos(t[k2]);
//		pk13x = sgm1 / (fk[k1][2] - fk[k1][1] * tan2);
//		pk13y = pk13x * tan2;
//	}
//	pk[k1][1] = p[n3][1] + pk13x;
//	pk[k1][2] = p[n3][2] + pk13y;
//	ps24x = p[ns2][1] - p[n4][1];
//	ps24y = p[ns2][2] - p[n4][2];
//	pn24x = p[nn2][1] - p[n4][1];
//	pn24y = p[nn2][2] - p[n4][2];
//	pk14x = pk[k1][1] - p[n4][1];
//	pk14y = pk[k1][2] - p[n4][2];
//	si4 = ps24x * fi[ns2][2] - ps24y * fi[ns2][1];
//	si5 = pn24x * f[nn2][2] - pn24y * f[nn2][1];
//	si6 = pk14x * fk[k1][2] - pk14y * fk[k1][1];
//	sgm2 = -(si4 + si5 + si6 + ti[k2]);
//	if (e3 < 0.0001)
//	{
//		pk24x = 0;
//		pk24y = -sgm2 / fk[k2][1];
//	}
//	else
//	{
//		pk24x = sgm2 / (fk[k2][2] - fk[k2][1] * tan3);
//		pk24y = pk24x * tan3;
//	}
//	pk[k2][1] = p[n4][1] + pk24x;
//	pk[k2][2] = p[n4][2] + pk24y;
//	fr[n2][1] = fk[k2][1];
//	fr[n2][2] = fk[k2][2];
//	p[n2][1] = pk[k2][1];
//	p[n2][2] = pk[k2][2];
//}


#pragma endregion
