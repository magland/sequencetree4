#include "run_virtual_scan.h"
#include <QList>
#include <QDebug>
#include <math.h>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QSettings>

#ifndef PI
	#define PI 3.14159265358979323846
#endif


RealtimeOutputHandler::RealtimeOutputHandler() {
}
RealtimeOutputHandler::~RealtimeOutputHandler() {
}
void RealtimeOutputHandler::addReadout(const QList<double> &data_re,const QList<double> &data_im,const LoopIndex &loop_index) {
	RealtimeOutputReadout RO;
	RO.data_re=data_re;
	RO.data_im=data_im;
	RO.loop_index=loop_index;
	m_pending_readouts << RO;
	if (m_pending_readouts.count()>=m_num_readouts_per_packet) write_packet();
}
void RealtimeOutputHandler::initialize() {
	QSettings settings("Magland","VirtualScanner");
	m_scan_number=settings.value("last_scan_number",0).toInt()+1;
	settings.setValue("last_scan_number",m_scan_number);
	m_current_packet_num=1;
	m_pending_readouts.clear();
}
void RealtimeOutputHandler::finalize() {
	if (m_pending_readouts.count()>0) write_packet();
}
void write_readout_header(FILE *outf,LoopIndex &loop_index,int column_count) {
	
	unsigned char dummy8=0;
	for (int j=0; 	j<28; j++) fwrite(&dummy8,1,1,outf);; //skip 28 bytes
	quint16 num_columns=column_count; fwrite(&num_columns,sizeof(quint16),1,outf); //2 bytes
	quint16 num_channels=1; fwrite(&num_channels,sizeof(quint16),1,outf); //2 bytes
	quint16 line=loop_index.currentLine; fwrite(&line,sizeof(quint16),1,outf); //2 bytes
	quint16 average=loop_index.currentAverage; fwrite(&average,sizeof(quint16),1,outf); //2 bytes
	quint16 slice=loop_index.currentSlice; fwrite(&slice,sizeof(quint16),1,outf); //2 bytes
	quint16 partition=loop_index.currentPartition; fwrite(&partition,sizeof(quint16),1,outf); //2 bytes
	quint16 echo=loop_index.currentEcho; fwrite(&echo,sizeof(quint16),1,outf); //2 bytes
	quint16 phase=loop_index.currentPhase; fwrite(&phase,sizeof(quint16),1,outf); //2 bytes
	quint16 repetition=loop_index.currentRepetition; fwrite(&repetition,sizeof(quint16),1,outf); //2 bytes
	quint16 set=loop_index.currentSet; fwrite(&set,sizeof(quint16),1,outf); //2 bytes
	quint16 segment=loop_index.currentSegment; fwrite(&segment,sizeof(quint16),1,outf); //2 bytes
	quint16 ida=loop_index.currentIda; fwrite(&ida,sizeof(quint16),1,outf); //2 bytes
	quint16 idb=loop_index.currentIdb; fwrite(&idb,sizeof(quint16),1,outf); //2 bytes
	quint16 idc=loop_index.currentIdc; fwrite(&idc,sizeof(quint16),1,outf); //2 bytes
	quint16 idd=loop_index.currentIdd; fwrite(&idd,sizeof(quint16),1,outf); //2 bytes
	quint16 ide=loop_index.currentIde; fwrite(&ide,sizeof(quint16),1,outf); //2 bytes
	for (int j=0; 	j<20; j++) fwrite(&dummy8,1,1,outf); //skip 20 bytes
	for (int j=0; 	j<8; j++) fwrite(&dummy8,1,1,outf); //skip 8 bytes
	for (int j=0; 	j<8; j++) fwrite(&dummy8,1,1,outf); //skip 8 bytes
	for (int j=0; 	j<28; j++) fwrite(&dummy8,1,1,outf); //skip 28 bytes
	quint16 channel=0; fwrite(&channel,sizeof(quint16),1,outf); //2 bytes
	for (int j=0; 	j<2; j++) fwrite(&dummy8,1,1,outf); //skip 2 bytes	
	
	//unsigned char holddata[128];
	//for (int j=0; j<128; j++)
	//	holddata[j]=0;
	//holddata[120]=243;
	//holddata[121]=4;
	//holddata[122]=53;
	//holddata[123]=63;
	//holddata[30]=1;
	//fwrite(holddata,sizeof(unsigned char),128,outf);	
}
void RealtimeOutputHandler::write_packet() {
	QString scan_string=QString::number(m_scan_number);
	while (scan_string.count()<6) scan_string="0"+scan_string;
	QString packet_string=QString::number(m_current_packet_num);
	while (packet_string.count()<6) packet_string="0"+packet_string;
	QString fname=QString("%1/001_%2_%3.dcm").arg(m_output_dir).arg(scan_string).arg(packet_string);
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	FILE *outf=fopen(fname.toAscii().data(),"wb");
	
	QString header_txt=QString("tProtocolName \"%1\"\n\n").arg(m_sequence_name);
	char header_txt_2[500];
	strcpy(header_txt_2,header_txt.toAscii().data());
	fwrite(header_txt_2,sizeof(char),header_txt.count(),outf);
	for (int j=0; j<1000; j++) {
		unsigned char dummy=1;
		fwrite(&dummy,sizeof(unsigned char),1,outf);
	}
	strcpy(header_txt_2,"SIEMENS RAW DATA            ");
	fwrite(header_txt_2,sizeof(char),strlen(header_txt_2),outf);
		
	for (int j=0; j<m_pending_readouts.count(); j++) {
		write_readout_header(outf,m_pending_readouts[j].loop_index,m_pending_readouts[j].data_re.count());	
		for (long k=0; k<m_pending_readouts[j].data_re.count(); k++) {			
			float re=m_pending_readouts[j].data_re[k];
			float im=m_pending_readouts[j].data_im[k];
			fwrite(&re,sizeof(float),1,outf);
			fwrite(&im,sizeof(float),1,outf);
		}
	}
	
	fclose(outf);
	m_pending_readouts.clear();
	m_current_packet_num++;
}

