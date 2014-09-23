#include "stscanner.h"
#include <stdlib.h>

STScannerRFPulse::STScannerRFPulse(STNode *parent) : STScannerEvent(parent) {
	m_pulse_count=0;
	m_pulse_magnitude=0;
	m_pulse_phase=0;
	m_flip_angle=0;
	m_phase=0;
	m_frequency=0;
	m_reference_flip_angle=90;
	m_time_step=0;
	m_reference_time=0;
}

STScannerRFPulse::~STScannerRFPulse() {
	if (m_pulse_magnitude) free(m_pulse_magnitude);
	if (m_pulse_phase) free(m_pulse_phase);
}

long STScannerRFPulse::pulseCount() {
	return m_pulse_count;
}
double STScannerRFPulse::pulseMagnitude(long index) {
	return m_pulse_magnitude[index]*m_flip_angle/m_reference_flip_angle;
}
double STScannerRFPulse::pulsePhase(long index) {
	return m_pulse_phase[index];
}
double STScannerRFPulse::flipAngle() {
	return m_flip_angle;
}
double STScannerRFPulse::timeStep() {
	return m_time_step;
}

double STScannerRFPulse::SAR() {
	double ret=0;
	for (long j=0; j<m_pulse_count; j++) {
		ret+=pulseMagnitude(j)*pulseMagnitude(j)*timeStep()/1.0E6;
	}
	return ret;
}

void STScannerRFPulse::setPulse(long N,double *data_mag/*uT*/,double *data_phase/*degrees*/,double time_step,double reference_flip_angle,double reference_time) {
	if (m_pulse_magnitude) free(m_pulse_magnitude); m_pulse_magnitude=0;
	if (m_pulse_phase) free(m_pulse_phase); m_pulse_phase=0;
	m_pulse_count=N;
	m_reference_flip_angle=reference_flip_angle;
	m_reference_time=reference_time;
	m_time_step=time_step;
	if (N==0) return;
	m_pulse_magnitude=(double *)malloc(sizeof(double)*N);
	m_pulse_phase=(double *)malloc(sizeof(double)*N);
	for (long j=0; j<N; j++) {
		m_pulse_magnitude[j]=data_mag[j];
		m_pulse_phase[j]=data_phase[j];
	}
}
void STScannerRFPulse::setFlipAngle(double flip) {
	m_flip_angle=flip;
}
void STScannerEvent::setPhase(double phase /*degrees*/) {
	m_phase=normalizePhase(phase);
}
void STScannerEvent::setFrequency(double freq /*Hz*/) {
	m_frequency=freq;
}

STScannerReadout::STScannerReadout(STNode *parent) : STScannerEvent(parent) {
	m_readout_count=0;
	m_phase=0;
	m_frequency=0;
	m_dwell_time=0;
	m_reference_time=0;
	m_online_ADC=true;
	m_current_line=0;
	m_current_partition=0;
	m_current_average=0;
	m_current_slice=0;
	m_current_echo=0;
	m_current_phase=0;
	m_current_repetition=0;
	m_current_set=0;
	m_current_segment=0;
	m_current_ida=0;
	m_current_idb=0;
	m_current_idc=0;
	m_current_idd=0;
	m_current_ide=0;
}

long STScannerReadout::readoutCount() {
	return m_readout_count;
}
double STScannerEvent::phase() {
	return m_phase;
}
double STScannerEvent::frequency() {
	return m_frequency;
}
double STScannerReadout::dwellTime() {
	return m_dwell_time;
}

double STScannerReadout::referenceTime() {
	return m_reference_time;	
}

int STScannerReadout::ADCIndex() {
	return m_ADC_index;
}
	
void STScannerReadout::setReadout(long N,double dwell_time,double reference_time,int ADC_index) {
	m_readout_count=N;
	m_dwell_time=dwell_time;
	m_reference_time=reference_time;
	m_ADC_index=ADC_index;
}
void STScannerReadout::setADCIndex(int ADC_index) {
	m_ADC_index=ADC_index;
}

STScannerGradient::STScannerGradient(STNode *parent) : STScannerEvent(parent) {
	m_direction=1;
	m_amplitude=0;
	m_ramp_time_1=0;
	m_plateau_time=0;
	m_ramp_time_2=0;
}

int STScannerGradient::direction() {
	return m_direction;
}
double STScannerGradient::amplitude() {
	return m_amplitude;
}
double STScannerGradient::rampTime1() {
	return m_ramp_time_1;
}
double STScannerGradient::plateauTime() {
	return m_plateau_time;
}
double STScannerGradient::rampTime2() {
	return m_plateau_time;
}
	
void STScannerGradient::setGradient(int direction,double amp,double ramptime1,double plateautime,double ramptime2) {
	m_direction=direction;
	m_amplitude=amp;
	m_ramp_time_1=ramptime1;
	m_plateau_time=plateautime;
	m_ramp_time_2=ramptime2;
}

