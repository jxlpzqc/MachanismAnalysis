#ifndef CORE_H
#define CORE_H


void bark(int n1, int n2, int k, double r1, double gam, double t[], double w[],
	double e[], double p[][3], double vp[][3], double ap[][3]);


void rrrk(int m, int n1, int n2, int n3, int k1, int k2, double r1, double r2, 
	double t[], double w[], double e[], 
	double p[][3], double vp[][3], double ap[][3]);

void rrpk(int m, int  n1, int  n2, int n3, int  k1, int  k2, int  k3, double  r1,
	double* r2, double* vr2, double* ar2,
	double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3]);

void rprk(int m, int  n1, int n2, int  k1, int  k2, double  r1, double* r2,
	double* vr2, double* ar2,
	double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3]);


void prpk(int n1, int n2, int  n3, int  k1, int  k2, int  k3, int k4, double* r1, double* vr1,
	double* ar1, double* r2, double* vr2,
	double* ar2, double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3]);


void rppk(int n1, int n2, int n3, int  n4, int  k1, int  k2, int k3, double  r1, double gam1, double gam2,
	double* r2, double* vr2, double* ar2, double* r3, double* vr3, double* ar3,
	double* t, double* w, double* e,
	double p[][3], double vp[][3], double ap[][3]);



void barf(int n1, int ns1, int nn1, int k1, double p[][3], double ap[][3], double* e, double fr[][3], double* tb);

void rrrf(int n1, int  n2, int  n3, int  ns1, int  ns2, int nn1, int nn2, int  nexf, int  k1, int  k2,
	double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], double fr[][3],
	void (*extf) (double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], int nexf, double fe[][3])
);

void rrpf(int n1, int n2, int n3, int  ns1, int ns2, int  nn1, int nn2, int  nexf, int  k1, int k2,
	double p[20][3], double vp[20][3], double ap[20][3],
	double* t, double* w, double* e, double fr[][3],
	void (*extf) (double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], int nexf, double fe[][3])

);

void rprf(int n1, int n2, int ns1, int ns2, int nn1, int nn2, int nexf, int k1, int k2,
	double p[][3], double vp[][3], double ap[][3], double* t, double* w, double* e,
	double fr[][3], double fk[][3], double pk[][3],
	void (*extf) (double p[][3], double vp[][3], double ap[][3], double t[], double w[], double e[], int nexf, double fe[][3])
);




#endif // !CORE_H


