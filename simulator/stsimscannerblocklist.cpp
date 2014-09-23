#include "stsimscannerblocklist.h"

void STSimScannerBlock::write(FILE *outf) {
	fwrite(&m_duration,1,sizeof(double),outf);
	fwrite(&m_block_number,1,sizeof(long),outf);
	fwrite(&m_elapsed_time,1,sizeof(double),outf);
	fwrite(&m_wait_for_trigger,1,sizeof(bool),outf);
	long num_events=eventCount();
	fwrite(&num_events,1,sizeof(long),outf);
	for (long j=0; j<eventCount(); j++) {
		STSimScannerEvent evt=event(j);
		write_event(outf,evt);
	}
}
void STSimScannerBlock::read(FILE *inf) {
	m_events.clear();
	fread(&m_duration,1,sizeof(double),inf);
	fread(&m_block_number,1,sizeof(long),inf);
	fread(&m_elapsed_time,1,sizeof(double),inf);
	fread(&m_wait_for_trigger,1,sizeof(bool),inf);
	long num_events=0;
	fread(&num_events,1,sizeof(long),inf);
	for (long j=0; j<num_events; j++) {
		STSimScannerEvent evt;
		read_event(inf,evt);
		m_events << evt;
	}
}

void STSimScannerBlock::write_event(FILE *outf,STSimScannerEvent &evt) {
	fwrite(&evt.block_index,1,sizeof(int),outf);
	fwrite(&evt.tree_index,1,sizeof(int),outf);
	fwrite(&evt.event_type,1,sizeof(int),outf);
	fwrite(&evt.start_time,1,sizeof(double),outf);
	fwrite(&evt.duration,1,sizeof(double),outf);
	fwrite(&evt.reference_time,1,sizeof(double),outf);
	
	//for RF and readout
	fwrite(&evt.timestep,1,sizeof(double),outf);
	fwrite(&evt.phase,1,sizeof(double),outf);
	fwrite(&evt.frequency,1,sizeof(double),outf);
	
	//for RF
	long num_data_points=evt.data_mag.count();
	fwrite(&num_data_points,1,sizeof(long),outf);
	for (long j=0; j<num_data_points; j++) {
		double mag=evt.data_mag[j];
		double ph=evt.data_phase[j];
		fwrite(&mag,1,sizeof(double),outf);
		fwrite(&ph,1,sizeof(double),outf);
	}
	
	//for readout
	fwrite(&evt.num_points,1,sizeof(long),outf);
	fwrite(&evt.onlineADC,1,sizeof(bool),outf);
	fwrite(&evt.currentLine,1,sizeof(int),outf);
	fwrite(&evt.currentPartition,1,sizeof(int),outf);
	fwrite(&evt.currentAverage,1,sizeof(int),outf);
	fwrite(&evt.currentSlice,1,sizeof(int),outf);
	fwrite(&evt.currentEcho,1,sizeof(int),outf);
	fwrite(&evt.currentPhase,1,sizeof(int),outf);
	fwrite(&evt.currentRepetition,1,sizeof(int),outf);
	fwrite(&evt.currentSet,1,sizeof(int),outf);
	fwrite(&evt.currentSegment,1,sizeof(int),outf);
	fwrite(&evt.currentIda,1,sizeof(int),outf);
	fwrite(&evt.currentIdb,1,sizeof(int),outf);
	fwrite(&evt.currentIdc,1,sizeof(int),outf);
	fwrite(&evt.currentIdd,1,sizeof(int),outf);
	fwrite(&evt.currentIde,1,sizeof(int),outf);
	
	//for gradient
	fwrite(&evt.direction,1,sizeof(int),outf);
	long num_times=evt.times.count();
	fwrite(&num_times,1,sizeof(long),outf);
	for (long j=0; j<num_times; j++) {
		double t=evt.times[j];
		double amp=evt.amplitudes[j];
		fwrite(&t,1,sizeof(double),outf);
		fwrite(&amp,1,sizeof(double),outf);
	}
	
}
void STSimScannerBlock::read_event(FILE *inf,STSimScannerEvent &evt) {
	fread(&evt.block_index,1,sizeof(int),inf);
	fread(&evt.tree_index,1,sizeof(int),inf);
	fread(&evt.event_type,1,sizeof(int),inf);
	fread(&evt.start_time,1,sizeof(double),inf);
	fread(&evt.duration,1,sizeof(double),inf);
	fread(&evt.reference_time,1,sizeof(double),inf);
	
	//for RF and readout
	fread(&evt.timestep,1,sizeof(double),inf);
	fread(&evt.phase,1,sizeof(double),inf);
	fread(&evt.frequency,1,sizeof(double),inf);
	
	//for RF
	long num_data_points=0;
	fread(&num_data_points,1,sizeof(long),inf);
	for (long j=0; j<num_data_points; j++) {
		double mag=0;
		double ph=0;
		fread(&mag,1,sizeof(double),inf);
		fread(&ph,1,sizeof(double),inf);
		evt.data_mag << mag;
		evt.data_phase << ph;
	}
	
	//for readout
	fread(&evt.num_points,1,sizeof(long),inf);
	fread(&evt.onlineADC,1,sizeof(bool),inf);
	fread(&evt.currentLine,1,sizeof(int),inf);
	fread(&evt.currentPartition,1,sizeof(int),inf);
	fread(&evt.currentAverage,1,sizeof(int),inf);
	fread(&evt.currentSlice,1,sizeof(int),inf);
	fread(&evt.currentEcho,1,sizeof(int),inf);
	fread(&evt.currentPhase,1,sizeof(int),inf);
	fread(&evt.currentRepetition,1,sizeof(int),inf);
	fread(&evt.currentSet,1,sizeof(int),inf);
	fread(&evt.currentSegment,1,sizeof(int),inf);
	fread(&evt.currentIda,1,sizeof(int),inf);
	fread(&evt.currentIdb,1,sizeof(int),inf);
	fread(&evt.currentIdc,1,sizeof(int),inf);
	fread(&evt.currentIdd,1,sizeof(int),inf);
	fread(&evt.currentIde,1,sizeof(int),inf);
	
	//for gradient
	fread(&evt.direction,1,sizeof(int),inf);
	long num_times=0;
	fread(&num_times,1,sizeof(long),inf);
	for (long j=0; j<num_times; j++) {
		double t=0;
		double amp=0;
		fread(&t,1,sizeof(double),inf);
		fread(&amp,1,sizeof(double),inf);
		evt.times << t;
		evt.amplitudes << amp;
	}
}

