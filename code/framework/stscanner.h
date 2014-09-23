#ifndef stscanner_H
#define stscanner_H

#include "stnode.h"

#define ST_EVENT_RF_PULSE 1
#define ST_EVENT_READOUT 2
#define ST_EVENT_GRADIENT 3
#define ST_EVENT_ARB_GRADIENT 4

struct STScannerEventInfo {
	double start_time;
	double phase;
	double frequency;
};

class ST_EXPORT STScannerEvent {
public:
	STScannerEvent(STNode *parent) {m_parent=parent; m_start_time=0;}
	void setStartTime(double t) {m_start_time=t;}
	virtual void setPhase(double phase /*degrees*/);
	virtual void setFrequency(double freq /*Hz*/);
	
	virtual int eventType() {return 0;}
	virtual double duration() {return 0;} //microsec
	virtual double referenceTime() {return 0;}
	
	double startTime() {return m_start_time;}
	double phase(); //degrees
	double frequency(); //Hz
	double endTime() {return m_start_time+duration();}
	
	int treeIndex();
	STNode *parent() {return m_parent;}
	
protected:
	double m_start_time;
	double m_phase;
	double m_frequency;
	STNode *m_parent;
};

class ST_EXPORT STScannerRFPulse : public STScannerEvent {
public:
	STScannerRFPulse(STNode *parent);
	virtual ~STScannerRFPulse();
	
	int eventType() {return ST_EVENT_RF_PULSE;}
	double duration() {return pulseCount()*timeStep();}
	double referenceTime() {return m_reference_time;}
	
	long pulseCount();
	double pulseMagnitude(long index); //uT
	double pulsePhase(long index); //degrees
	double flipAngle();
	double timeStep(); //microsec
	
	virtual double SAR();
	virtual void setPulse(long N,double *data_mag/*uT*/,double *data_phase/*degrees*/,double time_step,double reference_flip_angle,double reference_time); 
	virtual void setFlipAngle(double flip);
	virtual void setReferenceInfo(double slice_select_amp,double slice_thickness,int pulse_type) {}
private:
	long m_pulse_count;
	double *m_pulse_magnitude;
	double *m_pulse_phase;
	double m_flip_angle;
	double m_reference_flip_angle;
	double m_time_step;
	double m_reference_time;
};

class ST_EXPORT STScannerReadout : public STScannerEvent {
public:
	STScannerReadout(STNode *parent);
	int eventType() {return ST_EVENT_READOUT;}
	double duration() {return readoutCount()*dwellTime();}
	
	long readoutCount();
	double dwellTime(); //microsec
	double referenceTime();
	int ADCIndex();
	
	bool onlineADC() {return m_online_ADC;}
	void setOnlineADC(bool val) {m_online_ADC=val;}
	
	virtual void setReadout(long N,double dwell_time,double reference_time,int ADC_index);
	void setADCIndex(int ADC_index);
	
	virtual void setCurrentLine(long val) {m_current_line=val;}
	virtual void setCurrentPartition(long val) {m_current_partition=val;}
	virtual void setCurrentAverage(long val) {m_current_average=val;}
	virtual void setCurrentSlice(long val) {m_current_slice=val;}	
	virtual void setCurrentEcho(long val) {m_current_echo=val;}
	virtual void setCurrentPhase(long val) {m_current_phase=val;}
	virtual void setCurrentRepetition(long val) {m_current_repetition=val;}
	virtual void setCurrentSet(long val) {m_current_set=val;}
	virtual void setCurrentSegment(long val) {m_current_segment=val;}
	virtual void setCurrentIda(long val) {m_current_ida=val;}
	virtual void setCurrentIdb(long val) {m_current_idb=val;}
	virtual void setCurrentIdc(long val) {m_current_idc=val;}
	virtual void setCurrentIdd(long val) {m_current_idd=val;}
	virtual void setCurrentIde(long val) {m_current_ide=val;}
	virtual void setFirstScanInSlice(bool val) {}
	virtual void setLastScanInSlice(bool val) {}
	virtual void setLastScanInConcatenation(bool val) {}
	virtual void setLastScanInMeasurement(bool val) {}
	int currentLine() {return m_current_line;}
	int currentPartition() {return m_current_partition;}
	int currentAverage() {return m_current_average;}
	int currentSlice() {return m_current_slice;}	
	int currentEcho() {return m_current_echo;}
	int currentPhase() {return m_current_phase;}
	int currentRepetition() {return m_current_repetition;}
	int currentSet() {return m_current_set;}
	int currentSegment() {return m_current_segment;}
	int currentIda() {return m_current_ida;}
	int currentIdb() {return m_current_idb;}
	int currentIdc() {return m_current_idc;}
	int currentIdd() {return m_current_idd;}
	int currentIde() {return m_current_ide;}
private:
	long m_readout_count;
	double m_dwell_time;
	double m_reference_time;
	int m_ADC_index;
	bool m_online_ADC;
	int m_current_line;
	int m_current_partition;
	int m_current_average;
	int m_current_slice;	
	int m_current_echo;
	int m_current_phase;	
	int m_current_repetition;
	int m_current_set;
	int m_current_segment;
	int m_current_ida;
	int m_current_idb;
	int m_current_idc;
	int m_current_idd;
	int m_current_ide;
};

