#include "stsimscanner.h"
#include <string.h>

void STSimScanner::beginSequence() {
	STScanner::beginSequence();
	clear();
}
void STSimScanner::endSequence() {
	STScanner::endSequence();
}
void STSimScanner::beginBlock(double duration) {
	STScanner::beginBlock(duration);
}
void STSimScanner::endBlock(STNode *block_node) {
	STScanner::endBlock(block_node);
	
	
	STSimScannerBlock *B=new STSimScannerBlock;
	B->setWaitForTrigger(m_wait_for_trigger);
	m_wait_for_trigger=false;
	
	if (m_raw_template_file) {
		SList<STScannerReadout *> readout_list;
		{for (int j=0; j<m_block_events.count(); j++) {
			STScannerEvent *E=m_block_events[j];
			if (E->eventType()==ST_EVENT_READOUT) {
				readout_list.append((STScannerReadout *)m_block_events[j]);
			}
		}}
		{for (int j=0; j<readout_list.count(); j++) {
			STScannerReadout *R=readout_list[j];
			int adc_index=R->ADCIndex();
			int N=R->readoutCount();
			int readout_code=5; //5 means readout

			///////////////////
			/*char logfname[500];
			strcpy(logfname,"c:\\download\\raw_template_log.txt");
			FILE *logf=fopen(logfname,"a");
			fprintf(logf,"READOUT\t%d\t%d\n",adc_index,N);
			fclose(logf);*/
			///////////////////

			fwrite(&readout_code,sizeof(int),1,m_raw_template_file);
			fwrite(&adc_index,sizeof(int),1,m_raw_template_file);
			fwrite(&N,sizeof(int),1,m_raw_template_file);
		}}
	}
	
	B->setDuration(m_block_duration);
	{for (int j=0; j<m_block_events.count(); j++) {
		add_block_event(B,m_block_events[j],m_event_info[j]);
	}}
	if (B->maxRFAmplitude()>m_blocks.maxRFAmplitude())
		m_blocks.setMaxRFAmplitude(B->maxRFAmplitude());
	if (inBlockRange())
		addBlock(B);
	B->setBlockNumber(m_current_block_number);
	B->setElapsedTime(m_total_duration-m_block_duration);
	m_current_block_number++;
	
}

void STSimScanner::add_block_event(STSimScannerBlock *B,STScannerEvent *E,STScannerEventInfo info) {
	if (m_raw_template_file) {
		if (E->eventType()!=ST_EVENT_READOUT) return;
	}
	STSimScannerEvent evt;
	evt.block_index=B->eventCount();
	evt.tree_index=E->treeIndex();
	evt.event_type=E->eventType();
	evt.start_time=info.start_time;
	evt.duration=E->duration();
	evt.reference_time=E->referenceTime();
	if (E->eventType()==ST_EVENT_RF_PULSE) {
		STScannerRFPulse *RF=(STScannerRFPulse *)E;
		evt.timestep=RF->timeStep();
		evt.phase=info.phase;
		evt.frequency=info.frequency;
		{for (long j=0; j<RF->pulseCount(); j++) {
			evt.data_mag << RF->pulseMagnitude(j);
			evt.data_phase << RF->pulsePhase(j);
		}}
		B->addEvent(evt);
	}
	else if (E->eventType()==ST_EVENT_READOUT) {
		STScannerReadout *RO=(STScannerReadout *)E;
		evt.timestep=RO->dwellTime();
		evt.phase=info.phase;
		evt.frequency=info.frequency;
		evt.num_points=RO->readoutCount();
		evt.onlineADC=RO->onlineADC();
		evt.currentLine=RO->currentLine();
		evt.currentPartition=RO->currentPartition();
		evt.currentAverage=RO->currentAverage();
		evt.currentSlice=RO->currentSlice();
		evt.currentEcho=RO->currentEcho();		
		evt.currentPhase=RO->currentPhase();
		evt.currentRepetition=RO->currentRepetition();
		evt.currentSet=RO->currentSet();
		evt.currentSegment=RO->currentSegment();
		evt.currentIda=RO->currentIda();
		evt.currentIdb=RO->currentIdb();
		evt.currentIdc=RO->currentIdc();
		evt.currentIdd=RO->currentIdd();
		evt.currentIde=RO->currentIde();
		B->addEvent(evt);
	}
	else if (E->eventType()==ST_EVENT_GRADIENT) {
		STScannerGradient *GR=(STScannerGradient *)E;
		evt.direction=GR->direction();
		evt.times << 0;
		evt.times << GR->rampTime1();
		evt.times << GR->rampTime1()+GR->plateauTime();
		evt.times << GR->duration();
		evt.amplitudes << 0;
		evt.amplitudes << GR->amplitude();
		evt.amplitudes << GR->amplitude();
		evt.amplitudes << 0;
		B->addEvent(evt);
	}
	else if (E->eventType()==ST_EVENT_ARB_GRADIENT) {
		STScannerArbGradient *GR=(STScannerArbGradient *)E;
		evt.direction=GR->direction();
		{for (long j=0; j<GR->count(); j++) {
			evt.times << GR->timeStep()*j;
			evt.amplitudes << GR->amplitude(j);
		}}
		B->addEvent(evt);
	}
}

void STSimScanner::iteratorInitialized(int iterator_index) {
	if (!m_raw_template_file) return;
	int code=10; //10 means initialize
	fwrite(&code,sizeof(int),1,m_raw_template_file);
	fwrite(&iterator_index,sizeof(int),1,m_raw_template_file);
}
void STSimScanner::iteratorStepped(int iterator_index) {
	if (!m_raw_template_file) return;
	int code=15; //10 means step
	fwrite(&code,sizeof(int),1,m_raw_template_file);
	fwrite(&iterator_index,sizeof(int),1,m_raw_template_file);
}
void STSimScanner::iteratorReset(int iterator_index) {
	if (!m_raw_template_file) return;
	int code=20; //20 means reset
	fwrite(&code,sizeof(int),1,m_raw_template_file);
	fwrite(&iterator_index,sizeof(int),1,m_raw_template_file);
}

int STSimScanner::scannerCommand(SString command_name,void *data1,void *data2,void *data3,void *data4) {
	if (command_name=="waitForTrigger") {
		m_wait_for_trigger=true;
	}
	else if (command_name=="setMDHOnline") {
		STScannerReadout *RO=(STScannerReadout *)data1;
		RO->setOnlineADC(true);
	}
	return 0;
}

void STSimScanner::setReadoutIndex(int ADC_index,int iterator_index,int indx) {
	if (!m_raw_template_file) return;
	int code=25; //25 means set readout index
	int num_bytes=sizeof(int)*3;
	fwrite(&code,sizeof(int),1,m_raw_template_file);
	fwrite(&num_bytes,sizeof(int),1,m_raw_template_file); //number of bytes to follow
	fwrite(&ADC_index,sizeof(int),1,m_raw_template_file);
	fwrite(&iterator_index,sizeof(int),1,m_raw_template_file);
	fwrite(&indx,sizeof(int),1,m_raw_template_file);
}