STSimScannerBlockList::STSimScannerBlockList() {
	m_max_RF_amplitude=0;
}
STSimScannerBlockList::~STSimScannerBlockList() {
	deleteAndClear();
}
void STSimScannerBlockList::addBlock(STSimScannerBlock *B) {
	m_blocks.append(B);
}
void STSimScannerBlockList::clear() {
	m_blocks.clear();
	m_max_RF_amplitude=0;
}
void STSimScannerBlockList::deleteAndClear() {
	for (long j=0; j<m_blocks.count(); j++) {
		delete m_blocks[j];
	}
	clear();
}

void STSimScannerBlockList::write(SString file_name) {
	#ifdef USING_SSTRING
	FILE *outf=fopen(file_name.data(),"wb");
	#else
	FILE *outf=fopen(file_name.toAscii().data(),"wb");
	#endif
	if (!outf) return;
	fwrite(&m_max_RF_amplitude,1,sizeof(double),outf);
	long num_blocks=count();
	fwrite(&num_blocks,1,sizeof(long),outf);
	for (long j=0; j<count(); j++)
		block(j)->write(outf);
	fclose(outf);
}
void STSimScannerBlockList::read(SString file_name) {
	clear();
	#ifdef USING_SSTRING
	FILE *inf=fopen(file_name.data(),"rb");
	#else
	FILE *inf=fopen(file_name.toAscii().data(),"rb");
	#endif
	if (!inf) return;
	fread(&m_max_RF_amplitude,1,sizeof(double),inf);
	long num_blocks=0;
	fread(&num_blocks,1,sizeof(long),inf);
	for (long j=0; j<num_blocks; j++) {
		STSimScannerBlock *B=new STSimScannerBlock;
		B->read(inf);
		addBlock(B);
	}
	fclose(inf);
}


