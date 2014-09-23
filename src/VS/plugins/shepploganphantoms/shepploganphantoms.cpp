#include "shepploganphantoms.h"
#include <QtPlugin>
#include <math.h>

Q_EXPORT_PLUGIN2(shepploganphantoms,SheppLoganPhantoms)

SheppLoganPhantoms::SheppLoganPhantoms() {
	VSPhantom *hold;
	
	hold=new VSPhantom; 
	hold->addSubphantom(new SheppLoganPhantom,"Shepp-Logan");
	hold->setName("Shepp-Logan");
	m_phantom_list << hold;
}
SheppLoganPhantoms::~SheppLoganPhantoms() {
	qDeleteAll(m_phantom_list);
}
QList<VSPhantom *> SheppLoganPhantoms::phantomList() {
	return m_phantom_list;
}

Complex shiftfactor(double kval,double shiftvalue) {
	return exp(Complex(0,2*PI*kval*shiftvalue));
}

void rotate_kxy(double &kx2,double &ky2,double kx,double ky,double angle) {
	double theta=angle*PI/180;
	kx2=cos(theta)*kx+sin(theta)*ky;
	ky2=-sin(theta)*kx+cos(theta)*ky;
}


