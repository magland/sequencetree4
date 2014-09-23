#include "vsktermlist.h"
#include <math.h>
#include <QDebug>

#define m_keps 0.0001
#define m_teps 500

bool operator<(const Kterm &T1,const Kterm &T2) {
	if (T1.kx+m_keps<T2.kx) return true;
	if (T1.kx>T2.kx+m_keps) return false;
	if (T1.ky+m_keps<T2.ky) return true;
	if (T1.ky>T2.ky+m_keps) return false;
	if (T1.kz+m_keps<T2.kz) return true;
	if (T1.kz>T2.kz+m_keps) return false;
	if (T1.tau+m_teps<T2.tau) return true;
	if (T1.tau>T2.tau+m_teps) return false;
	if (T1.alpha_re*T1.alpha_re+T1.alpha_im*T1.alpha_im<T2.alpha_re*T2.alpha_re+T2.alpha_im*T2.alpha_im) return true;
	return false;
}

VSKtermList::VSKtermList() {
	m_shiftx=m_shifty=m_shiftz=0;
	m_max_num_terms=100000;
	m_min_term_size=0.000001;
	//m_keps=0.001;
	//m_teps=1000;
	m_gamma=42.5764; //Hz/uT
	reset();
}
VSKtermList::~VSKtermList() {
}
void VSKtermList::reset() {
	m_transverse_terms.clear();
	m_longitudinal_terms.clear();
}

void VSKtermList::add_transverse_term(Kterm &T) {
	if (!is_too_small(T))
		m_transverse_terms << T;
}

void VSKtermList::add_longitudinal_term(Kterm &T) {
	if (!is_too_small(T)) {
		if (T.alpha_im<0) {
			T.alpha_im*=(-1);
			T.kx*=(-1);
			T.ky*=(-1);
			T.kz*=(-1);
			T.tau*=(-1);
		}
		m_longitudinal_terms << T;
	}
}


