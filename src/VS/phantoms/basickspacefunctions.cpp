#include "basickspacefunctions.h"
#include <math.h>

#include "gsl/gsl_sf_bessel.h"
#include "vsktermlist.h"


double sinc(double x) {
	double ret;
	double x2=x*3.141592654;
	if ((x2<-0.005)||(x2>0.005)) {
		ret=sin(x2)/x2;
		return ret;
	}
	else {
		ret=1-x2*x2/6+x2*x2*x2*x2/60;
	}
	return ret;
}
double bessj1sinc(double x) {
	if (x==0) return 1;
	return 2*gsl_sf_bessel_J1(x*PI)/(x*PI);
}



///////////////////////////////////////////////////////////////
//// Cylinder
///////////////////////////////////////////////////////////////

KspaceCylinder::KspaceCylinder() {
	m_diameter_x=50;
	m_diameter_y=50;
	m_length=50;
}
KspaceCylinder::~KspaceCylinder() {
}

void KspaceCylinder::evaluate(double &re, double &im, double kx,double ky,double kz, double tau) {
	Q_UNUSED(tau);
	double kx2=kx*m_diameter_x;
	double ky2=ky*m_diameter_y;
	double volume=PI*(m_diameter_x*m_diameter_y)/4*m_length;
	re=volume*bessj1sinc(sqrt(kx2*kx2+ky2*ky2))*sinc(kz*m_length); 
	im=0;
}	

///////////////////////////////////////////////////////////////
//// Cube
///////////////////////////////////////////////////////////////

KspaceCube::KspaceCube() {
	m_length_x=50;
	m_length_y=50;
	m_length_z=50;
}
KspaceCube::~KspaceCube() {
}

void KspaceCube::evaluate(double &re, double &im, double kx,double ky,double kz, double tau) {
	Q_UNUSED(tau);
	re=m_length_x*m_length_y*m_length_z*sinc(kx*m_length_x)*sinc(ky*m_length_y)*sinc(kz*m_length_z); 
	im=0;
}	

///////////////////////////////////////////////////////////////
//// Sphere
///////////////////////////////////////////////////////////////


KspaceSphere::KspaceSphere() {
	m_diameter_x=50;
	m_diameter_y=50;
	m_diameter_z=50;
}
KspaceSphere::~KspaceSphere() {
}
void KspaceSphere::evaluate(double &re, double &im, double kx,double ky,double kz, double tau) {
	Q_UNUSED(tau);
	kx*=m_diameter_x*PI;
	ky*=m_diameter_y*PI;
	kz*=m_diameter_z*PI;
	double volume=1.0/6*PI*m_diameter_x*m_diameter_y*m_diameter_z;
	double kr=sqrt(kx*kx+ky*ky+kz*kz);
	if (fabs(kr)<0.001) {	
		re=volume*(1-kr*kr/10+kr*kr*kr*kr/280);//check this
	}	
	else {
		re=volume*3*(sin(kr)/kr-cos(kr))/kr/kr;
	}
	im=0;	
}	

