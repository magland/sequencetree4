#ifndef run_virtual_scan_H
#define run_virtual_scan_H

#include "vsabstractscanner.h"
#include "stsimscannerblocklist.h"

struct LoopIndex {
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
};

struct RealtimeOutputReadout {
	QList<double> data_re;
	QList<double> data_im;
	LoopIndex loop_index;
};

class RealtimeOutputHandler {
public:
	RealtimeOutputHandler();
	virtual ~RealtimeOutputHandler();
	
	void setOutputDir(QString val) {m_output_dir=val;}
	void setNumReadoutsPerPacket(int val) {m_num_readouts_per_packet=val;}
	void setSequenceName(QString val) {m_sequence_name=val;}
	void addReadout(const QList<double> &data_re,const QList<double> &data_im,const LoopIndex &loop_index);
	void initialize();
	void finalize();
private:
	QString m_output_dir;
	int m_num_readouts_per_packet;
	QString m_sequence_name;
	
	int m_scan_number;
	int m_current_packet_num;
	QList<RealtimeOutputReadout> m_pending_readouts;
	
	void write_packet();
};

bool run_virtual_scan(STSimScannerBlockList &Blocks, VSAbstractScanner *S, QString output_fname, QString realtime_output_dirname,QString sequence_name);

#endif