void VSKtermList::processRF(double A[3][3],double B[3]) {
/// A is a 3x3 matrix
/// B is a 3x1 vector
/// Corresponds to the transformation M'=AM + B

	long NT=m_transverse_terms.count();
	long NL=m_longitudinal_terms.count();

	double c1_re=(A[0][0]+A[1][1])/2;
	double c1_im=(A[1][0]-A[0][1])/2;
	double c2_re=(A[0][0]-A[1][1])/2;
	double c2_im=(A[1][0]+A[0][1])/2;
	double c3_re=A[2][0];
	double c3_im=-A[2][1];
	double c4_re=A[0][2]/2;
	double c4_im=A[1][2]/2;
	double c5_re=A[0][2]/2;
	double c5_im=A[1][2]/2;
	double c6=A[2][2];

	//printf("debug c=(%g+%gi,%g+%gi,%g+%gi,%g+%gi,%g+%gi,%g)\n",c1_re,c1_im,c2_re,c2_im,c3_re,c3_im,c4_re,c4_im,c5_re,c5_im,c6);

	Kterm T0,L0;
	T0.alpha_re=c4_re+c5_re+B[0];
	T0.alpha_im=c4_im+c5_im+B[1];
	T0.kx=0; T0.ky=0; T0.kz=0; 
	T0.tau=0; 
	T0.term_type=TERM_TYPE_TRANSVERSE;
	add_transverse_term(T0);

	L0.alpha_re=c6-1+B[2];
	L0.alpha_im=0;
	L0.kx=0; L0.ky=0; L0.kz=0; 
	L0.tau=0; 
	L0.term_type=TERM_TYPE_LONGITUDINAL;
	add_longitudinal_term(L0);

	Kterm T,new_term;
	long j;
	for (j=0; j<NT; j++) {
		T=m_transverse_terms[j];
		new_term.alpha_re=c1_re*T.alpha_re-c1_im*T.alpha_im;
		new_term.alpha_im=c1_im*T.alpha_re+c1_re*T.alpha_im;
		new_term.kx=T.kx; new_term.ky=T.ky; new_term.kz=T.kz;
		new_term.tau=T.tau; 
		new_term.term_type=TERM_TYPE_TRANSVERSE;
		m_transverse_terms[j]=new_term;

		new_term.alpha_re=c2_re*T.alpha_re+c2_im*T.alpha_im;
		new_term.alpha_im=c2_im*T.alpha_re-c2_re*T.alpha_im;
		new_term.kx=-T.kx; new_term.ky=-T.ky; new_term.kz=-T.kz;
		new_term.tau=-T.tau; 
		new_term.term_type=TERM_TYPE_TRANSVERSE;
		add_transverse_term(new_term);

		new_term.alpha_re=c3_re*T.alpha_re-c3_im*T.alpha_im;
		new_term.alpha_im=c3_im*T.alpha_re+c3_re*T.alpha_im;
		new_term.kx=T.kx; new_term.ky=T.ky; new_term.kz=T.kz;
		new_term.tau=T.tau; 
		new_term.term_type=TERM_TYPE_LONGITUDINAL;
		add_longitudinal_term(new_term);
	}
	for (j=0; j<NL; j++) {
		T=m_longitudinal_terms[j];
		new_term.alpha_re=c4_re*T.alpha_re-c4_im*T.alpha_im;
		new_term.alpha_im=c4_im*T.alpha_re+c4_re*T.alpha_im;
		new_term.kx=T.kx; new_term.ky=T.ky; new_term.kz=T.kz;
		new_term.tau=T.tau; 
		new_term.term_type=TERM_TYPE_TRANSVERSE;
		add_transverse_term(new_term);

		new_term.alpha_re=c5_re*T.alpha_re+c5_im*T.alpha_im;
		new_term.alpha_im=c5_im*T.alpha_re-c5_re*T.alpha_im;
		new_term.kx=-T.kx; new_term.ky=-T.ky; new_term.kz=-T.kz;
		new_term.tau=-T.tau; 
		new_term.term_type=TERM_TYPE_TRANSVERSE;
		add_transverse_term(new_term);

		new_term.alpha_re=c6*T.alpha_re;
		new_term.alpha_im=c6*T.alpha_im;
		new_term.kx=T.kx; new_term.ky=T.ky; new_term.kz=T.kz;
		new_term.tau=T.tau; 
		new_term.term_type=TERM_TYPE_LONGITUDINAL;
		m_longitudinal_terms[j]=new_term;
	}	

	//Join terms that are close
	qSort(m_transverse_terms.begin(), m_transverse_terms.end());
	j=1;
	while (j<m_transverse_terms.count()) {
		Kterm T=m_transverse_terms[j];
		Kterm Tprev=m_transverse_terms[j-1];
		if (is_close_to(T,Tprev)) {
			Tprev.alpha_re+=T.alpha_re;
			Tprev.alpha_im+=T.alpha_im;
			m_transverse_terms[j-1]=Tprev;
			m_transverse_terms.removeAt(j);
		}
		else
			j++;
	}
	qSort(m_longitudinal_terms.begin(), m_longitudinal_terms.end());
	j=1;
	while (j<m_longitudinal_terms.count()) {
		Kterm T=m_longitudinal_terms[j];
		Kterm Tprev=m_longitudinal_terms[j-1];
		if (is_close_to(T,Tprev)) {
			Tprev.alpha_re+=T.alpha_re;
			Tprev.alpha_im+=T.alpha_im;
			m_longitudinal_terms[j-1]=Tprev;
			m_longitudinal_terms.removeAt(j);
		}
		else
			j++;
	}
}

bool VSKtermList::is_too_small(Kterm &T) {
	double mag=sqrt(T.alpha_re*T.alpha_re+T.alpha_im*T.alpha_im);
	return (mag<m_min_term_size);
}

bool VSKtermList::is_close_to(const Kterm &T1,const Kterm &T2) {
	if (T1.kx>T2.kx+m_keps) return false;
	if (T1.kx<T2.kx-m_keps) return false;
	if (T1.ky>T2.ky+m_keps) return false;
	if (T1.ky<T2.ky-m_keps) return false;
	if (T1.kz>T2.kz+m_keps) return false;
	if (T1.kz<T2.kz-m_keps) return false;
	if (T1.tau>T2.tau+m_teps) return false;
	if (T1.tau<T2.tau-m_teps) return false;
	return true;
}

void VSKtermList::remove_terms_below_threshold(double threshold) {
	long j=0;
	while (j<m_transverse_terms.count()) {
		Kterm T=m_transverse_terms[j];
		if (sqrt(T.alpha_re*T.alpha_re+T.alpha_im*T.alpha_im)<threshold)
			m_transverse_terms.removeAt(j);
		else
			j++;
	}
	j=0;
	while (j<m_longitudinal_terms.count()) {
		Kterm T=m_longitudinal_terms[j];
		if (sqrt(T.alpha_re*T.alpha_re+T.alpha_im*T.alpha_im)<threshold)
			m_longitudinal_terms.removeAt(j);
		else
			j++;
	}
}


