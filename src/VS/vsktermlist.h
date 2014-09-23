#ifndef vsktermlist_H
#define vsktermlist_H

#include "vsobjectstate.h"
#include "vsevents.h"

#define TERM_TYPE_LONGITUDINAL 1
#define TERM_TYPE_TRANSVERSE 2


struct Kterm {
	int term_type;
	double alpha_re;
	double alpha_im;
	double kx,ky,kz; //mm^(-1)
	double tau;      //us
	//if term_type = TERM_TYPE_TRANSVERSE then
	//	M_XY(x,y,z,f) = alpha*exp(2*pi*i*[kx*x+ky*y+kz*z+tau*f])
	//if term_type = TERM_TYPE_LONGITUDINAL then
	//  M_Z(x,y,z) = REAL( alpha*exp(2*pi*i*[kx*x+ky*y+kz*z+tau*f]) )
	//where x,y,z are in mm, and f is in KHz
};

bool operator<(const Kterm &T1,const Kterm &T2);

class VSKtermList : public VSObjectState {
	Q_OBJECT
public:
	VSKtermList();
	virtual ~VSKtermList();
	void applyTransformation(const VSObjectTransformation &T); //virtual from VSObjectState
	VSObjectTransformation transformation() {return m_transformation;}
	
	void setT1(double val) {m_t1=val;}
	void setT2(double val) {m_t2=val;}
	void setChemicalShift(double val) {m_chemical_shift=val;}
	double t1() {return m_t1;}
	double t2() {return m_t2;}
	double chemicalShift() {return m_chemical_shift;}
	void reset();
	void processRF(double A[3][3],double B[3]);
	void processRF(const VSRFPulseEvent &RF);
	void processMoment(double mx/*uT/mm-us*/,double my,double mz,double t /*us*/,bool consider_dropping_terms,bool apply_decay=true);
	void processGradient(const VSGradientEvent &G);
	
	long longitudinalCount() {return m_longitudinal_terms.count();}	
	long transverseCount() {return m_transverse_terms.count();}
	Kterm transverseTerm(int index) {return m_transverse_terms[index];}
	Kterm longitudinalTerm(int index) {return m_longitudinal_terms[index];}
	
	void setMinTermSize(double val) {m_min_term_size=val;}
	void setMaxNumTerms(long val) {m_max_num_terms=val;}
	
	void sortTerms();
private:
	VSObjectTransformation m_transformation;
	long m_max_num_terms;
	double m_min_term_size;
	double m_t1,m_t2; //ms
	double m_chemical_shift; //Hz
	double m_gamma;
	double m_shiftx,m_shifty,m_shiftz; //mm
	//total transverse = sum_j( M_XY(transverse_terms[j]) )
	//total longitudinal = 1 + sum_j( M_Z(longitudinal_terms[j]) )
	QList<Kterm> m_transverse_terms;
	QList<Kterm> m_longitudinal_terms;
	//double m_keps,m_teps;
	
	bool is_too_small(Kterm &T);
	bool is_close_to(const Kterm &T1,const Kterm &T2);
	void add_transverse_term(Kterm &T);
	void add_longitudinal_term(Kterm &T);
	
	void remove_terms_below_threshold(double threshold);
};

#endif
