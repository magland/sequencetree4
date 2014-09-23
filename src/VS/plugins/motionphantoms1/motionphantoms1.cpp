#include "motionphantoms1.h"
#include "motionphantoms1.h"
#include <QtPlugin>
#include <math.h>

Q_EXPORT_PLUGIN2(motionphantoms1,MotionPhantoms1)

MotionPhantoms1::MotionPhantoms1() {
	m_phantom_list << new RotatingCube;
	m_phantom_list << new RotatingCubeWithHole;
	m_phantom_list << new BeatingHeart;	
}
MotionPhantoms1::~MotionPhantoms1() {
	qDeleteAll(m_phantom_list);
}
QList<VSPhantom *> MotionPhantoms1::phantomList() {
	return m_phantom_list;
}


RotatingPhantom::RotatingPhantom() {
	m_rotation_frequency=0;
}
RotatingPhantom::~RotatingPhantom() {
}

Complex RotatingPhantom::evaluate(double kx,double ky,double kz,double tau) {
	qDebug() << __FUNCTION__ << __LINE__;
	return VSPhantom::evaluate(kx,ky,kz,tau);
}
void RotatingPhantom::addElapsedTime(double t) {
	qDebug() << __FUNCTION__ << __LINE__;
	t/=1000;
	VSObjectTransformation T;
	double angle=t/1000*360*m_rotation_frequency;
	T.xyRotate(angle);
	transformSubphantom(0,T);
	VSPhantom::addElapsedTime(t);
}
void RotatingPhantom::reset() {
	VSObjectTransformation T;
	setSubphantomTransformation(0,T);
}

BeatingHeart::BeatingHeart() {
	setName("Beating Heart");
	m_elapsed_time=0;
	m_period=1000;
	m_subphantom=new VSCubeWithHolePhantom;
	m_amplitude=0.2;
	addSubphantom(m_subphantom,"Cube With Hole");
}
BeatingHeart::~BeatingHeart() {
}
Complex BeatingHeart::evaluate(double kx,double ky,double kz,double tau) {
	return VSPhantom::evaluate(kx,ky,kz,tau);
}
void BeatingHeart::addElapsedTime(double t) {
	if (m_period<=0) return;
	m_elapsed_time+=t;
	if (m_period>0) {
		while (m_elapsed_time/1000>m_period) m_elapsed_time-=m_period*1000;
		while (m_elapsed_time<0) m_elapsed_time+=m_period*1000;
	}
	double holdt=m_elapsed_time/1000;
	double tfrac=0;
	if (m_period>0) tfrac=holdt/m_period;	
	double bconst=0.25;
	tfrac=tfrac/(bconst+(1-bconst)*tfrac);
	double compression=1-amplitude()*sin(tfrac*PI)*sin(tfrac*PI);
	VSObjectTransformation T;
	T.scale(compression,sqrt(1/compression),1);
	setSubphantomTransformation(0,T);
	VSPhantom::addElapsedTime(t);
}
void BeatingHeart::reset() {
	VSObjectTransformation T;
	setSubphantomTransformation(0,T);
}

