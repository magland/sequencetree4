#include "breastphantoms.h"
#include <QtPlugin>
#include <math.h>

double sinc(double x) {
	double ret;
	double x2=x*3.141592654;
	if ((x2<-0.005)||(x2>0.005)) {
		ret=sin(x2)/x2;
		return ret;
	}
	else {
		ret=1-x2*x2/6+x2*x2*x2*x2/60;
	}
	return ret;
}


Q_EXPORT_PLUGIN2(breastphantoms,BreastPhantoms)

BreastPhantoms::BreastPhantoms() {
	VSPhantom *hold;
	
	hold=new BreastPhantom; 
	hold->setName("Breast");
	m_phantom_list << hold;
}
BreastPhantoms::~BreastPhantoms() {
	qDeleteAll(m_phantom_list);
}
QList<VSPhantom *> BreastPhantoms::phantomList() {
	return m_phantom_list;
}
void SampledPhantom::setKspace(QString kspace_fname) {
	#ifdef WIN32 //;
	kspace_fname.replace("/","\\");
	#endif
	m_kspace_data.read(kspace_fname.toAscii().data());
}
void SampledPhantom::setResolution(double dx,double dy,double dz) {
	m_dx=dx;
	m_dy=dy;
	m_dz=dz;
}
void SampledPhantom::deallocate() {
	m_kspace_data.clear();
}
BreastPhantom::BreastPhantom() {
	m_a_subphantom=new SampledPhantom;
	m_fg_subphantom=new SampledPhantom;
	
	addSubphantom(m_a_subphantom,"A");
	addSubphantom(m_fg_subphantom,"FG");
}

BreastPhantom::~BreastPhantom() {
	delete m_a_subphantom;
	delete m_fg_subphantom;
}

void BreastPhantom::allocate() {
	QString a_fname="c:/dev/st4/vsplugins/data/breast/kspace_A_1mm.mda";
	qDebug()  << "Reading:" << a_fname;
	m_a_subphantom->setKspace(a_fname);
	m_a_subphantom->setResolution(1,1,1);
	
	QString fg_fname="c:/dev/st4/vsplugins/data/breast/kspace_FG_1mm.mda";
	qDebug()  << "Reading:" << fg_fname;
	m_fg_subphantom->setKspace(fg_fname);
	m_fg_subphantom->setResolution(1,1,1);
}

void BreastPhantom::deallocate() {
	m_a_subphantom->deallocate();
	m_fg_subphantom->deallocate();
}

Complex SampledPhantom::eval_kspace(int i1,int i2,int i3) {
	if ((i1<0)||(i2<0)||(i3<0)) return Complex(0,0);
	if ((i1>=m_kspace_data.size(0))||(i2>=m_kspace_data.size(1))||(i3>=m_kspace_data.size(2))) return Complex(0,0);
	return m_kspace_data(i1,i2,i3);
}
double SampledPhantom::eval_kernel(double x,double y,double z) {
	return sinc(x)*sinc(y)*sinc(z);
	//return exp(-x*x-y*y-z*z);
}

Complex SampledPhantom::evaluate(double kx,double ky,double kz, double tau) {
	double ix=(kx*m_dx*m_kspace_data.size(0))+m_kspace_data.size(0)/2;
	double iy=(ky*m_dy*m_kspace_data.size(1))+m_kspace_data.size(1)/2;
	double iz=(kz*m_dz*m_kspace_data.size(2))+m_kspace_data.size(2)/2;
	long Ix=(long)ix;
	long Iy=(long)iy;
	long Iz=(long)iz;
	if ((ix<0)||(iy<0)||(iz<0)) return Complex(0,0);
	if ((ix>=m_kspace_data.size(0))||(iy>=m_kspace_data.size(1))||(iz>=m_kspace_data.size(2))) return Complex(0,0);
	Complex ret(0,0);
	for (int dx=-4; dx<=4; dx++)
	for (int dy=-4; dy<=4; dy++)
	for (int dz=-4; dz<=4; dz++) {
		ret=ret+eval_kspace(Ix+dx,Iy+dy,Iz+dz)*eval_kernel(Ix+dx-ix,Iy+dy-iy,Iz+dz-iz);
	}
	return ret;
}
