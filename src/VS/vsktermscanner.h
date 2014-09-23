#ifndef vsktermscanner_H
#define vsktermscanner_H

#include "vsabstractscanner.h"
#include "vsktermlist.h"

class VSKtermScanner : public VSAbstractScanner {
public:
	VSKtermScanner();
	virtual ~VSKtermScanner();
	
	void processRFPulse(const VSRFPulseEvent &RF);
	void processGradient(const VSGradientEvent &G);
	void processReadout(const VSReadoutEvent &ADC,QList<double> &data_real,QList<double> &data_imag);
	QString statusString();
	void initializeScan();
	void finalizeScan();
	VSObjectState *createObjectState() {return new VSKtermList;}
	
	void setMinTermSize(double val);
	void setMaxNumTerms(long val);	
	
	QString getComponentsText();
	
};

#endif