void VSKtermList::processMoment(double mx,double my,double mz, //uT/mm - us
							   double t,  //us
							   bool consider_dropping_terms,
							   bool apply_decay) {
	double wx=mx*m_gamma*1.0E-6; //cycles/mm
	double wy=my*m_gamma*1.0E-6;
	double wz=mz*m_gamma*1.0E-6;
	
	long NT=m_transverse_terms.count();
	long NL=m_longitudinal_terms.count();
	
	double phase_angle=m_chemical_shift*t/1.0E6*2*PI;
	double phase_re=cos(phase_angle);
	double phase_im=sin(phase_angle);

	double T2decay=1;
	if (apply_decay) T2decay=exp(-t/1000/m_t2);
	long j;
	for (j=0; j<NT; j++) {
		m_transverse_terms[j].kx+=wx; m_transverse_terms[j].ky+=wy; m_transverse_terms[j].kz+=wz;
		m_transverse_terms[j].tau+=t; 
		double re2=m_transverse_terms[j].alpha_re;
		double im2=m_transverse_terms[j].alpha_im;
		m_transverse_terms[j].alpha_re=re2*phase_re-im2*phase_im;
		m_transverse_terms[j].alpha_im=re2*phase_im+im2*phase_re;
		if (apply_decay) {
			m_transverse_terms[j].alpha_re*=T2decay;
			m_transverse_terms[j].alpha_im*=T2decay;
		}
	}
	if (apply_decay) {
		double T1decay=exp(-t/1000/m_t1);
		for (j=0; j<NL; j++) {
			m_longitudinal_terms[j].alpha_re*=T1decay;
			m_longitudinal_terms[j].alpha_im*=T1decay;
		}
	}
	if (consider_dropping_terms) {
		remove_terms_below_threshold(m_min_term_size);
		if (m_transverse_terms.count()+m_longitudinal_terms.count()>m_max_num_terms) {
			QList<double> term_sizes;
			for (long i=0; i<m_transverse_terms.count(); i++) {
				Kterm T=m_transverse_terms[i];
				term_sizes << sqrt(T.alpha_re*T.alpha_re+T.alpha_im*T.alpha_im);
			}
			for (long i=0; i<m_longitudinal_terms.count(); i++) {
				Kterm T=m_longitudinal_terms[i];
				term_sizes << sqrt(T.alpha_re*T.alpha_re+T.alpha_im*T.alpha_im);
			}
			qSort(term_sizes);
			long num_to_delete=m_transverse_terms.count()+m_longitudinal_terms.count()-m_max_num_terms;
			double threshold=term_sizes[num_to_delete];
			qWarning() << QString("Maximum number of terms exceeded, dropping terms below threshold: %1").arg(threshold);
			remove_terms_below_threshold(threshold);
		}
	}
}

typedef double matrix33[3][3];

