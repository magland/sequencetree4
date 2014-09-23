#include "vsktermscanner.h"
#include <QDebug>

VSKtermScanner::VSKtermScanner() 
{
}

VSKtermScanner::~VSKtermScanner()
{
}

void VSKtermScanner::processRFPulse(const VSRFPulseEvent &RF) {
	if (!m_phantom) return;
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
		if (L) {
			qDebug() << "Processing RF:" << RF.time_step << RF.data_real.count();
			L->processRF(RF);
		}
	}
		
	VSAbstractScanner::processRFPulse(RF);
}
void VSKtermScanner::processGradient(const VSGradientEvent &G) {
	if (!m_phantom) return;
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
		if (L) {
			L->processGradient(G);
		}
	}
			
	VSAbstractScanner::processGradient(G);
}
void VSKtermScanner::processReadout(const VSReadoutEvent &ADC,QList<double> &data_real,QList<double> &data_imag) {
	if (!m_phantom) return;
	
	while (data_real.count()<ADC.GX.count()) {
		data_real << 0;
	}
	while (data_imag.count()<ADC.GX.count()) {
		data_imag << 0;
	}
		
	long N=ADC.GX.count(); 
	for (int ind=0; ind<N; ind++) {
		double re=0;
		double im=0;
		
		for (int j=0; j<m_phantom->subphantomCount(); j++) {
			VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
			VSSubphantom *Sub=m_phantom->subphantom(j);
			if (ind==0) {
				L->processMoment(ADC.GX[ind]*ADC.time_step/2,			
							ADC.GY[ind]*ADC.time_step/2,
							ADC.GZ[ind]*ADC.time_step/2,
							ADC.time_step/2,false,true);
			}
			
			double re2=0;
			double im2=0;
			for (long jj=0; jj<L->transverseCount(); jj++) {
				Kterm T=L->transverseTerm(jj);
				double kx2=T.kx+m_b0_inhom_x*T.tau/1.0E6;
				double ky2=T.ky+m_b0_inhom_y*T.tau/1.0E6;
				double kz2=T.kz+m_b0_inhom_z*T.tau/1.0E6;
				Complex val=m_phantom->evaluateSubphantom(j,kx2,ky2,kz2,T.tau);
				//Complex val=Sub->evaluate(T.kx,T.ky,T.kz,T.tau);
				re2+=T.alpha_re*val.re()-T.alpha_im*val.im();
				im2+=T.alpha_re*val.im()+T.alpha_im*val.re();
			}			
			
			re+=re2;
			im+=im2;
			if (ind==N-1) {
				L->processMoment(ADC.GX[ind]*ADC.time_step/2,			
								ADC.GY[ind]*ADC.time_step/2,
								ADC.GZ[ind]*ADC.time_step/2,
								ADC.time_step/2,false,true);
			}
			else {
				L->processMoment(ADC.GX[ind]*ADC.time_step,			
								ADC.GY[ind]*ADC.time_step,
								ADC.GZ[ind]*ADC.time_step,
								ADC.time_step,false,true);
			}
		}
		double ph=ADC.phase[ind]*PI/180;
		double re2=re*cos(-ph)-im*sin(-ph);
		double im2=re*sin(-ph)+im*cos(-ph);
		data_real[ind] += re2;
		data_imag[ind] += im2;
	}
	
	//add noise, etc.
	VSAbstractScanner::processReadout(ADC,data_real,data_imag);
}

QString VSKtermScanner::statusString() {
	if (!m_phantom) return "";
	long NL=0;
	long NT=0;
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
		if (L) {
			NT+=L->transverseCount();
			NL+=L->longitudinalCount();
		}
	}
	return QString("%1 Longitudinal, %2 Transverse Terms...").arg(NL).arg(NT);	
}

void VSKtermScanner::setMinTermSize(double val) {
	if (!m_phantom) return;
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
		if (L) {
			L->setMinTermSize(val);
		}
	}
}
void VSKtermScanner::setMaxNumTerms(long val) {
	if (!m_phantom) return;
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
		if (L) {
			L->setMaxNumTerms(val);
		}
	}
}


void VSKtermScanner::initializeScan() {
	if (!m_phantom) return;
	m_phantom->allocate();
	m_phantom->reset();
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(j));
		if (L) {
			L->setT1(m_phantom->subphantom(j)->T1());
			L->setT2(m_phantom->subphantom(j)->T2());
			L->setChemicalShift(m_phantom->subphantom(j)->chemicalShift());
			L->reset();
		}
	}
	VSAbstractScanner::initializeScan();
}
void VSKtermScanner::finalizeScan() {
	VSAbstractScanner::finalizeScan();
	if (m_phantom) {
		m_phantom->deallocate();
	}
}

QString VSKtermScanner::getComponentsText() {
	if (m_phantom->subphantomCount()==0) return "";
	VSKtermList *L=qobject_cast<VSKtermList *>(m_phantom->subphantomObjectState(0));
	if (!L) return "";
	QString str;
	L->sortTerms();
	for (long j=0; j<L->transverseCount(); j++) {
		Kterm term=L->transverseTerm(j);
		str+=QString("T\t%1\t%2\t%3\t%4\t%5\t%6\n")
				.arg(term.alpha_re)
				.arg(term.alpha_im)
				.arg(term.kx)
				.arg(term.ky)
				.arg(term.kz)
				.arg(term.tau);
	}
	for (long j=0; j<L->longitudinalCount(); j++) {
		Kterm term=L->longitudinalTerm(j);
		str+=QString("L\t%1\t%2\t%3\t%4\t%5\t%6\n")
				.arg(term.alpha_re)
				.arg(term.alpha_im)
				.arg(term.kx)
				.arg(term.ky)
				.arg(term.kz)
				.arg(term.tau);
	}
	return str;
}

