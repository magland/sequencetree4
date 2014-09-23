#include "vsphantom.h"
#include "complex.h"
#include <QDebug>

VSSubphantom::VSSubphantom() {
	m_t1=300;
	m_t2=40;
	m_chemical_shift=0;
}

VSPhantom::VSPhantom() 
{
}

VSPhantom::~VSPhantom()
{
	for (int j=0; j<m_subphantom_data.count(); j++) 
		if (m_subphantom_data[j].state) delete m_subphantom_data[j].state;
}

void VSPhantom::addSubphantom(VSSubphantom *P,QString name) {
	P->m_parent=this;
	P->setParent(this);
	P->setName(name);
	P->setObjectName(name);
	m_subphantoms << P;
	VSSubphantomData D;
	D.intensity=1;
	D.state=0;
	m_subphantom_data << D;
}

void VSPhantom::setSubphantomIntensity(int index,double val) {
	m_subphantom_data[index].intensity=val;
}
void VSPhantom::setSubphantomObjectState(int index,VSObjectState *S) {
	if (m_subphantom_data[index].state) delete m_subphantom_data[index].state;
	m_subphantom_data[index].state=S;
}

Complex VSPhantom::evaluateSubphantom(int index,double kx,double ky,double kz,double tau) {
	double phase_offset=0;
	double scale_factor=1;
	m_subphantom_data[index].transformation.applyToKspace(kx,ky,kz,phase_offset,scale_factor);
	Complex ret=m_subphantoms[index]->evaluate(kx,ky,kz,tau);	
	if (phase_offset!=0) {
		ret=ret*exp(Complex(0,phase_offset*PI*2/180));
	}
	scale_factor*=m_subphantom_data[index].intensity;
	if (scale_factor!=1) {
		ret=ret*scale_factor;
	}
	return ret;
}

Complex VSPhantom::evaluate(double kx,double ky,double kz,double tau) {
	//default implementation
	Complex ret=Complex(0,0);
	for (int j=0; j<subphantomCount(); j++) {
		ret=ret+evaluateSubphantom(j,kx,ky,kz,tau);
	}
	return ret;
}
void VSPhantom::addElapsedTime(double t) {
	//default implementation
	for (int j=0; j<subphantomCount(); j++) {
		m_subphantoms[j]->addElapsedTime(t);
	}
}
void VSPhantom::reset() {
	//default implementation
	for (int j=0; j<subphantomCount(); j++) {
		m_subphantoms[j]->reset();
	}
}

void VSPhantom::transformSubphantom(int index,const VSObjectTransformation &T) {
	VSObjectState *S=m_subphantom_data[index].state;
	if (!S) return;
	S->applyTransformation(T);
	m_subphantom_data[index].transformation.leftMultiply(T);
}
void VSPhantom::setSubphantomTransformation(int index,const VSObjectTransformation &T) {
	VSObjectTransformation T2=m_subphantom_data[index].transformation;
	T2.invert();
	VSObjectTransformation T3=T2;
	T3.leftMultiply(T);
	transformSubphantom(index,T3);
}