void sort_vs_events(QList<VSEvent> &list) {
	//bubble sort because it's most likely already in proper order
	bool changed=true;
	while (changed) {
		changed=false;
		for (int j=0; j<list.count()-1; j++) {
			if (list[j+1].start_time<list[j].start_time) {
				changed=true;
				VSEvent hold=list[j+1];
				list[j+1]=list[j];
				list[j]=hold;
				changed=true;
			}
		}
	}
}

double compute_moment(STSimScannerEvent *G,double t1,double t2,int direc) {
	t1-=G->start_time;
	t2-=G->start_time;
	if (G->direction!=direc) return 0;
	double ret=0;
	if (t2<0) return 0;
	if (t1>G->duration) return 0;
	for (long j=0; j<G->amplitudes.count()-1; j++) {
		double t=G->times[j];
		double next_t=G->times[j+1];
		double step_t=next_t-t;
		if (step_t>0) {
			if ((t1<=t)&&(next_t<=t2)) {
				ret+=(G->amplitudes[j]+G->amplitudes[j+1])/2*step_t;
			}
			else if ((t1<=t)&&(t<=t2)) {
				double amp2=G->amplitudes[j]+(G->amplitudes[j+1]-G->amplitudes[j])*(t2-t)/step_t;
				ret+=(G->amplitudes[j]+amp2)/2*(t2-t);
			}
			else if ((t1<=next_t)&&(next_t<=t2)) {
				double amp1=G->amplitudes[j+1]+(G->amplitudes[j]-G->amplitudes[j+1])*(next_t-t1)/step_t;
				ret+=(amp1+G->amplitudes[j+1])/2*(next_t-t1);
			}
			else if ((t<=t1)&&(t2<=next_t)) {
				double amp1=G->amplitudes[j+1]+(G->amplitudes[j]-G->amplitudes[j+1])*(next_t-t1)/step_t;
				double amp2=G->amplitudes[j]+(G->amplitudes[j+1]-G->amplitudes[j])*(t2-t)/step_t;
				ret+=(amp1+amp2)/2*(t2-t1);
			}
		}
	}
	return ret;
};

double compute_amplitude(STSimScannerEvent *G,double t,int direc) {
	if (G->direction!=direc) return 0;
	//!!!!!!!!!!!! fix --- this will be inefficient for arb gradient waveforms !!!!!!!!!!!!!!!!!!!
	t-=G->start_time;
	if (t<0) return 0;
	if (G->duration<t) return 0;
	if (G->times.count()==0) return 0;
	long index=0;
	double holdt=G->times[index];;
	while ((holdt<t)&&(index<G->times.count())) {
		index++;
		holdt=G->times[index];		
	}
	if (index==0) return 0;
	if (G->times[index-1]==G->times[index]) return (G->amplitudes[index-1]+G->amplitudes[index])/2;
	return G->amplitudes[index-1]+(G->amplitudes[index]-G->amplitudes[index-1])*(t-G->times[index-1])/(G->times[index]-G->times[index-1]);
}


