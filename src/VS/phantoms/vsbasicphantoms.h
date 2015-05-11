#ifndef vsbasicphantoms_H
#define vsbasicphantoms_H

#include "vsphantom.h"
#include "basickspacefunctions.h"
#include <QString>
#include <QDebug>

#ifndef PI
	#define PI 3.14159265358979323846
#endif

QString vector2_to_string(float x,float y);
void string_to_vector2(QString val,float &x,float &y);
QString vector3_to_string(float x,float y,float z);
void string_to_vector3(QString val,float &x,float &y,float &z);

class VSCubePhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(QString length READ length WRITE setLength)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	VSCubePhantom() {m_apodization=1;}
	virtual ~VSCubePhantom() {}	
	virtual QString typeName() {return "Cube";}
	QString length() {
		return vector3_to_string(m_cube.lengthX(),m_cube.lengthY(),m_cube.lengthZ());
	}
	void setLength(QString val) {
		float dx,dy,dz;
		string_to_vector3(val,dx,dy,dz);
		m_cube.setLengthX(dx);
		m_cube.setLengthY(dy);
		m_cube.setLengthZ(dz);
	}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cube.setApodization(val,val,val);}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		return m_cube.eval(kx,ky,kz,tau);
	}	
private:
	KspaceCube m_cube;
	float m_apodization;
};

class VSCylinderPhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(float diameterX READ diameterX WRITE setDiameterX)
	Q_PROPERTY(float diameterY READ diameterY WRITE setDiameterY)
	Q_PROPERTY(float length READ length WRITE setLength)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	VSCylinderPhantom() {m_apodization=1;}
	virtual ~VSCylinderPhantom() {}	
	virtual QString typeName() {return "Cylinder";}
	float diameterX() {return m_cylinder.diameterX();}
	void setDiameterX(float val) {m_cylinder.setDiameterX(val);}
	float diameterY() {return m_cylinder.diameterY();}
	void setDiameterY(float val) {m_cylinder.setDiameterY(val);}
	float length() {return m_cylinder.length();}
	void setLength(float val) {m_cylinder.setLength(val);}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cylinder.setApodization(val,val,val);}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		Complex ret=m_cylinder.eval(kx,ky,kz,tau);;
		return ret;
	}	
private:
	KspaceCylinder m_cylinder;
	float m_apodization;
};

class VSSpherePhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(QString diameter READ diameter WRITE setDiameter)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	VSSpherePhantom() {m_apodization=1;}
	virtual ~VSSpherePhantom() {}	
	virtual QString typeName() {return "Sphere";}
	QString diameter() {
		return vector3_to_string(m_sphere.diameterX(),m_sphere.diameterY(),m_sphere.diameterZ());
	}
	void setDiameter(QString val) {
		float dx,dy,dz; 
		string_to_vector3(val,dx,dy,dz); 
		m_sphere.setDiameterX(dx); 
		m_sphere.setDiameterY(dy); 
		m_sphere.setDiameterZ(dz);
	}
	float apodization() {return m_apodization;}
	void setApodization(float val) {
		m_apodization=val; 
		m_sphere.setApodization(val,val,val);
	}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		return m_sphere.eval(kx,ky,kz,tau);
	}	
private:
	KspaceSphere m_sphere;
	float m_apodization;
};


class VSAnnulusPhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(float diameterX READ diameterX WRITE setDiameterX)
	Q_PROPERTY(float diameterY READ diameterY WRITE setDiameterY)
	Q_PROPERTY(float innerDiameterX READ innerDiameterX WRITE setInnerDiameterX)
	Q_PROPERTY(float innerDiameterY READ innerDiameterY WRITE setInnerDiameterY)
	Q_PROPERTY(float length READ length WRITE setLength)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	VSAnnulusPhantom() {
		setDiameterX(50);
		setInnerDiameterX(40);
		setDiameterY(50);
		setInnerDiameterY(40);
		setLength(50);
		m_apodization=1;
	}
	virtual ~VSAnnulusPhantom() {}	
	virtual QString typeName() {return "Annulus";}
	float diameterX() {return m_cylinder.diameterX();}
	void setDiameterX(float val) {m_cylinder.setDiameterX(val);}
	float diameterY() {return m_cylinder.diameterY();}
	void setDiameterY(float val) {m_cylinder.setDiameterY(val);}
	float innerDiameterX() {return m_inner_cylinder.diameterX();}
	void setInnerDiameterX(float val) {m_inner_cylinder.setDiameterX(val);}
	float innerDiameterY() {return m_inner_cylinder.diameterY();}
	void setInnerDiameterY(float val) {m_inner_cylinder.setDiameterY(val);}
	float length() {return m_cylinder.length();}
	void setLength(float val) {m_cylinder.setLength(val); m_inner_cylinder.setLength(val);}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cylinder.setApodization(val,val,val); m_inner_cylinder.setApodization(val,val,val);}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		return m_cylinder.eval(kx,ky,kz,tau)
			- m_inner_cylinder.eval(kx,ky,kz,tau);
	}	
private:
	KspaceCylinder m_cylinder;
	KspaceCylinder m_inner_cylinder;
	float m_apodization;
};

class VSCubeWithHolePhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(float lengthX READ lengthX WRITE setLengthX)
	Q_PROPERTY(float lengthY READ lengthY WRITE setLengthY)
	Q_PROPERTY(float lengthZ READ lengthZ WRITE setLengthZ)
	Q_PROPERTY(float holeDiameterX READ holeDiameterX WRITE setHoleDiameterX)
	Q_PROPERTY(float holeDiameterY READ holeDiameterY WRITE setHoleDiameterY)	
	Q_PROPERTY(float holeOffsetX READ holeOffsetX WRITE setHoleOffsetX)
	Q_PROPERTY(float holeOffsetY READ holeOffsetY WRITE setHoleOffsetY)
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
public:
	VSCubeWithHolePhantom() {
		setLengthX(80);
		setLengthY(80);		
		setLengthZ(50);
		setHoleDiameterX(40);
		setHoleDiameterY(40);
		setHoleOffsetX(15);
		setHoleOffsetY(10);
		m_apodization=1;
	}
	virtual ~VSCubeWithHolePhantom() {}	
	virtual QString typeName() {return "Cube With Hole";}
	float lengthX() {return m_cube.lengthX();}
	float lengthY() {return m_cube.lengthY();}
	float holeDiameterX() {return m_inner_cylinder.diameterX();}
	float holeDiameterY() {return m_inner_cylinder.diameterY();}
	float holeOffsetX() {return m_hole_offset_x;}
	float holeOffsetY() {return m_hole_offset_y;}
	void setLengthX(float val) {m_cube.setLengthX(val);}
	void setLengthY(float val) {m_cube.setLengthY(val);}
	void setHoleDiameterX(float val) {m_inner_cylinder.setDiameterX(val);}
	void setHoleDiameterY(float val) {m_inner_cylinder.setDiameterY(val);}
	void setHoleOffsetX(float val) {m_hole_offset_x=val;}
	void setHoleOffsetY(float val) {m_hole_offset_y=val;}
	
	float lengthZ() {return m_cube.lengthZ();}
	void setLengthZ(float val) {m_cube.setLengthZ(val); m_inner_cylinder.setLength(val);}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cube.setApodization(val,val,val); m_inner_cylinder.setApodization(val,val,val);}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		double cylinder_phase=(kx*m_hole_offset_x+ky*m_hole_offset_y)*2*PI;
		Complex cylinder_factor=exp(Complex(0,cylinder_phase));
		return m_cube.eval(kx,ky,kz,tau)
			- m_inner_cylinder.eval(kx,ky,kz,tau)*cylinder_factor;
	}	
private:
	KspaceCube m_cube;
	KspaceCylinder m_inner_cylinder;
	float m_apodization;
	double m_hole_offset_x;
	double m_hole_offset_y;
};

#endif
