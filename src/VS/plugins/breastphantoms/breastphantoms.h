#ifndef breastphantoms_H
#define breastphantoms_H

#include "vsphantomplugin.h"
#include "vsbasicphantoms.h"
#include "mda.h"

class BreastPhantoms : public QObject, public VSPhantomPlugin {
	Q_OBJECT
	Q_INTERFACES(VSPhantomPlugin)
public:
	BreastPhantoms();
	virtual ~BreastPhantoms();
	virtual QList<VSPhantom *> phantomList();
private:
	QList<VSPhantom *> m_phantom_list;
};

class SampledPhantom : public VSSubphantom {
	Q_OBJECT
	//Q_PROPERTY(float contrast READ contrast WRITE setContrast)
public:
	SampledPhantom() {
	}
	virtual ~SampledPhantom() {}
	virtual QString typeName() {return "BreastPhantom";}
	void setKspace(QString kspace_fname);
	void setResolution(double dx,double dy,double dz);
	void deallocate();
	
	Complex evaluate(double kx,double ky,double kz, double tau);
private:
	Mda m_kspace_data;
	double m_dx,m_dy,m_dz;
	
	Complex eval_kspace(int i1,int i2,int i3);
	double eval_kernel(double x,double y,double z);
};

class BreastPhantom : public VSPhantom {
public:
	BreastPhantom();
	virtual ~BreastPhantom();
	void allocate();
	void deallocate();
private:
	SampledPhantom *m_a_subphantom;
	SampledPhantom *m_fg_subphantom;
};




#endif
