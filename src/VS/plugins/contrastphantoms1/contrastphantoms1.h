#ifndef contrastphantoms1_H
#define contrastphantoms1_H

#include "vsphantomplugin.h"
#include "vsbasicphantoms.h"
#include <math.h>

class ContrastPhantoms1 : public QObject, public VSPhantomPlugin {
	Q_OBJECT
	Q_INTERFACES(VSPhantomPlugin)
public:
	ContrastPhantoms1();
	virtual ~ContrastPhantoms1();
	virtual QList<VSPhantom *> phantomList();
private:
	QList<VSPhantom *> m_phantom_list;
};

class ContrastPhantom1 : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(double period READ period WRITE setPeriod)
	Q_PROPERTY(double startTime READ startTime WRITE setStartTime)
	Q_PROPERTY(QString length READ length WRITE setLength)
	Q_PROPERTY(QString innerDiameter READ innerDiameter WRITE setInnerDiameter)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	ContrastPhantom1() {
		m_elapsed_time=0;
		setApodization(5);
		setLength("(80,80,50)");
		m_cylinder_value=1;
		m_period=m_start_time=0;
	}
	virtual ~ContrastPhantom1() {}	
	virtual QString typeName() {return "ContrastPhantom1";}
	virtual void reset() {m_elapsed_time=0; addElapsedTime(0);}
	QString length() {
		return vector3_to_string(m_cube.lengthX(),m_cube.lengthY(),m_cube.lengthZ());
	}
	void setLength(QString val) {
		float dx,dy,dz;
		string_to_vector3(val,dx,dy,dz);
		m_cube.setLengthX(dx);
		m_cube.setLengthY(dy);
		m_cube.setLengthZ(dz);		
		m_cylinder.setLength(dz);
	}
	QString innerDiameter() {
		return vector2_to_string(m_cylinder.diameterX(),m_cylinder.diameterY());
	}
	void setInnerDiameter(QString val) {
		float dx,dy;
		string_to_vector2(val,dx,dy);
		m_cylinder.setDiameterX(dx);
		m_cylinder.setDiameterY(dy);
	}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cube.setApodization(val,val,val);}
	double period() {return m_period;}
	void setPeriod(double val) {m_period=val;}
	double startTime() {return m_start_time;}
	void setStartTime(double val) {m_start_time=val;}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		return 
			m_cube.eval(kx,ky,kz,tau)
			-m_cylinder.eval(kx,ky,kz,tau)*(1-m_cylinder_value);
	}	
	void addElapsedTime(double t) {
		m_elapsed_time+=t;
		if (m_elapsed_time<m_start_time*1000) {
			m_cylinder_value=0;
			return;
		}
		double holdt=m_elapsed_time/1000-m_start_time;
		if (m_period>0) {
			while (holdt>m_period) holdt-=m_period;
			while (holdt<0) holdt+=m_period;
		}
		double tfrac=0;
		if (m_period>0) {
			tfrac=holdt/m_period;
		}
		m_cylinder_value=1-tfrac;
	}
private:
	double m_elapsed_time;
	double m_start_time;
	double m_period;
	KspaceCube m_cube;
	KspaceCylinder m_cylinder;
	double m_cylinder_value;
	float m_apodization;
};


#endif
