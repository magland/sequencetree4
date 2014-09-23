#ifndef kspacefunction_H
#define kspacefunction_H

#include <QObject>
#include "complex.h"

class KspaceFunction : public QObject {
public:
	KspaceFunction();
	virtual ~KspaceFunction();
	virtual QString name() {return "Zero";}
	Complex eval(double kx,double ky,double kz,double tau);
	
	void setApodization(double Akx, double Aky, double Akz);	
protected:
	virtual void evaluate(double &re, double &im, double kx, double ky, double kz, double tau);	
	
	double m_max_kx;
	double m_max_ky;
	double m_max_kz;
	double m_apodize_kx;
	double m_apodize_ky;
	double m_apodize_kz;
};

#endif