void add_gradient_event_2(VSEvent &vsevent, STSimScannerEvent *G, double start_time) {
	if ((start_time<vsevent.start_time)||(start_time>vsevent.start_time+vsevent.duration)) {
		qWarning() << "Unexected problem." << __FILE__ << __LINE__;
		return;
	}
	if (vsevent.event_type==VS_EVENT_TYPE_GRADIENT) {
		vsevent.G.MX+=compute_moment(G,start_time,vsevent.start_time+vsevent.duration,1);
		vsevent.G.MY+=compute_moment(G,start_time,vsevent.start_time+vsevent.duration,2);
		vsevent.G.MZ+=compute_moment(G,start_time,vsevent.start_time+vsevent.duration,3);
	}
	else if (vsevent.event_type==VS_EVENT_TYPE_RF_PULSE) {
		long N=vsevent.RF.GX.count();
		for (long j=0; j<N; j++) {
			double t=vsevent.start_time+j*vsevent.RF.time_step;
			vsevent.RF.GX[j]+=compute_amplitude(G,t,1);
			vsevent.RF.GY[j]+=compute_amplitude(G,t,2);
			vsevent.RF.GZ[j]+=compute_amplitude(G,t,3);
		}
	}
	else if (vsevent.event_type==VS_EVENT_TYPE_READOUT) {
		long N=vsevent.ADC.GX.count();
		for (long j=0; j<N; j++) {
			double t=vsevent.start_time+j*vsevent.ADC.time_step;
			vsevent.ADC.GX[j]+=compute_amplitude(G,t,1);
			vsevent.ADC.GY[j]+=compute_amplitude(G,t,2);
			vsevent.ADC.GZ[j]+=compute_amplitude(G,t,3);
		}
	}
}

void add_gradient_event(QList<VSEvent> &vsevents, STSimScannerEvent *G, double start_time, int start_index=0) {
	if (start_time>=G->start_time+G->duration) return;
	int index=start_index;
	while (index<vsevents.count()) {
		if (vsevents[index].start_time>start_time) return;
		if (vsevents[index].start_time+vsevents[index].duration>start_time) {
			add_gradient_event_2(vsevents[index],G,start_time);
			add_gradient_event(vsevents,G,vsevents[index].start_time+vsevents[index].duration,index+1);
			return;
		}
		else index++;
	}
}

