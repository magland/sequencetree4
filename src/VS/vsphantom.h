#ifndef vsphantom_H
#define vsphantom_H

#include <QObject>
#include "vsobjecttransformation.h"
#include "vsobjectstate.h"
#include "complex.h"

class VSSubphantom;

struct VSSubphantomData {
	VSObjectTransformation transformation;	
	double intensity;
	VSObjectState *state;
};

class VSPhantom : public QObject {
public:
	VSPhantom();
	virtual ~VSPhantom();
	virtual Complex evaluate(double kx,double ky,double kz,double tau);
	virtual void addElapsedTime(double t);
	virtual void reset();
	virtual void allocate() {};
	virtual void deallocate() {};
	
	QString name() {return m_name;}	
	void setName(QString name) {m_name=name;}
	
	void addSubphantom(VSSubphantom *P,QString name);
	int subphantomCount() {return m_subphantoms.count();}
	VSSubphantom *subphantom(int index) {return m_subphantoms[index];}
	void setSubphantomTransformation(int index,const VSObjectTransformation &X);
	void setSubphantomIntensity(int index,double val);
	void setSubphantomObjectState(int index,VSObjectState *S);
	VSObjectState *subphantomObjectState(int index) {return m_subphantom_data[index].state;}
	Complex evaluateSubphantom(int index,double kx,double ky,double kz,double tau);
protected:
	void transformSubphantom(int index,const VSObjectTransformation &T);
private:
	QList<VSSubphantom *> m_subphantoms;
	QList<VSSubphantomData> m_subphantom_data;
	QString m_name;
};

class VSSubphantom : public QObject {
	Q_OBJECT
	Q_PROPERTY(double T1 READ T1 WRITE setT1)
	Q_PROPERTY(double T2 READ T2 WRITE setT2)
	Q_PROPERTY(double chemicalShift READ chemicalShift WRITE setChemicalShift)
private:
	friend class VSPhantom;
public:
	VSSubphantom();
	VSPhantom *parentPhantom() {return m_parent;}
	virtual Complex evaluate(double kx,double ky,double kz,double tau) {Q_UNUSED(kx) Q_UNUSED(ky) Q_UNUSED(kz) Q_UNUSED(tau) return Complex(0,0);}
	virtual void addElapsedTime(double t) {Q_UNUSED(t)}
	virtual QString typeName() {return "<undefined>";}
	virtual void reset() {}
	
	QString name() {return m_name;}
	void setName(QString name) {m_name=name;}
	
	double T1() {return m_t1;}
	double T2() {return m_t2;}
	double chemicalShift() {return m_chemical_shift;}
	void setT1(double val) {m_t1=val;}
	void setT2(double val) {m_t2=val;}
	void setChemicalShift(double val) {m_chemical_shift=val;}	
private:
	VSPhantom *m_parent;
	QString m_name;
	double m_t1,m_t2; //ms
	double m_chemical_shift; //Hz
};


#endif
