#ifndef vsabstractscanner_H
#define vsabstractscanner_H

#include <QObject>
#include <math.h>
#include <QDebug>
#include "vsevents.h"
#include "vsobjectstate.h"
#include "vsphantom.h"

typedef double matrix33[3][3];

double randreal(); //return random number between 0 and 1

class VSAbstractScanner : public QObject {
	Q_OBJECT
public:
	VSAbstractScanner();
	virtual ~VSAbstractScanner();
	
	virtual void processRFPulse(const VSRFPulseEvent &RF);
	virtual void processGradient(const VSGradientEvent &G);
	virtual void processReadout(const VSReadoutEvent &ADC,QList<double> &data_real,QList<double> &data_imag);
	virtual QString statusString() {return "";}
	virtual void initializeScan();
	virtual void finalizeScan();
	virtual VSObjectState *createObjectState() {return 0;}
	
	void setPhantom(VSPhantom *P);
	
	void setProgress(long prog,long total,QString msg);
		
	bool isRunning() {return m_is_running;}
	void cancelScan() {m_scan_cancelled=true;}
	bool scanCancelled() {return m_scan_cancelled;}
	void setNoiseLevel(double val) {m_noise_level=val;}
	double noiseLevel() {return m_noise_level;}
	void setB1Scale(double val) {m_b1_scale=val;}
	double b1Scale() {return m_b1_scale;}
	void setB0Inhomogeneity(double valx,double valy,double valz) {m_b0_inhom_x=valx; m_b0_inhom_y=valy; m_b0_inhom_z=valz;}

	void addElapsedTime(double t) {if (m_phantom) m_phantom->addElapsedTime(t);}	
signals:
	void progress(long prog,long total,QString msg);
protected:
	double m_b1_scale;
	double m_b0_inhom_x; //Hz/mm
	double m_b0_inhom_y; //Hz/mm
	double m_b0_inhom_z; //Hz/mm
	VSPhantom *m_phantom;
private:
	bool m_scan_cancelled;
	bool m_is_running;
	double m_noise_level;
};

#endif
