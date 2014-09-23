#ifndef motionphantoms1_H
#define motionphantoms1_H

#include "vsphantomplugin.h"
#include "vsbasicphantoms.h"

class MotionPhantoms1 : public QObject, public VSPhantomPlugin {
	Q_OBJECT
	Q_INTERFACES(VSPhantomPlugin)
public:
	MotionPhantoms1();
	virtual ~MotionPhantoms1();
	virtual QList<VSPhantom *> phantomList();
private:
	QList<VSPhantom *> m_phantom_list;
};

class RotatingPhantom : public VSPhantom {
	Q_OBJECT
	Q_PROPERTY(double rotationFrequency READ rotationFrequency WRITE setRotationFrequency)
public:
	RotatingPhantom();
	virtual ~RotatingPhantom();
	virtual Complex evaluate(double kx,double ky,double kz,double tau);
	virtual void addElapsedTime(double t);
	virtual void reset();
	
	double rotationFrequency() {return m_rotation_frequency;}
	void setRotationFrequency(double val) {m_rotation_frequency=val;}
private:
	double m_rotation_frequency; //Hz
};

class RotatingCube : public RotatingPhantom {
	Q_OBJECT
public:
	RotatingCube() {
		setName("Rotating Cube");
		m_cube=new VSCubePhantom;
		addSubphantom(m_cube,"Cube");
	}
	virtual ~RotatingCube() {}
private:
	VSCubePhantom *m_cube;
};


class RotatingCubeWithHole : public RotatingPhantom {
	Q_OBJECT
public:
	RotatingCubeWithHole() {
		setName("Rotating Cube With Hole");
		m_cube_with_hole=new VSCubeWithHolePhantom;
		addSubphantom(m_cube_with_hole,"Cube With Hole");
	}
	virtual ~RotatingCubeWithHole() {}
private:
	VSCubeWithHolePhantom *m_cube_with_hole;
};

class BeatingHeart : public VSPhantom {
	Q_OBJECT
	Q_PROPERTY(double period READ period WRITE setPeriod)
	Q_PROPERTY(double amplitude READ amplitude WRITE setAmplitude)
public:
	BeatingHeart();
	virtual ~BeatingHeart();
	virtual Complex evaluate(double kx,double ky,double kz,double tau);
	virtual void addElapsedTime(double t);
	virtual void reset();
	
	double period() {return m_period;}
	void setPeriod(double val) {m_period=val;}
	double amplitude() {return m_amplitude;}
	void setAmplitude(double val) {m_amplitude=val;}
private:
	VSCubeWithHolePhantom *m_subphantom;
	double m_period; //ms
	double m_amplitude;
	double m_elapsed_time;
};



#endif
