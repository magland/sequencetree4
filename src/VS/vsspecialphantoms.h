#ifndef vsspecialphantoms_H
#define vsspecialphantoms_H

#include "vsphantom.h"
#include "basickspacefunctions.h"
#include "basictimefunctions.h"
#include "basicmotionfunctions.h"

class VSDynamicImpulsePhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(float lengthX READ lengthX WRITE setLengthX)
	Q_PROPERTY(float lengthY READ lengthY WRITE setLengthY)
	Q_PROPERTY(float lengthZ READ lengthZ WRITE setLengthZ)
	Q_PROPERTY(float innerDiameterX READ innerDiameterX WRITE setInnerDiameterX)
	Q_PROPERTY(float innerDiameterY READ innerDiameterY WRITE setInnerDiameterY)	
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
	Q_PROPERTY(float impulseStartTime READ impulseStartTime WRITE setImpulseStartTime)
	Q_PROPERTY(float impulsePeriod READ impulsePeriod WRITE setImpulsePeriod)
	Q_PROPERTY(float impulseDuration READ impulseDuration WRITE setImpulseDuration)
	Q_PROPERTY(float impulseTransitionTime READ impulseTransitionTime WRITE setImpulseTransitionTime)
	
public:
	VSDynamicImpulsePhantom() {
		setLengthX(100);
		setLengthY(50);
		setLengthZ(50);
		setInnerDiameterX(40);
		setInnerDiameterY(40);		
		m_apodization=1;
	}
	virtual ~VSDynamicImpulsePhantom() {}	
	virtual QString name() {return "Dynamic Impulse";}
	float lengthX() {return m_cube.lengthX();}
	void setLengthX(float val) {m_cube.setLengthX(val);}
	float lengthY() {return m_cube.lengthY();}
	void setLengthY(float val) {m_cube.setLengthY(val);}
	float innerDiameterX() {return m_inner_cylinder.diameterX();}
	void setInnerDiameterX(float val) {m_inner_cylinder.setDiameterX(val);}
	float innerDiameterY() {return m_inner_cylinder.diameterY();}
	void setInnerDiameterY(float val) {m_inner_cylinder.setDiameterY(val);}
	float lengthZ() {return m_cube.lengthZ();}
	void setLengthZ(float val) {m_cube.setLengthZ(val); m_inner_cylinder.setLength(val);}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cube.setApodization(val,val,val); m_inner_cylinder.setApodization(val,val,val);}
	
	float impulseStartTime() {return m_periodic_impulse.startTime();}
	float impulsePeriod() {return m_periodic_impulse.period();}
	float impulseDuration() {return m_periodic_impulse.duration();}
	float impulseTransitionTime() {return m_periodic_impulse.transitionTime();}
	void setImpulseStartTime(float val) {m_periodic_impulse.setStartTime(val);}
	void setImpulsePeriod(float val) {m_periodic_impulse.setPeriod(val);}
	void setImpulseDuration(float val) {m_periodic_impulse.setDuration(val);}
	void setImpulseTransitionTime(float val) {m_periodic_impulse.setTransitionTime(val);}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		return m_cube.eval(kx,ky,kz,tau)-m_inner_cylinder.eval(kx,ky,kz,tau);
			//+ m_inner_cylinder.eval(kx,ky,kz,tau)*(m_periodic_impulse.eval(elapsed_scan_time)-1);
	}	
private:
	KspaceCube m_cube;
	KspaceCylinder m_inner_cylinder;
	float m_apodization;
	PeriodicImpulse m_periodic_impulse;
};

class VSSingleMotionPhantom : public VSSubphantom {
	Q_OBJECT
	Q_PROPERTY(float lengthX READ lengthX WRITE setLengthX)
	Q_PROPERTY(float lengthY READ lengthY WRITE setLengthY)
	Q_PROPERTY(float lengthZ READ lengthZ WRITE setLengthZ)
	Q_PROPERTY(float innerDiameterX READ innerDiameterX WRITE setInnerDiameterX)
	Q_PROPERTY(float innerDiameterY READ innerDiameterY WRITE setInnerDiameterY)	
	Q_PROPERTY(float apodization READ apodization WRITE setApodization)
	Q_PROPERTY(double startTime READ startTime WRITE setStartTime)
	Q_PROPERTY(double duration READ duration WRITE setDuration)
	Q_PROPERTY(double xTranslation READ xTranslation WRITE setXTranslation)
	Q_PROPERTY(double yTranslation READ yTranslation WRITE setYTranslation)
	Q_PROPERTY(double zTranslation READ zTranslation WRITE setZTranslation)
	Q_PROPERTY(double xyRotation READ xyRotation WRITE setXYRotation)	
	
public:
	VSSingleMotionPhantom() {
		setLengthX(80);
		setLengthY(80);
		setLengthZ(50);
		setInnerDiameterX(40);
		setInnerDiameterY(40);
		m_apodization=1;
		setStartTime(5000);
		setDuration(1000);
		setXTranslation(0);
		setYTranslation(0);
		setZTranslation(0);
		setXYRotation(0);
	}
	virtual ~VSSingleMotionPhantom() {}	
	virtual QString name() {return "Single Motion";}
	float lengthX() {return m_cube.lengthX();}
	void setLengthX(float val) {m_cube.setLengthX(val);}
	float lengthY() {return m_cube.lengthY();}
	void setLengthY(float val) {m_cube.setLengthY(val);}
	float lengthZ() {return m_cube.lengthZ();}
	void setLengthZ(float val) {m_cube.setLengthZ(val); m_inner_cylinder.setLength(val);}
	float innerDiameterX() {return m_inner_cylinder.diameterX();}
	void setInnerDiameterX(float val) {m_inner_cylinder.setDiameterX(val);}
	float innerDiameterY() {return m_inner_cylinder.diameterY();}
	void setInnerDiameterY(float val) {m_inner_cylinder.setDiameterY(val);}
	float apodization() {return m_apodization;}
	void setApodization(float val) {m_apodization=val; m_cube.setApodization(val,val,val); m_inner_cylinder.setApodization(val,val,val);}
	
	double startTime() {return m_motion_func.startTime();}
	double duration() {return m_motion_func.duration();}
	double xTranslation() {return m_motion_func.xTranslation();}
	double yTranslation() {return m_motion_func.yTranslation();}
	double zTranslation() {return m_motion_func.zTranslation();}
	double xyRotation() {return m_motion_func.xyRotation();}
	void setStartTime(double val) {m_motion_func.setStartTime(val);}
	void setDuration(double val) {m_motion_func.setDuration(val);}
	void setXTranslation(double val) {m_motion_func.setXTranslation(val);}
	void setYTranslation(double val) {m_motion_func.setYTranslation(val);}
	void setZTranslation(double val) {m_motion_func.setZTranslation(val);}
	void setXYRotation(double val) {m_motion_func.setXYRotation(val);}
	
	Complex evaluate(double kx,double ky,double kz, double tau) {
		//double phase_factor=0;
		//m_motion_func.transformKspace(elapsed_scan_time,kx,ky,kz,phase_factor);
		//phase_factor*=3.141592/180;
		Complex ret=(m_cube.eval(kx,ky,kz,tau)-m_inner_cylinder.eval(kx,ky,kz,tau));
			//			*exp(Complex(0,phase_factor));
		return ret;
	}	
private:
	KspaceCube m_cube;
	KspaceCylinder m_inner_cylinder;
	float m_apodization;
	SingleMotion m_motion_func;
};


#endif
