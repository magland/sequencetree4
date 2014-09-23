#ifndef shepploganphantoms_H
#define shepploganphantoms_H

#include "vsphantomplugin.h"
#include "vsbasicphantoms.h"

class SheppLoganPhantoms : public QObject, public VSPhantomPlugin {
	Q_OBJECT
	Q_INTERFACES(VSPhantomPlugin)
public:
	SheppLoganPhantoms();
	virtual ~SheppLoganPhantoms();
	virtual QList<VSPhantom *> phantomList();
private:
	QList<VSPhantom *> m_phantom_list;
};

Complex shiftfactor(double kval,double shiftvalue);
void rotate_kxy(double &kx2,double &ky2,double kx,double ky,double angle);

class SheppLoganPhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(float contrast READ contrast WRITE setContrast)
	Q_PROPERTY(float diameterX READ diameterX WRITE setDiameterX)
	Q_PROPERTY(float diameterY READ diameterY WRITE setDiameterY)
	Q_PROPERTY(float length READ length WRITE setLength)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	SheppLoganPhantom() {
		m_length=50;
		m_diamx=40;
		m_diamy=60;
		m_apodization=1;
		m_contrast=0.01;
		need_to_update=true;
	}
	virtual ~SheppLoganPhantom() {}
	virtual QString typeName() {return "Cylinder";}
	float contrast() {return m_contrast;}
	void setContrast(float val) {m_contrast=val; need_to_update=true;}
	float diameterX() {return m_diamx;}
	void setDiameterX(float val) {m_diamx=val; need_to_update=true;}
	float diameterY() {return m_diamy;}
	void setDiameterY(float val) {m_diamy=val;  need_to_update=true;}
	float length() {return m_length;}
	void setLength(float val) {m_length=val;  need_to_update=true;}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val;  need_to_update=true;}
	Complex evaluate(double kx,double ky,double kz, double tau) {
		if (need_to_update) update_cylinders();
		need_to_update=false;
		Complex ret=Complex(0,0);
		double kx2,ky2;
		ret=ret+m_cylinders[0].eval(kx,ky,kz,tau)*(1.0);		
		ret=ret+m_cylinders[1].eval(kx,ky,kz,tau)*(-1+m_contrast*2)*shiftfactor(ky,-0.02/0.92*m_diamy/2);
		rotate_kxy(kx2,ky2,kx,ky,-18);
		ret=ret+m_cylinders[2].eval(kx2,ky2,kz,tau)*(-m_contrast*2)*shiftfactor(kx,0.22/0.69*m_diamx/2);
		rotate_kxy(kx2,ky2,kx,ky,+18);
		ret=ret+m_cylinders[3].eval(kx2,ky2,kz,tau)*(-m_contrast*2)*shiftfactor(kx,-0.22/0.69*m_diamx/2);
		ret=ret+m_cylinders[4].eval(kx,ky,kz,tau)*(m_contrast)*shiftfactor(ky,0.35/0.92*m_diamy/2);
		ret=ret+m_cylinders[5].eval(kx,ky,kz,tau)*(m_contrast)*shiftfactor(ky,0.10/0.92*m_diamy/2);
		ret=ret+m_cylinders[6].eval(kx,ky,kz,tau)*(m_contrast)*shiftfactor(ky,-0.10/0.92*m_diamy/2);
		ret=ret+m_cylinders[7].eval(kx,ky,kz,tau)*(m_contrast)*shiftfactor(kx,-0.08/0.69*m_diamx/2)*shiftfactor(ky,-0.61/0.92*m_diamy/2);
		ret=ret+m_cylinders[8].eval(kx,ky,kz,tau)*(m_contrast)*shiftfactor(ky,-0.61/0.92*m_diamy/2);
		ret=ret+m_cylinders[9].eval(kx,ky,kz,tau)*(m_contrast)*shiftfactor(kx,0.06/0.69*m_diamx/2)*shiftfactor(ky,-0.61/0.92*m_diamy/2);
		return ret;
	}	
	void update_cylinders() {
		for (int j=0; j<10; j++) {
			m_cylinders[j].setApodization(m_apodization,m_apodization,m_apodization);
			m_cylinders[j].setLength(m_length);
		}
		m_cylinders[0].setDiameterX(m_diamx*0.69/0.69);
		m_cylinders[1].setDiameterX(m_diamx*0.68/0.69);
		m_cylinders[2].setDiameterX(m_diamx*0.11/0.69);
		m_cylinders[3].setDiameterX(m_diamx*0.16/0.69);
		m_cylinders[4].setDiameterX(m_diamx*0.21/0.69);
		m_cylinders[5].setDiameterX(m_diamx*0.05/0.69);
		m_cylinders[6].setDiameterX(m_diamx*0.05/0.69);
		m_cylinders[7].setDiameterX(m_diamx*0.05/0.69);
		m_cylinders[8].setDiameterX(m_diamx*0.02/0.69);
		m_cylinders[9].setDiameterX(m_diamx*0.02/0.69);
		
		m_cylinders[0].setDiameterY(m_diamy*0.92/0.92);
		m_cylinders[1].setDiameterY(m_diamy*0.87/0.92);
		m_cylinders[2].setDiameterY(m_diamy*0.31/0.92);
		m_cylinders[3].setDiameterY(m_diamy*0.41/0.92);
		m_cylinders[4].setDiameterY(m_diamy*0.25/0.92);
		m_cylinders[5].setDiameterY(m_diamy*0.05/0.92);
		m_cylinders[6].setDiameterY(m_diamy*0.05/0.92);
		m_cylinders[7].setDiameterY(m_diamy*0.02/0.92);
		m_cylinders[8].setDiameterY(m_diamy*0.02/0.92);
		m_cylinders[9].setDiameterY(m_diamy*0.05/0.92);
	}
private:
	double m_diamx;
	double m_diamy;
	double m_length;
	double m_apodization;
	bool need_to_update;
	double m_contrast;

	KspaceCylinder m_cylinders[12];

};




#endif
