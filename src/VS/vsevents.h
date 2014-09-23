#ifndef vsevent_H
#define vsevent_H

#include <QList>

#define VS_EVENT_TYPE_RF_PULSE 1
#define VS_EVENT_TYPE_READOUT 2
#define VS_EVENT_TYPE_GRADIENT 3

struct VSRFPulseEvent {
	double time_step; //us
	QList<double> data_real; //uT
	QList<double> data_imag; //uT
	QList<double> phase; //degrees
	QList<double> freq; //Hz
	QList<double> GX; //uT/mm
	QList<double> GY; //uT/mm
	QList<double> GZ; //uT/mm
};

struct VSReadoutEvent {
	double time_step; //us
	QList<double> phase; //degrees
	QList<double> freq; //Hz
	QList<double> GX; //uT/mm
	QList<double> GY; //uT/mm
	QList<double> GZ; //uT/mm
	int current_line;
	int current_partition;
	int current_average;
	int current_slice;
	int current_echo;
	int current_phase;
	int current_repetition;
	int current_set;
	int current_segment;
	int current_ida;
	int current_idb;
	int current_idc;
	int current_idd;
	int current_ide;
};

struct VSGradientEvent {
	double duration; //us
	double MX; //uT/mm-us
	double MY; //uT/mm-us
	double MZ; //uT/mm-us
};

struct VSEvent {
	int event_type;
	double start_time;
	double duration;
	VSRFPulseEvent RF;
	VSReadoutEvent ADC;
	VSGradientEvent G;
	bool onlineADC;
};

#endif