void setRFAB(double RF_re,double RF_im,matrix33 A,double *B,double gamma,double decayT1,double decayT2,double precession /*radians*/) {

	double RF_mag=sqrt(RF_re*RF_re+RF_im*RF_im);
	double phi=(RF_mag*gamma*1.0E-6*2*PI);
	double theta=atan2(RF_im,RF_re);

	double cosphi=cos(phi);
	double sinphi=sin(phi);
	double costheta=cos(theta);
	double sintheta=sin(theta);

	double M1[3][3];
	M1[0][0]=(1+(cosphi-1)*sintheta*sintheta)  ;
	M1[1][0]=(-(cosphi-1)*sintheta*costheta)   ;
	M1[2][0]=(-sinphi*sintheta)                ;

	M1[0][1]=(-(cosphi-1)*sintheta*costheta)   ;
	M1[1][1]=(1+(cosphi-1)*costheta*costheta)  ;
	M1[2][1]=(sinphi*costheta)                 ;

	M1[0][2]=(sinphi*sintheta)                 ;
	M1[1][2]=(-sinphi*costheta)                ;
	M1[2][2]=(cosphi)                          ;


	double M2[3][3];
	M2[0][0]=cos(precession)   *decayT2;
	M2[1][0]=sin(precession)   *decayT2;
	M2[2][0]=0;

	M2[0][1]=-sin(precession)  *decayT2;
	M2[1][1]=cos(precession)   *decayT2;
	M2[2][1]=0;

	M2[0][2]=0;
	M2[1][2]=0;
	M2[2][2]=					decayT1;

	int j,k;

	double A2[3][3],A3[3][3];
	double B2[3],B3[3];

	A2[0][0]=M1[0][0]*A[0][0]+M1[0][1]*A[1][0]+M1[0][2]*A[2][0];
	A2[0][1]=M1[0][0]*A[0][1]+M1[0][1]*A[1][1]+M1[0][2]*A[2][1];
	A2[0][2]=M1[0][0]*A[0][2]+M1[0][1]*A[1][2]+M1[0][2]*A[2][2];
	A2[1][0]=M1[1][0]*A[0][0]+M1[1][1]*A[1][0]+M1[1][2]*A[2][0];
	A2[1][1]=M1[1][0]*A[0][1]+M1[1][1]*A[1][1]+M1[1][2]*A[2][1];
	A2[1][2]=M1[1][0]*A[0][2]+M1[1][1]*A[1][2]+M1[1][2]*A[2][2];
	A2[2][0]=M1[2][0]*A[0][0]+M1[2][1]*A[1][0]+M1[2][2]*A[2][0];
	A2[2][1]=M1[2][0]*A[0][1]+M1[2][1]*A[1][1]+M1[2][2]*A[2][1];
	A2[2][2]=M1[2][0]*A[0][2]+M1[2][1]*A[1][2]+M1[2][2]*A[2][2];

	A3[0][0]=M2[0][0]*A2[0][0]+M2[0][1]*A2[1][0]+M2[0][2]*A2[2][0];
	A3[0][1]=M2[0][0]*A2[0][1]+M2[0][1]*A2[1][1]+M2[0][2]*A2[2][1];
	A3[0][2]=M2[0][0]*A2[0][2]+M2[0][1]*A2[1][2]+M2[0][2]*A2[2][2];
	A3[1][0]=M2[1][0]*A2[0][0]+M2[1][1]*A2[1][0]+M2[1][2]*A2[2][0];
	A3[1][1]=M2[1][0]*A2[0][1]+M2[1][1]*A2[1][1]+M2[1][2]*A2[2][1];
	A3[1][2]=M2[1][0]*A2[0][2]+M2[1][1]*A2[1][2]+M2[1][2]*A2[2][2];
	A3[2][0]=M2[2][0]*A2[0][0]+M2[2][1]*A2[1][0]+M2[2][2]*A2[2][0];
	A3[2][1]=M2[2][0]*A2[0][1]+M2[2][1]*A2[1][1]+M2[2][2]*A2[2][1];
	A3[2][2]=M2[2][0]*A2[0][2]+M2[2][1]*A2[1][2]+M2[2][2]*A2[2][2];

	B2[0]=M1[0][0]*B[0]+M1[0][1]*B[1]+M1[0][2]*B[2];
	B2[1]=M1[1][0]*B[0]+M1[1][1]*B[1]+M1[1][2]*B[2];
	B2[2]=M1[2][0]*B[0]+M1[2][1]*B[1]+M1[2][2]*B[2];

	B3[0]=M2[0][0]*B2[0]+M2[0][1]*B2[1]+M2[0][2]*B2[2];
	B3[1]=M2[1][0]*B2[0]+M2[1][1]*B2[1]+M2[1][2]*B2[2];
	B3[2]=M2[2][0]*B2[0]+M2[2][1]*B2[1]+M2[2][2]*B2[2];

	for (j=0; j<3; j++)
		for (k=0; k<3; k++)
			A[j][k]=A3[j][k];
	for (j=0; j<3; j++)
		B[j]=B3[j];

	B[2]+=1-decayT1;
}