STScannerArbGradient::STScannerArbGradient(STNode *parent) : STScannerEvent(parent) {
	m_time_step=10;
	m_direction=1;
}

void STScannerArbGradient::setArbGradient(int direction,long N,double *amplitudes,double time_step) {
	m_time_step=time_step;
	m_direction=direction;
	m_amplitudes.clear();
	for (long j=0; j<N; j++)
		m_amplitudes << amplitudes[j];
}


STScanner::STScanner() {
	m_total_SAR=0;
	m_total_duration=0;
	m_total_gradient_moment_X=0;
	m_total_gradient_moment_Y=0;
	m_total_gradient_moment_Z=0;
}


void STScanner::beginSequence() {
	m_block_duration=0;
	m_block_events.clear();
	m_event_info.clear();
	m_total_SAR=0;
	m_total_gradient_moment_X=m_total_gradient_moment_Y=m_total_gradient_moment_Z=0;
	m_total_duration=0;
}
void STScanner::endSequence() {
}

void STScanner::beginBlock(double duration /*microsec*/) {
	m_block_duration=duration;
	m_block_events.clear();
	m_event_info.clear();
}
void STScanner::addEvent(STScannerEvent *event) {
	STScannerEventInfo info;
	info.start_time=event->startTime();
	info.phase=event->phase();
	info.frequency=event->frequency();
	m_event_info << info;
	m_block_events.append(event);
}
void STScanner::endBlock(STNode *block_node) {
	m_total_duration+=block_node->duration();
	m_total_SAR+=block_node->SAR();
	Vector3 mom=block_node->totalGradientMoment();
	m_total_gradient_moment_X+=mom.x();
	m_total_gradient_moment_Y+=mom.y();
	m_total_gradient_moment_Z+=mom.z();
}

SString STScanner::errors() {
	return m_errors;
}
void STScanner::reportError(SString err) {
	m_errors+=err+"\n";
}
void STScanner::clearErrors() {
	m_errors="";
}


int STScannerEvent::treeIndex() {
	STNode *ptr=m_parent;
	while (ptr) {
		if (ptr->treeIndex()>=0) return ptr->treeIndex();
		ptr=ptr->parent();
	}
	return -1;
}


void STScanner::setValue(SString key,double val) {
	for (int j=0; j<m_protocol_parameters.count(); j++)
		if (m_protocol_parameters[j].name==key) {
			m_protocol_parameters[j].value=val;
			return;
		}
	STProtocolParameter P;
	P.name=key;
	P.value=val;
	P.minimum=val;
	P.maximum=val;
	P.step=1;
	m_protocol_parameters.append(P);
}
double STScanner::value(SString key) {
	for (int j=0; j<m_protocol_parameters.count(); j++)
		if (m_protocol_parameters[j].name==key) {
			return m_protocol_parameters[j].value;
		}
	return 0;
}
SString STScanner::text(SString key) {
	for (int j=0; j<m_protocol_parameters.count(); j++)
		if (m_protocol_parameters[j].name==key) {
			return m_protocol_parameters[j].text;
		}
	return "";
}
void STScanner::setRange(SString key,double min,double step,double max) {
	for (int j=0; j<m_protocol_parameters.count(); j++)
		if (m_protocol_parameters[j].name==key) {
			m_protocol_parameters[j].minimum=min;
			m_protocol_parameters[j].step=step;
			m_protocol_parameters[j].maximum=max;
			return;
		}
	STProtocolParameter P;
	P.name=key;
	P.value=min;
	P.minimum=min;
	P.maximum=max;
	P.step=step;
	m_protocol_parameters.append(P);
}
void STScanner::setText(SString key,SString val) {
	for (int j=0; j<m_protocol_parameters.count(); j++)
		if (m_protocol_parameters[j].name==key) {
			m_protocol_parameters[j].text=val;
			return;
		}
	STProtocolParameter P;
	P.name=key;
	P.value=0;
	P.minimum=0;
	P.maximum=0;
	P.step=1;
	P.text=val;
	m_protocol_parameters.append(P);
}
STProtocolParameter STScanner::parameter(SString key) {
	for (int j=0; j<m_protocol_parameters.count(); j++)
		if (m_protocol_parameters[j].name==key) {
			return m_protocol_parameters[j];
		}
	STProtocolParameter P;
	P.name=key;
	P.value=0;
	P.minimum=0;
	P.maximum=0;
	P.step=1;
	P.text="";
	return P;
}

void STScanner::addSpecialParameter(SString name,SString ptype,SString label,SString units) {
	STSpecialParameter SP;
	SP.name=name;
	SP.ptype=ptype;
	SP.label=label;
	SP.units=units;
	m_special_parameters << SP;
}
int STScanner::specialParameterCount() {
	return m_special_parameters.count();
}
STSpecialParameter STScanner::specialParameter(int index) {
	if (index<m_special_parameters.count())
		return m_special_parameters[index];
	STSpecialParameter dummy;
	return dummy;
}


