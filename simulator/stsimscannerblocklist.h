
#ifndef stsimscannerblocklist_H
#define stsimscannerblocklist_H

#include "slist.h"
#include "sstring.h"
#include <stdio.h>

#ifndef ST_EVENT_RF_PULSE
	#define ST_EVENT_RF_PULSE 1
	#define ST_EVENT_READOUT 2
	#define ST_EVENT_GRADIENT 3
	#define ST_EVENT_ARB_GRADIENT 4
#endif

struct STSimScannerEvent {
	int block_index; //index of event within the block
	int tree_index; //index of event's parent node within the tree
	int event_type; //ST_EVENT_RF_PULSE, ST_EVENT_READOUT, ST_EVENT_GRADIENT, or ST_EVENT_ARB_GRADIENT
	double start_time; //microsec
	double duration; //microsec
	double reference_time; //microsec, relative to start_time
	
	//for RF and readout
	double timestep;
	double phase; //degrees
	double frequency; //Hz
	
	//for RF
	SList<double> data_mag; //uT
	SList<double> data_phase; //uT
	
	//for readout
	long num_points;
	bool onlineADC;
	int currentLine;
	int currentPartition;
	int currentAverage;
	int currentSlice;
	int currentEcho;	
	int currentPhase;
	int currentRepetition;
	int currentSet;
	int currentSegment;
	int currentIda;
	int currentIdb;
	int currentIdc;
	int currentIdd;
	int currentIde;
	
	//for gradient
	int direction; //1, 2, or 3
	SList<double> times; //microsec, starting with 0
	SList<double> amplitudes; //mT/m
};

class ST_EXPORT STSimScannerBlock {
public:
	STSimScannerBlock() {
		m_duration=0;
		m_max_RF_amplitude=0;
		m_block_number=0;
		m_elapsed_time=0;
		m_wait_for_trigger=false;
	}
	int eventCount() {return m_events.count();}
	STSimScannerEvent event(int index) {return m_events[index];}
	void addEvent(STSimScannerEvent event) {
		if (event.event_type==ST_EVENT_RF_PULSE) {
			for (long j=0; j<event.data_mag.count(); j++)
				if (event.data_mag[j]>m_max_RF_amplitude)
					m_max_RF_amplitude=event.data_mag[j];
		}
		m_events.append(event);
	}
	double duration() {return m_duration;}
	void setDuration(double val) {m_duration=val;}
	long blockNumber() {return m_block_number;}
	void setBlockNumber(long val) {m_block_number=val;}
	double elapsedTime() {return m_elapsed_time;}
	void setElapsedTime(double val) {m_elapsed_time=val;}
	void write(FILE *outf);
	void read(FILE *inf);
	double maxRFAmplitude() {return m_max_RF_amplitude;}
	void setWaitForTrigger(bool val) {m_wait_for_trigger=val;}
	bool waitForTrigger() {return m_wait_for_trigger;}
private:
	SList<STSimScannerEvent> m_events;
	double m_duration;
	double m_elapsed_time;
	double m_max_RF_amplitude;
	long m_block_number;
	bool m_wait_for_trigger;
	
	void write_event(FILE *outf,STSimScannerEvent &evt);
	void read_event(FILE *inf,STSimScannerEvent &evt);
};


class ST_EXPORT STSimScannerBlockList {
public:
	STSimScannerBlockList();
	virtual ~STSimScannerBlockList();
	void addBlock(STSimScannerBlock *B);
	long count() {return m_blocks.count();}
	STSimScannerBlock *block(long index) {return m_blocks[index];}
	void clear();
	void deleteAndClear();
	void write(SString file_name);
	void read(SString file_name);
	double maxRFAmplitude() {return m_max_RF_amplitude;}
	void setMaxRFAmplitude(double val) {m_max_RF_amplitude=val;}
private:
	SList<STSimScannerBlock *> m_blocks;
	double m_max_RF_amplitude;
};

#endif
