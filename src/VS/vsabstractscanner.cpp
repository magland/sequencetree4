#include "vsabstractscanner.h"

VSAbstractScanner::VSAbstractScanner() {
	m_scan_cancelled=false;
	m_is_running=false;
	m_noise_level=0;
	m_b1_scale=1;
	m_phantom=0;
	m_b0_inhom_x=m_b0_inhom_y=m_b0_inhom_z=0;
}
VSAbstractScanner::~VSAbstractScanner() {
}

void VSAbstractScanner::processRFPulse(const VSRFPulseEvent &RF) {
	Q_UNUSED(RF);
}
void VSAbstractScanner::processGradient(const VSGradientEvent &G) {
	Q_UNUSED(G);
}
void VSAbstractScanner::processReadout(const VSReadoutEvent &ADC,QList<double> &data_real,QList<double> &data_imag) {
	while (data_real.count()<ADC.GX.count()) {
		data_real << 0;
	}
	while (data_imag.count()<ADC.GX.count()) {
		data_imag << 0;
	}
	
	double noise_factor=0;
	if (ADC.time_step>0) {
		noise_factor=m_noise_level/sqrt(ADC.time_step/1000);
	}
	for (long j=0; j<ADC.GX.count(); j++) {
		if (noise_factor!=0) {
			data_real[j]+=(randreal()*2-1)*noise_factor;
			data_imag[j]+=(randreal()*2-1)*noise_factor;
		}
	}
}


double randreal() {
	return ((double)rand())/RAND_MAX;
}

void VSAbstractScanner::setProgress(long prog,long total,QString msg) {
	emit progress(prog,total,msg);
}
void VSAbstractScanner::initializeScan() {
	m_scan_cancelled=false;
	m_is_running=true;
}
void VSAbstractScanner::finalizeScan() {
	m_is_running=false;
}

void VSAbstractScanner::setPhantom(VSPhantom *P) {
	m_phantom=P;
	for (int j=0; j<m_phantom->subphantomCount(); j++) {
		m_phantom->setSubphantomObjectState(j,createObjectState());
	}
}