bool run_virtual_scan(STSimScannerBlockList &Blocks, VSAbstractScanner *S, QString output_fname, QString realtime_output_dirname,QString sequence_name) {
	RealtimeOutputHandler ROH;
	ROH.setNumReadoutsPerPacket(50);
	ROH.setOutputDir(realtime_output_dirname);
	ROH.setSequenceName(sequence_name);
	ROH.initialize();
	#ifdef WIN32
	output_fname.replace("/","\\");
	#endif
	FILE *outf=fopen(output_fname.toAscii().data(),"wb");
	if (!outf) return false;
	for (int ct=0; ct<32; ct++) {
		quint8 dummy=0;
		fwrite(&dummy,sizeof(quint8),1,outf);
	}
	S->initializeScan();
	QTime time;
	time.start();
	for (long j=0; j<Blocks.count(); j++) {	
		if (time.elapsed()>500) { //500 ms
			if (S->scanCancelled()) {
				fclose(outf);
				S->finalizeScan();
				return false;
			}
			S->setProgress(j,Blocks.count(),"Running virtual scan..."+S->statusString());
			time.start();
		}
		STSimScannerBlock *B=Blocks.block(j);
		QList<VSEvent> vsevents;
		//first collect only the RF pulse and readout events
		for (int k=0; k<B->eventCount(); k++) {
			STSimScannerEvent E=B->event(k);
			if (E.event_type==ST_EVENT_RF_PULSE) {
				VSEvent evt;
				evt.event_type=VS_EVENT_TYPE_RF_PULSE;
				evt.start_time=E.start_time;
				evt.duration=E.duration;
				evt.RF.time_step=E.timestep;
				for (long l=0; l<E.data_mag.count(); l++) {
					evt.RF.data_real << E.data_mag[l]*cos(E.data_phase[l]*PI/180);
					evt.RF.data_imag << E.data_mag[l]*sin(E.data_phase[l]*PI/180);
					evt.RF.freq << E.frequency;
					evt.RF.phase << E.phase + E.frequency*l*E.timestep/1.0E6*360;
					evt.RF.GX << 0;
					evt.RF.GY << 0;
					evt.RF.GZ << 0;
				}
				vsevents << evt;
			}
			else if (E.event_type==ST_EVENT_READOUT) {
				VSEvent evt;
				evt.event_type=VS_EVENT_TYPE_READOUT;
				evt.start_time=E.start_time;
				evt.duration=E.duration;
				evt.ADC.time_step=E.timestep;
				evt.ADC.current_line=E.currentLine;
				evt.ADC.current_partition=E.currentPartition;
				evt.ADC.current_average=E.currentAverage;
				evt.ADC.current_slice=E.currentSlice;
				evt.ADC.current_echo=E.currentEcho;
				evt.ADC.current_phase=E.currentPhase;
				evt.ADC.current_repetition=E.currentRepetition;
				evt.ADC.current_set=E.currentSet;
				evt.ADC.current_segment=E.currentSegment;
				evt.ADC.current_ida=E.currentIda;
				evt.ADC.current_idb=E.currentIdb;
				evt.ADC.current_idc=E.currentIdc;
				evt.ADC.current_idd=E.currentIdd;
				evt.ADC.current_ide=E.currentIde;
				evt.onlineADC=E.onlineADC;
				for (long l=0; l<E.num_points; l++) {
					evt.ADC.freq << E.frequency;
					evt.ADC.phase << E.phase + E.frequency*l*E.timestep/1.0E6*360;
					evt.ADC.GX << 0;
					evt.ADC.GY << 0;
					evt.ADC.GZ << 0;
				}
				vsevents << evt;
			}
		}
		sort_vs_events(vsevents);
		double last_time=0;
		//fill in-between time by gradient events
		for (int k=0; k<vsevents.count(); k++) {
			if (vsevents[k].start_time>last_time) {
				VSEvent GE;
				GE.event_type=VS_EVENT_TYPE_GRADIENT;
				GE.start_time=last_time;
				GE.duration=vsevents[k].start_time-last_time;
				GE.G.duration=GE.duration;
				GE.G.MX=0;
				GE.G.MY=0;
				GE.G.MZ=0;
				last_time=vsevents[k].start_time+vsevents[k].duration;
				vsevents.insert(k,GE);						
				k++;
			}
			else {
				last_time=vsevents[k].start_time+vsevents[k].duration;
			}
		}
		//append a gradient event to fill in remainder of time
		if (B->duration()>last_time) {
			VSEvent GE;
			GE.event_type=VS_EVENT_TYPE_GRADIENT;
			GE.start_time=last_time;
			GE.duration=B->duration()-last_time;
			GE.G.duration=GE.duration;
			GE.G.MX=0;
			GE.G.MY=0;
			GE.G.MZ=0;
			vsevents.append(GE);
		}
		for (int k=0; k<B->eventCount(); k++) {
			STSimScannerEvent E=B->event(k);
			if ((E.event_type==ST_EVENT_GRADIENT)||(E.event_type==ST_EVENT_ARB_GRADIENT)) {
				add_gradient_event(vsevents,&E,E.start_time);
			}
		}
		for (int k=0; k<vsevents.count(); k++) {
			VSEvent E=vsevents[k];
			if (E.event_type==VS_EVENT_TYPE_GRADIENT) {
				S->processGradient(E.G);
			}
			else if (E.event_type==VS_EVENT_TYPE_RF_PULSE) {
				S->processRFPulse(E.RF);
			}
			else if (E.event_type==VS_EVENT_TYPE_READOUT) {
				QList<double> data_real;
				QList<double> data_imag;
				S->processReadout(E.ADC,data_real,data_imag);
				for (int ct=0; ct<128; ct++) {
					quint8 dummy=0;
					fwrite(&dummy,sizeof(quint8),1,outf);
				}
				for (long ct=0; ct<data_real.count(); ct++) {
					float hold_re=(float)data_real[ct];
					float hold_im=(float)data_imag[ct];
					fwrite(&hold_re,4,1,outf);
					fwrite(&hold_im,4,1,outf);
				}
				if (E.onlineADC) {
					if (!realtime_output_dirname.isEmpty()) {
						LoopIndex LI;
						LI.currentLine=E.ADC.current_line;
						LI.currentPartition=E.ADC.current_partition;
						LI.currentAverage=E.ADC.current_average;
						LI.currentSlice=E.ADC.current_slice;
						LI.currentEcho=E.ADC.current_echo;
						LI.currentPhase=E.ADC.current_phase;
						LI.currentRepetition=E.ADC.current_repetition;
						LI.currentSet=E.ADC.current_set;
						LI.currentSegment=E.ADC.current_segment;
						LI.currentIda=E.ADC.current_ida;
						LI.currentIdb=E.ADC.current_idb;
						LI.currentIdc=E.ADC.current_idc;
						LI.currentIdd=E.ADC.current_idd;
						LI.currentIde=E.ADC.current_ide;						
						ROH.addReadout(data_real,data_imag,LI);
					}
				}
			}
		}
		S->addElapsedTime(B->duration());		
	}
	fclose(outf);
	S->finalizeScan();
	ROH.finalize();
	return true;
}
