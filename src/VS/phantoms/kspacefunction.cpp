#include "kspacefunction.h"
#include <math.h>

KspaceFunction::KspaceFunction() 
{
	m_max_kx=m_max_ky=m_max_kz=0;
	m_apodize_kx=m_apodize_ky=m_apodize_kz=0;
}

KspaceFunction::~KspaceFunction()
{
}

void KspaceFunction::evaluate(double &re, double &im, double kx, double ky, double kz, double tau) {
	Q_UNUSED(kx) Q_UNUSED(ky) Q_UNUSED(kz) Q_UNUSED(tau)
	re=im=0;
}

Complex KspaceFunction::eval(double kx,double ky,double kz,double tau) {
	if ((m_max_kx>0)&&(kx>m_max_kx)) return Complex(0,0);
	if ((m_max_ky>0)&&(ky>m_max_ky)) return Complex(0,0);
	if ((m_max_kz>0)&&(kz>m_max_kz)) return Complex(0,0);
	double re,im;
	evaluate(re,im,kx,ky,kz,tau);
	if (m_apodize_kx>0) {
		double val=kx/m_apodize_kx;
		double factor=exp(-val*val);
		if (re!=0) re*=factor;
		if (im!=0) im*=factor;
	}
	if (m_apodize_ky>0) {
		double val=ky/m_apodize_ky;
		double factor=exp(-val*val);
		if (re!=0) re*=factor;
		if (im!=0) im*=factor;
	}
	if (m_apodize_kz>0) {
		double val=kz/m_apodize_kz;
		double factor=exp(-val*val);
		if (re!=0) re*=factor;
		if (im!=0) im*=factor;
	}
	return Complex(re,im);
}

void KspaceFunction::setApodization(double Akx, double Aky, double Akz) {
	m_apodize_kx=1.0/Akx;
	m_apodize_ky=1.0/Aky;
	m_apodize_kz=1.0/Akz;
	m_max_kx=2.0/Akx;
	m_max_ky=2.0/Aky;
	m_max_kz=3.0/Akz;
}

