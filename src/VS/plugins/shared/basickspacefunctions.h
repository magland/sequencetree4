#ifndef basickspacefunctions_H
#define basickspacefunctions_H

#include "kspacefunction.h"


class KspaceCylinder : public KspaceFunction {
	Q_OBJECT
	Q_PROPERTY(float diameterX READ diameterX WRITE setDiameterX)
	Q_PROPERTY(float diameterY READ diameterY WRITE setDiameterY)
	Q_PROPERTY(float length READ length WRITE setLength)	
public:
	KspaceCylinder();
	virtual ~KspaceCylinder();
	virtual QString name() {return "Cylinder";}
	
	float length() {return m_length;}	
	void setLength(float val) {m_length=val;}
	float diameterX() {return m_diameter_x;}
	void setDiameterX(float val) {m_diameter_x=val;}
	float diameterY() {return m_diameter_y;}
	void setDiameterY(float val) {m_diameter_y=val;}
protected:
	virtual void evaluate(double &re, double &im, double kx, double ky, double kz, double tau);
private:
	float m_length;
	float m_diameter_x,m_diameter_y;
};

class KspaceCube : public KspaceFunction {
	Q_OBJECT
	Q_PROPERTY(float lengthX READ lengthX WRITE setLengthX)
	Q_PROPERTY(float lengthY READ lengthY WRITE setLengthY)
	Q_PROPERTY(float lengthZ READ lengthZ WRITE setLengthZ)
public:
	KspaceCube();
	virtual ~KspaceCube();
	virtual QString name() {return "Cube";}
	
	float lengthX() {return m_length_x;}	
	void setLengthX(float val) {m_length_x=val;}
	float lengthY() {return m_length_y;}	
	void setLengthY(float val) {m_length_y=val;}
	float lengthZ() {return m_length_z;}	
	void setLengthZ(float val) {m_length_z=val;}
protected:
	virtual void evaluate(double &re, double &im, double kx, double ky, double kz, double tau);
private:
	float m_length_x, m_length_y, m_length_z;
};

class KspaceSphere : public KspaceFunction {
	Q_OBJECT
	Q_PROPERTY(float diameterX READ diameterX WRITE setDiameterX)
	Q_PROPERTY(float diameterY READ diameterY WRITE setDiameterY)
	Q_PROPERTY(float diameterZ READ diameterZ WRITE setDiameterZ)
public:
	KspaceSphere();
	virtual ~KspaceSphere();
	virtual QString name() {return "Sphere";}
	
	float diameterX() {return m_diameter_x;}
	void setDiameterX(float val) {m_diameter_x=val;}
	float diameterY() {return m_diameter_y;}
	void setDiameterY(float val) {m_diameter_y=val;}
	float diameterZ() {return m_diameter_z;}
	void setDiameterZ(float val) {m_diameter_z=val;}
protected:
	virtual void evaluate(double &re, double &im, double kx, double ky, double kz, double tau);
private:
	float m_diameter_x, m_diameter_y, m_diameter_z;
};



#endif