void VSKtermList::processRF(const VSRFPulseEvent &RF) {
	double step=RF.time_step; //microseconds

	matrix33 A;
	double B[3];
	
	for (int j=0; j<3; j++) {
		for (int k=0; k<3; k++) {
			if (j==k) A[j][k]=1;
			else A[j][k]=0;
		}
	}
	for (long j=0; j<3; j++)
		B[j]=0;

	double decayT1=exp(-RF.time_step/1000/m_t1);
	double decayT2=exp(-RF.time_step/1000/m_t2);
	qDebug() << "decayT2=" << decayT2;
	double precession=0;
	if ((RF.GX.value(0)==0)&&(RF.GY.value(0)==0)&&(RF.GZ.value(0)==0)) {
		precession=m_chemical_shift*RF.time_step/1.0E6*2*PI;
	}
	for (int j=0; j<RF.data_real.count(); j++) {
		double re,im;
		re=RF.data_real[j]*step;
		im=RF.data_imag[j]*step;
		double ph=RF.phase[j]*PI/180;
		double re2=re*cos(ph)-im*sin(ph);
		double im2=re*sin(ph)+im*cos(ph);

		setRFAB(re2,im2,A,B,m_gamma,decayT1,decayT2,precession);
	}

	/*printf("Rotation matrix:\n");
	for (j=0; j<3; j++) {
		for (k=0; k<3; k++) {
			printf("%g ",A[j][k]);
		}
		printf("\n");
	}
	printf("\n");*/


	long center_point=0;
	double holdmax=0;
	for (long j=0; j<RF.data_real.count(); j++) {
		double magsqr=RF.data_real[j]*RF.data_real[j]+RF.data_imag[j]*RF.data_imag[j];
		if (magsqr>holdmax) {
			holdmax=magsqr;
			center_point=j;
		}
	}

	double M1x=0,M1y=0,M1z=0;
	double M2x=0,M2y=0,M2z=0;
	/*double t1=0;
	double t2=0;
	for (long j=0; j<RF.GX.count(); j++) {
		if (j<center_point) {
			M1x+=RF.GX[j]*RF.time_step; //uT/mm * us
			M1y+=RF.GY[j]*RF.time_step;
			M1z+=RF.GZ[j]*RF.time_step;
			t1+=RF.time_step;
		}
		else {
			M2x+=RF.GX[j]*RF.time_step;
			M2y+=RF.GY[j]*RF.time_step;
			M2z+=RF.GZ[j]*RF.time_step;
			t2+=RF.time_step;
		}
	}*/

	//processMoment(M1x,M1y,M1z,t1,true,true);
	processRF(A,B);
	//processMoment(M2x,M2y,M2z,t2,true,true);
}

void VSKtermList::processGradient(const VSGradientEvent &G)
{
	double momx=G.MX;
	double momy=G.MY;
	double momz=G.MZ;
	processMoment(momx,momy,momz,G.duration,true,true);
}

/*void VSKtermList::computeSignal(double &re,double &im,double elapsed_scan_time) {
	re=0;
	im=0;
	for (long j=0; j<m_transverse_terms.count(); j++) {
		Kterm T=m_transverse_terms[j];
		Complex val=m_phantom->eval(T.kx,T.ky,T.kz,T.tau,elapsed_scan_time);
		re+=T.alpha_re*val.re()-T.alpha_im*val.im();
		im+=T.alpha_re*val.im()+T.alpha_im*val.re();
	}
}*/

void apply_to_term(const VSObjectTransformation &T,Kterm &term) {
	double phase_offset,scale_factor;
	T.applyToKspace(term.kx,term.ky,term.kz,phase_offset,scale_factor);		
	if (phase_offset!=0) {
		double theta=phase_offset*PI/180;
		double re=term.alpha_re;
		double im=term.alpha_im;
		term.alpha_re=re*cos(theta)-im*sin(theta);
		term.alpha_im=re*sin(theta)+im*cos(theta);
	}
	if (scale_factor!=1) {
		term.alpha_re*=scale_factor;
		term.alpha_im*=scale_factor;
	}
}


//virtual from VSObjectState
void VSKtermList::applyTransformation(const VSObjectTransformation &T) { 
	m_transformation.leftMultiply(T);
	VSObjectTransformation Tinv=T;
	Tinv.invert();
	
	for (long j=0; j<m_transverse_terms.count(); j++) {
		apply_to_term(Tinv,m_transverse_terms[j]);
	}
	for (long j=0; j<m_longitudinal_terms.count(); j++) {
		apply_to_term(Tinv,m_longitudinal_terms[j]);
	}
}

void VSKtermList::sortTerms() {
	qSort(m_transverse_terms.begin(), m_transverse_terms.end());
	qSort(m_longitudinal_terms.begin(), m_longitudinal_terms.end());
}