class ST_EXPORT STScannerGradient : public STScannerEvent {
public:
	STScannerGradient(STNode *parent);
	
	int eventType() {return ST_EVENT_GRADIENT;}
	double duration() {return m_ramp_time_1+m_plateau_time+m_ramp_time_2;}
	double referenceTime() {return startTime();}
	
	int direction(); //1, 2, or 3
	double amplitude(); //mT/m, or uT/mm
	double rampTime1(); //microsec
	double plateauTime(); //microsec
	double rampTime2(); //microsec
	
	double moment() {return m_amplitude*(m_ramp_time_1/2+m_plateau_time+m_ramp_time_2);}
	
	virtual void setGradient(int direction,double amp,double ramptime1,double plateautime,double ramptime2);
private:
	int m_direction; 
	double m_amplitude;
	double m_ramp_time_1;
	double m_plateau_time;
	double m_ramp_time_2;
	
};

class ST_EXPORT STScannerArbGradient : public STScannerEvent {
public:
	STScannerArbGradient(STNode *parent);
	virtual ~STScannerArbGradient() {}
	
	int eventType() {return ST_EVENT_ARB_GRADIENT;}
	double duration() {
		if (m_amplitudes.count()==0) return 0;
		return (m_amplitudes.count()-1)*m_time_step;
	}
	double referenceTime() {return startTime();}
	
	int direction() {return m_direction;}; //1, 2, or 3
	long count() {return m_amplitudes.count();}
	double amplitude(long index) {return m_amplitudes[index];} //mT/m, or uT/mm
	double timeStep() {return m_time_step;}
	
	double moment() {
		double ret=0;
		for (long j=0; j<count()-1; j++) {
			ret+=m_amplitudes[j];
		}
		return ret*m_time_step;
	}
	
	virtual void setArbGradient(int direction,long N,double *amplitudes,double time_step);
private:
	double m_time_step;
	SList<double> m_amplitudes;
	int m_direction; 
};

struct STProtocolParameter {
	SString name;
	double value;
	double minimum;
	double maximum;
	double step;
	SString text;
};
struct STSpecialParameter {
	SString name; //should match protocol parameter
	SString ptype;
	SString label;
	SString units;
};

class ST_EXPORT STScanner {
public:
	STScanner();
	virtual ~STScanner() {}
	
	virtual STScannerRFPulse *allocateRFPulse(STNode *parent) {return new STScannerRFPulse(parent);}
	virtual STScannerReadout *allocateReadout(STNode *parent) {return new STScannerReadout(parent);}
	virtual STScannerGradient *allocateGradient(STNode *parent) {return new STScannerGradient(parent);}
	virtual STScannerArbGradient *allocateArbGradient(STNode *parent) {return new STScannerArbGradient(parent);}
	
	virtual void beginSequence();
	virtual void endSequence();
	
	virtual void beginBlock(double duration /*microsec*/);
	virtual void addEvent(STScannerEvent *event);
	virtual void endBlock(STNode *block_node);
	
	virtual bool inBlockRange() {return true;}
	
	virtual void iteratorInitialized(int iterator_index) {int dummy=iterator_index; dummy++;}
	virtual void iteratorStepped(int iterator_index) {int dummy=iterator_index; dummy++;}
	virtual void iteratorReset(int iterator_index) {int dummy=iterator_index; dummy++;}
	
	double totalSAR() {return m_total_SAR;}
	double totalDuration() {return m_total_duration;}
	long double totalGradientMomentX() {return m_total_gradient_moment_X;}
	long double totalGradientMomentY() {return m_total_gradient_moment_Y;}
	long double totalGradientMomentZ() {return m_total_gradient_moment_Z;}
	
	SString errors();
	void reportError(SString err);
	void clearErrors();
	
	double rounduptime(double t) {return ((long)(t/10+0.999999))*10;}
	double rounddowntime(double t) {return ((long)(t/10))*10;}
	
	virtual bool doneScanning() {return false;}
	
	virtual int scannerCommand(SString command_name,void *data1=0,void *data2=0,void *data3=0,void *data4=0) {return 0;}
	virtual void setReadoutIndex(int ADC_index,int iterator_index,int indx) {}

	void setValue(SString key,double val);
	void setRange(SString key,double min,double step,double max);
	void setText(SString key,SString val);
	double value(SString key);
	SString text(SString key);
	STProtocolParameter parameter(SString key);

	void addSpecialParameter(SString name,SString ptype,SString label,SString units);
	int specialParameterCount();
	STSpecialParameter specialParameter(int index);
protected:
	double m_block_duration;
	SList<STScannerEvent *> m_block_events;
	SList<STScannerEventInfo> m_event_info;
	
	double m_total_SAR;
	double m_total_duration;
	long double m_total_gradient_moment_X;
	long double m_total_gradient_moment_Y;
	long double m_total_gradient_moment_Z;
	SString m_errors;

	SList<STProtocolParameter> m_protocol_parameters;
	SList<STSpecialParameter> m_special_parameters;
};

#endif
