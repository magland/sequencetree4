////////////////////////////////////////////////////////////////
///// Copyright 2005 by Jeremy Magland
///// This code is confidential
////////////////////////////////////////////////////////////////

#ifndef STSIM_H
#define STSIM_H

#include <stdlib.h>
#include <stdio.h>
#include <QList>

struct simulation_sequence_RF_event {
	double time_step; //microseconds
	double center_time;
	QList<double> data_re; //microTesla
	QList<double> data_im;
	QList<double> gradientampX; //uT/mm
	QList<double> gradientampY; //uT/mm
	QList<double> gradientampZ; //uT/mm
};

void write_RF(simulation_sequence_RF_event &RF,FILE *outf) {
	long num_points=RF.data_re.count();
	fwrite(&num_points,sizeof(long),1,outf);
	fwrite(&RF.time_step,sizeof(double),1,outf);
	fwrite(&RF.center_time,sizeof(double),1,outf);
	for (long j=0; j<num_points; j++) {
		double re=RF.data_re[j];
		double im=RF.data_im[j];
		double gx=RF.gradientampX[j];
		double gy=RF.gradientampX[j];
		double gz=RF.gradientampX[j];
		fwrite(&re,sizeof(double),1,outf);
		fwrite(&im,sizeof(double),1,outf);
		fwrite(&gx,sizeof(double),1,outf);
		fwrite(&gy,sizeof(double),1,outf);
		fwrite(&gz,sizeof(double),1,outf);
	}
}
void read_RF(simulation_sequence_RF_event &RF,FILE *inf) {
	long num_points;
	fwrite(&num_points,sizeof(long),1,inf);
	fread(&RF.time_step,sizeof(double),1,inf);
	fread(&RF.center_time,sizeof(double),1,inf);
	RF.data_re.clear();
	RF.data_im.clear();
	RF.gradientampX.clear();
	RF.gradientampY.clear();
	RF.gradientampZ.clear();
	for (long j=0; j<num_points; j++) {
		double re,im,gx,gy,gz;
		fread(&re,sizeof(double),1,inf);
		fread(&im,sizeof(double),1,inf);
		fread(&gx,sizeof(double),1,inf);
		fread(&gy,sizeof(double),1,inf);
		fread(&gz,sizeof(double),1,inf);
		RF.data_re << re;
		RF.data_im << im;
		RF.gradientampX << gx;
		RF.gradientampY << gy;
		RF.gradientampZ << gz;
	}
}

struct simulation_sequence_ADC_event {
	int index;
	int num_points;
	double time_step; //microsec
	QList<double> data_phase;
	QList<double> gradientampX; //uT/mm
	QList<double> gradientampY; //uT/mm
	QList<double> gradientampZ; //uT/mm
};

void write_ADC(simulation_sequence_ADC_event &ADC,FILE *outf) {
	fwrite(&ADC.index,sizeof(int),1,outf);
	fwrite(&ADC.num_points,sizeof(int),1,outf);
	fwrite(&ADC.time_step,sizeof(double),1,outf);
	for (long j=0; j<ADC.num_points; j++) {
		double ph=ADC.data_phase[j];
		double gx=ADC.gradientampX[j];
		double gy=ADC.gradientampY[j];
		double gz=ADC.gradientampZ[j];
		fwrite(&ph,sizeof(double),1,outf);
		fwrite(&gx,sizeof(double),1,outf);
		fwrite(&gy,sizeof(double),1,outf);
		fwrite(&gz,sizeof(double),1,outf);
	}
}

class simulation_sequence_gradient_event {
public:
	float moment[3];
	float time;
	
	void write_to_file(FILE *outf) {
		fwrite(moment,sizeof(float),3,outf);
		fwrite(&time,sizeof(float),1,outf);
		
	}
	void read_from_file(FILE *inf) {
		fread(moment,sizeof(float),3,inf);
		fread(&time,sizeof(float),1,inf);	
	}
};

/*
class STsim_block {
public:
	PointerList event_list;
	float duration;
	vector3 initial_moment;
	STsim_block *next,*prev;
	STsim_block() {
		next=prev=0;
		event_list.clear();
		duration=0;
		initial_moment=v3(0,0,0);
	}
	virtual ~STsim_block() {
		clear_events();
	}
	void clear_events() {
		event_list.clear();
	}
	ST_scanner_event *get_event(long j) {
		return (ST_scanner_event *)event_list.get(j);
	}
	long num_events() {
		return event_list.num_records;
	}
	void copy_from(STsim_block &B) {
		clear_events();
		duration=B.duration;
		initial_moment=B.initial_moment;
		long j;
		for (j=0; j<B.num_events(); j++)
			add_event((ST_scanner_event *)B.get_event(j));
	}
	void add_event(ST_scanner_event *X) {
		if (!X) return;

		ST_scanner_event *ptr=0;

		if (X->scanner_event_type==SET_GRADIENT) {
			STsim_gradient *X2=new STsim_gradient;
			(*X2)=(*(STsim_gradient *)X);
			ptr=X2;
		}
		else if (X->scanner_event_type==SET_ADC) {
			STsim_ADC *X2=new STsim_ADC;
			(*X2)=(*(STsim_ADC *)X);
			ptr=X2;
		}
		else if (X->scanner_event_type==SET_RF) {
			STsim_RF *X2=new STsim_RF;
			(*X2)=(*(STsim_RF *)X);
			ptr=X2;
		}
		else if (X->scanner_event_type==SET_ARBGRADIENT) {
			STsim_arbgradient *X2=new STsim_arbgradient;
			(*X2)=(*(STsim_arbgradient *)X);
			ptr=X2;
		}

		if (!ptr) return;

		event_list.add(ptr);
	}
	void get_next_gradient_point(float t,int direction,float &ret_time,float &ret_value) {
		ret_time=-1;
		ret_value=0;
		int j;
		int num=num_events();
		for (j=0; j<num; j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_GRADIENT) {
				if ((E->end_time()>t)&&
					((ret_time<0)||(E->start_time<ret_time))) {
					ST_scanner_gradient_event *G=(ST_scanner_gradient_event *)E;
					if (G->direction==direction) {
						if ((G->start_time>t)&&((G->start_time<ret_time)||(ret_time<0))) {
							ret_time=G->start_time;
						}
						else if ((G->start_time+G->rampup>t)&&((G->start_time+G->rampup<ret_time)||(ret_time<0))) {
							ret_time=G->start_time+G->rampup;
						}
						else if ((G->start_time+G->rampup+G->plateau>t)&&((G->start_time+G->rampup+G->plateau<ret_time)||(ret_time<0))) {
							ret_time=G->start_time+G->rampup+G->plateau;
						}
						else if ((G->end_time()>t)&&((G->end_time()<ret_time)||(ret_time<0))) {
							ret_time=G->end_time();
						}
						if (direction==1)
							ret_value=get_amplitude_at(ret_time).x;
						else if (direction==2)
							ret_value=get_amplitude_at(ret_time).y;
						else if (direction==3)
							ret_value=get_amplitude_at(ret_time).z;
						else
							ret_value=0;

					}
				}
			}
		}
	}
	
	void get_next_RF_point(float t,float &ret_time,float &ret_re,float &ret_im) {
		ret_time=-1;
		ret_im=0;
		int j;
		int num=num_events();
		for (j=0; j<num; j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_RF) {
				if ((E->end_time()>t)&&((ret_time<0)||(E->start_time<ret_time))) {
					ST_scanner_RF_event *RF=(ST_scanner_RF_event *)E;
					bool found=false;
					int ind=0;
					while ((!found)&&(ind<RF->N)) {
						float t0=RF->start_time+ind*RF->time_step;
						if ((t0>t)&&((t0<ret_time)||ret_time<0)) {
							ret_time=t0; 
							float scale_factor = ((STsim_RF *)RF)->scale_factor;
							float flip_angle = ((STsim_RF *)RF)->flip_angle;
							float time_step = ((STsim_RF *)RF)->time_step;
							float uTScale = scale_factor*flip_angle/90*time_step/10;

							ret_re=RF->data_mag[ind]*(float)cos(RF->data_phase[ind]*ST_PI/180)*uTScale;                                             
							ret_im=RF->data_mag[ind]*(float)sin(RF->data_phase[ind]*ST_PI/180)*uTScale;
							found=true;
						}
						ind++;
					}
				}
			}
		}
	}
	
	void get_next_ADC_point(float t,float &ret_time,float &ret_value) {
		ret_time=-1;
		ret_value=0;
		int j;
		int num=num_events();
		for (j=0; j<num; j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_ADC) {
				if ((E->end_time()>t)&&
					((ret_time<0)||(E->start_time<ret_time))) {
					ST_scanner_ADC_event *ADC=(ST_scanner_ADC_event *)E;
					bool found=false;
					int ind=0;
					while ((!found)&&(ind<ADC->N)) {
						float t0=ADC->start_time+ind*ADC->dwell_time;
						if ((t0>t)&&((t0<ret_time)||ret_time<0)) {
							ret_time=t0;
							ret_value=1;
							if ((ind==0)||(ind==ADC->N-1))
								ret_value=0;
							found=true;
						}
						ind++;
					}
				}
			}
		}
	}
	#define CHANNEL_GX		1
	#define CHANNEL_GY		2
	#define CHANNEL_GZ		3
	#define CHANNEL_RF_REAL 4
	#define CHANNEL_RF_IMAG 5
	#define CHANNEL_ADC		6
	void write_seq_channel(FILE *outf, int channel_number,long &ct,double &T) {
		float t=0;
		double val=0;
		double t_out=T/1000;
		fwrite(&t_out,sizeof(double),1,outf);
		fwrite(&val,sizeof(double),1,outf);
		ct++;
		bool done=false;
		double best_time=-1;
		double best_value=0;
		while (!done) {
			float hold_time=-1,hold_value=0;
			if (channel_number==CHANNEL_GX) {
				get_next_gradient_point(t,1,hold_time,hold_value);
			}
			else if (channel_number==CHANNEL_GY) {
				get_next_gradient_point(t,2,hold_time,hold_value);
			}
			else if (channel_number==CHANNEL_GZ) {
				get_next_gradient_point(t,3,hold_time,hold_value);
			}
			else if (channel_number==CHANNEL_RF_REAL) {
				float re,im;
				get_next_RF_point(t,hold_time,re,im);
				hold_value=re;
			}
			else if (channel_number==CHANNEL_RF_IMAG) {
				float re,im;
				get_next_RF_point(t,hold_time,re,im);
				hold_value=im;
			}
			else if (channel_number==CHANNEL_ADC) {
				get_next_ADC_point(t,hold_time,hold_value);
			}
			if (hold_time>=0) {
				t=hold_time;
				double t0=(T+t)/1000;
				double v0=hold_value;
				fwrite(&t0,sizeof(double),1,outf);
				fwrite(&v0,sizeof(double),1,outf);
				ct++;
			}
			else done=true;
		}
		T+=duration;
		val=0;
		t_out=T/1000;
		fwrite(&t_out,sizeof(double),1,outf);
		fwrite(&val,sizeof(double),1,outf);
		ct++;
	}
	void write_to_file(FILE *outf) {
		if (!outf) return;

		fwrite(&duration,sizeof(float),1,outf);
		fwrite(&initial_moment.x,sizeof(float),1,outf);
		fwrite(&initial_moment.y,sizeof(float),1,outf);
		fwrite(&initial_moment.z,sizeof(float),1,outf);

		long j;
		for (j=0; j<num_events(); j++) {
			int hold=1;
			fwrite(&hold,sizeof(int),1,outf);
			get_event(j)->write_to_file(outf);
		}
		int hold=0;
		fwrite(&hold,sizeof(int),1,outf);
	}
	void read_from_file(FILE *inf) {
		clear_events();

		if (!inf) return;

		fread(&duration,sizeof(float),1,inf);
		fread(&initial_moment.x,sizeof(float),1,inf);
		fread(&initial_moment.y,sizeof(float),1,inf);
		fread(&initial_moment.z,sizeof(float),1,inf);

		bool done=false;
		while (!done) {
			int hold;
			if (fread(&hold,sizeof(int),1,inf)<=0)
				done=true;
			if (!done) {
				if (!hold)
					done=true;
				else {
					ST_scanner_event *E;
					E=read_event_from_file(inf);
					if (E) {
						add_event(E);
						delete E;
					}
					else 
						done=true;
				}
			}
		}
	}
	ST_scanner_event *read_event_from_file(FILE *inf) {
		if (!inf) return 0;
		
		int event_type;
		fread(&event_type,sizeof(int),1,inf);
		if (event_type==SET_RF) {
			STsim_RF *E=new STsim_RF;
			E->read_from_file(inf);
			return E;
		}
		else if (event_type==SET_ADC) {
			STsim_ADC *E=new STsim_ADC;
			E->read_from_file(inf);
			return E;
		}
		else if (event_type==SET_GRADIENT) {
			STsim_gradient *E=new STsim_gradient;
			E->read_from_file(inf);
			return E;
		}
		else if (event_type==SET_ARBGRADIENT) {
			STsim_arbgradient *E=new STsim_arbgradient;
			E->read_from_file(inf);
			return E;
		}
		return 0;
	}
	float get_maxRFamplitude() {
		float ret=0;
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_RF) {
				float hold=((STsim_RF *)E)->get_maxRFamplitude();
				if (hold>ret)
					ret=hold;
			}
		}
		return ret;
	}
	vector3 get_gradientamp_at(float t) {
		vector3 ret=v3(0,0,0);
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if ((E->start_time<=t)&&(t<=E->end_time())) {
				ret=ret+E->get_gradientamp_at(t-E->start_time);
			}
		}
		return ret;
	}
	float get_RFamp_at(float t) {
		float ret=0;
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if ((E->start_time<=t)&&(t<=E->end_time())) {
				ret=ret+E->get_RFamp_at(t-E->start_time);
			}
		}
		return ret;
	}
	double get_SAR() {
		double ret=0;
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_RF) {
				STsim_RF *ptr=(STsim_RF *)E;
				ret+=ptr->get_SAR();
			}
		}
		return ret;
	}
	STsim_RF *find_rf_event_between(float t1,float t2) {
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_RF) {
				float t=E->start_time;
				if ((t1<t)&&(t<t2))
					return (STsim_RF *)E;
			}
		}
		return 0;
	}
	STsim_ADC *find_adc_event_between(float t1,float t2) {
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_ADC) {
				float t=E->start_time;
				if ((t1<t)&&(t<t2))
					return (STsim_ADC *)E;
			}
		}
		return 0;
	}
	void write_simulation_sequence_rf(FILE *outf,STsim_RF *rf) {
		if (!outf) return;
		if (!rf) return;
		int hold=SET_RF;
		fwrite(&hold,sizeof(int),1,outf);
		simulation_sequence_RF_event E;

		E.num_points=rf->N;
		int j;
		for (j=0; j<rf->N; j++) {
			float t=rf->start_time+j*rf->time_step;
			vector3 amp=get_amplitude_at(t);
			float mag=rf->data_mag[j]*rf->scale_factor*rf->flip_angle/90*10/rf->time_step;
			
			float phase=rf->data_phase[j]+rf->phase+360*rf->time_step/1.0E6F*rf->freq*j;
			E.data_re[j]=mag*(float)cos(phase*ST_PI/180);
			E.data_im[j]=mag*(float)sin(phase*ST_PI/180);
			E.gradientampX[j]=amp.x;
			E.gradientampY[j]=amp.y;
			E.gradientampZ[j]=amp.z;
		}

		E.time_step=rf->time_step;
		E.center_time=rf->center_time;

		E.write_to_file(outf);
	}
	void write_simulation_sequence_adc(FILE *outf,STsim_ADC *adc) {
		if (!outf) return;
		if (!adc) return;
		int hold=SET_ADC;
		fwrite(&hold,sizeof(int),1,outf);
		simulation_sequence_ADC_event E;

		E.index=adc->ADC_index;
		E.num_points=adc->N;
		E.time_step=adc->dwell_time;
		int j;
		for (j=0; j<adc->N; j++) {
			float t=adc->start_time+j*adc->dwell_time;
			vector3 amp=get_amplitude_at(t);
			E.gradientampX[j]=amp.x;
			E.gradientampY[j]=amp.y;
			E.gradientampZ[j]=amp.z;
			//////fix!!!!!!!!!!!!!!!!!!!!!!!!!
			E.data_phase[j]=adc->phase+360*adc->dwell_time/1.0E6F*adc->freq*j;
		}
		

		E.write_to_file(outf);
	}
	void write_simulation_sequence_gradient(FILE *outf,vector3 moment,float t) {
		if (!outf) return;
		int hold=SET_GRADIENT;
		fwrite(&hold,sizeof(int),1,outf);
		simulation_sequence_gradient_event E;
		E.moment[0]=moment.x;
		E.moment[1]=moment.y;
		E.moment[2]=moment.z;
		E.time=t;
		E.write_to_file(outf);
	}
	vector3 get_amplitude_at(STsim_gradient *X,float t) {
		if (!X)
			return v3(0,0,0);

		float rampup=X->rampup;
		float plateau=X->plateau;
		float rampdown=X->rampdown;
		float amp=X->amplitude;
		float dur=rampup+plateau+rampdown;
		float st=X->start_time;

		if (t>=st+dur)
			return v3(0,0,0);
		if (t<=st)
			return v3(0,0,0);

		float ret=0;

		t-=st;

		if (t<=0)
			ret=0;
		else if (t<=rampup)
			ret=amp*t/rampup;
		else if (t<=dur-rampdown)
			ret=amp;
		else if (t<=dur) {
			float diff=dur-t;
			ret=amp*diff/rampdown;
		}

		if (X->direction==1)
			return v3(ret,0,0);
		else if (X->direction==2)
			return v3(0,ret,0);
		else if (X->direction==3)
			return v3(0,0,ret);

		return v3(0,0,0);
	}
	vector3 get_amplitude_at(float t) {
		vector3 ret=v3(0,0,0);
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_GRADIENT) {
				ret=ret+get_amplitude_at((STsim_gradient *)E,t);
			}
		}
		return ret;
	}
	vector3 get_moment_between(STsim_arbgradient *X,float t1,float t2) {
		if (!X) 
			return v3(0,0,0);

		long j1=(long)floor((t1-X->start_time)/X->time_step);
		long j2=(long)ceil((t2-X->start_time)/X->time_step);

		if (j1<0) 
			j1=0;
		if (j1>=X->N)
			j1=X->N-1;
		if (j2<0) 
			j2=0;
		if (j2>=X->N)
			j2=X->N-1;

		vector3 ret=v3(0,0,0);
		long j;
		for (j=j1; j<j2; j++) {
			ret=ret+v3(X->dataX[j],X->dataY[j],X->dataZ[j])*X->time_step;
			if (j==j1) {
				float t=X->start_time+j*X->time_step;
				if (t<t1) {
					ret=ret-v3(X->dataX[j],X->dataY[j],X->dataZ[j])*(t1-t);
				}
			}
			if (j==j2-1) {
				float t=X->start_time+(j+1)*X->time_step;
				if (t>t2) {
					ret=ret-v3(X->dataX[j],X->dataY[j],X->dataZ[j])*(t-t2);
				}
			}
		}
		return ret;
	}
	vector3 get_moment_between(STsim_gradient *X,float t1,float t2) {
		if (!X)
			return v3(0,0,0);

		float rampup=X->rampup;
		float plateau=X->plateau;
		float rampdown=X->rampdown;
		float amp=X->amplitude;
		float dur=rampup+plateau+rampdown;
		float st=X->start_time;

		if ((t1>=st+dur)&&(t2>=st+dur))
			return v3(0,0,0);
		if ((t1<=st)&&(t2<=st))
			return v3(0,0,0);

		float ret=0;

		t1-=st;
		t2-=st;

		if (t2<=0)
			ret+=0;
		if (t2<=rampup)
			ret+=t2/2*amp*(t2/rampup);
		else if (t2<=dur-rampdown)
			ret+=rampup/2*amp+(t2-rampup)*amp;
		else if (t2<=dur) {
			float diff=dur-t2;
			 ret+=rampup/2*amp+(dur-rampup-rampdown)*amp
				+rampdown/2*amp-diff/2*amp*(diff/rampdown);
		}
		else
			ret+=(dur-rampup/2-rampdown/2)*amp;

		if (t1<=0)
			ret-=0;
		else if (t1<=rampup)
			ret-=t1/2*amp*(t1/rampup);
		else if (t1<=dur-rampdown)
			ret-=rampup/2*amp+(t1-rampup)*amp;
		else if (t1<=dur){
			float diff=dur-t1;
			ret-=rampup/2*amp+(dur-rampup-rampdown)*amp
				+rampdown/2*amp-diff/2*amp*(diff/rampdown);
		}
		else {
			ret-=(dur-rampup/2-rampdown/2)*amp;
		}

		if (X->direction==1)
			return v3(ret,0,0);
		else if (X->direction==2)
			return v3(0,ret,0);
		else if (X->direction==3)
			return v3(0,0,ret);

		return v3(0,0,0);
	}
	vector3 get_moment_between(float t1,float t2) {
		vector3 ret=v3(0,0,0);
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if (E->scanner_event_type==SET_GRADIENT) {
				if ((E->start_time<t2)&&(E->end_time()>t1))
					ret=ret+get_moment_between((STsim_gradient *)E,t1,t2);
			}
			else if (E->scanner_event_type==SET_ARBGRADIENT) {
				if ((E->start_time<t2)&&(E->end_time()>t1))
					ret=ret+get_moment_between((STsim_arbgradient *)E,t1,t2);
			}
		}
		return ret;
	}
	void write_simulation_sequence_block(FILE *outf,float t1=-1,float t2=-1) {
		if (!outf) return;
		if ((t1<0)||(t2<0)) {
			write_simulation_sequence_block(outf,0,duration);
			return;
		}
		if (t1==t2) 
			return;
		
		STsim_RF *rf=find_rf_event_between(t1,t2);
		if (rf) {
			write_simulation_sequence_block(outf,t1,rf->start_time);
			write_simulation_sequence_rf(outf,rf);
			write_simulation_sequence_block(outf,rf->start_time+rf->duration(),t2);
			return;
		}

		STsim_ADC *adc=find_adc_event_between(t1,t2);
		if (adc) {
			write_simulation_sequence_block(outf,t1,adc->start_time);
			write_simulation_sequence_adc(outf,adc);
			write_simulation_sequence_block(outf,adc->start_time+adc->duration(),t2);
			return;
		}

		vector3 moment=get_moment_between(t1,t2);
		write_simulation_sequence_gradient(outf,moment,t2-t1);
	}
	void get_next_mom_time(vector3 &mom,float &t,bool &is_excitation_time,bool &is_refocusing_time) {
		is_excitation_time=false;
		is_refocusing_time=false;
		if (t>=duration) {
			t=t+1;
			return;
		}
		float bestt=duration;
		ST_scanner_event *best_event=0;
		long j;
		for (j=0; j<num_events(); j++) {
			ST_scanner_event *E=get_event(j);
			if ((E->start_time<=bestt)&&(E->end_time()>t)) {
				if (E->scanner_event_type==SET_GRADIENT) {
					STsim_gradient *E2=(STsim_gradient *)E;
					if (E2->start_time>t) {
						if (E->start_time<bestt) {
							bestt=E->start_time;
							best_event=E;
						}
					}
					else if (E2->start_time+E2->rampup>t) {
						if (E2->start_time+E2->rampup<bestt) {
							bestt=E2->start_time+E2->rampup;
							best_event=E;
						}
					}
					else if (E2->start_time+E2->rampup+E2->plateau>t) {
						if (E2->start_time+E2->rampup+E2->plateau<bestt) {
							bestt=E2->start_time+E2->rampup+E2->plateau;
							best_event=E;
						}
					}
					else if (E2->end_time()>t) {
						if (E2->end_time()<bestt) {
							bestt=E2->end_time();
							best_event=E;
						}
					}
				}
				else if (E->scanner_event_type==SET_RF) {
					STsim_RF *E2=(STsim_RF *)E;
					if (E2->start_time+E2->center_time>t) {
						if (E2->start_time+E2->center_time<=bestt) {
							bestt=E2->start_time+E2->center_time;
							best_event=E;
						}
					}
				}
				else if (E->scanner_event_type==SET_ARBGRADIENT) {
					STsim_arbgradient *E2=(STsim_arbgradient *)E;
					float holdt=E2->start_time;
					long j=(long)floor((t-E2->start_time)/E2->time_step)+1;
					if (j<0) 
						j=0;
					if ((holdt<bestt)&&(j<E2->N)) {
						bestt=holdt+j*E2->time_step;
						best_event=E;
					}
				}
			}
		}
		if ((!best_event)||(bestt>=duration)) {
			t=duration;
			return;
		}
		mom=mom+get_moment_between(t,bestt);
		t=bestt;
		if (best_event->scanner_event_type==SET_RF) {
			STsim_RF *X=(STsim_RF *)best_event;
			if (X->pulse_type==1) {
				is_excitation_time=true;
			}
			else if (X->pulse_type==2) {
				is_refocusing_time=true;
			}
		}
	}
	void get_moment_vectors(float *times, float *ROmom,float *PEmom,float *SELmom,long &N) {
		long j=0;
		float t=0;
		bool is_excitation_time=false;
		bool is_refocusing_time=false;
		vector3 cur=initial_moment;
		bool done=false;
		while ((t<=duration)&&(!done)) {
			if (j<MAX_MOMENT_POINTS-5) {
				times[j]=t;
				ROmom[j]=cur.x;
				PEmom[j]=cur.y;
				SELmom[j]=cur.z;
				j++;
				if (is_excitation_time) {
					cur=v3(0,0,0);
					times[j]=t;
					ROmom[j]=0;
					PEmom[j]=0;
					SELmom[j]=0;
					j++;
				}
				else if (is_refocusing_time) {
					cur=cur*(-1);
					times[j]=t;
					ROmom[j]=cur.x;
					PEmom[j]=cur.y;
					SELmom[j]=cur.z;
					j++;
				}
				get_next_mom_time(cur,t,is_excitation_time,is_refocusing_time);
			}
			else {
				done=true;
			}
			
		}
		N=j;
	}
};

class STsim_block_list {
public:
	STsim_block *first_block;
	STsim_block *last_block;
	STsim_block *current_block;
	int current_block_index;
	long num_blocks;
	long first_block_number;
	float maxRFamplitude;

	STsim_block_list() {
		first_block_number=0;
		first_block=last_block=0;
		num_blocks=0;
		current_block=0;
		current_block_index=0;
		maxRFamplitude=0;
	}
	~STsim_block_list() {
		clear();
	}
	void clear() {
		STsim_block *B=first_block;
		while (B) {
			STsim_block *n=B->next;
			delete B;
			B=n;
		}
		first_block_number=0;
		first_block=last_block=0;
		num_blocks=0;
		current_block_index=0;
		current_block=0;
	}
	STsim_block *add_block() {
		STsim_block *X=new STsim_block;
		if (!first_block) {
			first_block=last_block=X;
			num_blocks=1;
		}
		else {
			last_block->next=X;
			X->prev=last_block;
			num_blocks++;
			last_block=X;
		}
		return X;
	}
	STsim_block *get_block(long num) {
		if (!current_block) {
			current_block=first_block;
			current_block_index=0;
		}
		while ((current_block)&&(current_block_index<num)) {
			current_block=current_block->next;
			current_block_index++;
		}
		while ((current_block)&&(current_block_index>num)) {
			current_block=current_block->prev;
			current_block_index--;
		}
		return current_block;
	}
	bool write_to_file(char *fname) {
		FILE *outf=fopen(fname,"wb");
		if (!outf) return false;

		fwrite(&first_block_number,sizeof(long),1,outf);
		fwrite(&maxRFamplitude,sizeof(float),1,outf);

		STsim_block *B=first_block;
		while (B) {
			int hold=1;
			fwrite(&hold,sizeof(int),1,outf);
			B->write_to_file(outf);
			B=B->next;
		}
		int hold=0;
		fwrite(&hold,sizeof(int),1,outf);

		fclose(outf);

		return true;
	}
	#define CHANNEL_GX		1
	#define CHANNEL_GY		2
	#define CHANNEL_GZ		3
	#define CHANNEL_RF_REAL 4
	#define CHANNEL_RF_IMAG	5
	#define CHANNEL_ADC		6
	void write_seq_channel(FILE *outf, int channel_number) {
		if (!outf) return;
		long pos=ftell(outf);
		long ct=0;
		double T=0;
		fwrite(&ct,sizeof(long),1,outf);

		STsim_block *B=first_block;
		while (B) {
			B->write_seq_channel(outf,channel_number,ct,T);
			B=B->next;
		}

		long holdpos=ftell(outf);
		fseek(outf,pos,SEEK_SET);
		fwrite(&ct,sizeof(long),1,outf);
		fseek(outf,holdpos,SEEK_SET);
	}
	bool read_from_file(char *fname) {
		clear();
		
		FILE *inf=fopen(fname,"rb");
		if (!inf) {
			printf("Cannot open file for reading: %s\n",fname);
			return false;
		}

		fread(&first_block_number,sizeof(long),1,inf);
		fread(&maxRFamplitude,sizeof(float),1,inf);

		bool done=false;
		while (!done) {
			int hold;
			if (fread(&hold,sizeof(int),1,inf)<=0)
				return false;
			if (!hold)
				done=true;
			else {
				STsim_block *B=add_block();
				B->read_from_file(inf);
			}
		}

		fclose(inf);

		return true;
	}
};





class STsim_scanner : public ST_scanner {
public:

	STsim_block_list Blocks;
	bool write_simulation_sequence_mode;
	FILE *simulation_sequence_file;
	char simulation_sequence_fname[1000];


	STsim_scanner() {
		write_simulation_sequence_mode=false;
		strcpy(simulation_sequence_fname,"");
		simulation_sequence_file=0;
	}
	virtual ~STsim_scanner() {
		if (simulation_sequence_file)
			fclose(simulation_sequence_file);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//Scanner commands
	virtual void begin_sequence() {
		ST_scanner::begin_sequence();

		Blocks.clear();

		if (write_simulation_sequence_mode) {
			simulation_sequence_file=fopen(simulation_sequence_fname,"wb");
		}
	}
	virtual void end_sequence() {
		if (write_simulation_sequence_mode) {
			if (simulation_sequence_file) {
				fclose(simulation_sequence_file);
				simulation_sequence_file=0;
			}
		}

	}
	virtual void begin_block() {
		if (statonly_mode) return;
		Blocks.add_block();
		if (Blocks.num_blocks==1) {
			Blocks.first_block_number=stat_num_blocks;
		}
	}
	virtual void end_block(float duration) {
		ST_scanner::end_block(duration);

		if (statonly_mode) return;
		if (Blocks.last_block) {
			Blocks.last_block->duration=duration;
			Blocks.last_block->initial_moment=block_moment;
			float maxRF=Blocks.last_block->get_maxRFamplitude();
			if (maxRF>Blocks.maxRFamplitude)
				Blocks.maxRFamplitude=maxRF;

			if ((write_simulation_sequence_mode)&&(simulation_sequence_file)) {
				Blocks.last_block->write_simulation_sequence_block(simulation_sequence_file);
			}
		}

		
	}
	virtual bool run_event(ST_scanner_event *event) {
		if (statonly_mode) return true;
		if (!event) return false;
		if (!Blocks.last_block)
			return false;
		Blocks.last_block->add_event(event);
		return true;
	}
	virtual ST_scanner_gradient_event *create_gradient() {
		return new STsim_gradient;
	}
	virtual ST_scanner_RF_event *create_RF() {
		return new STsim_RF;
	}
	virtual ST_scanner_ADC_event *create_ADC() {
		return new STsim_ADC;
	}
	virtual ST_scanner_arbgradient_event *create_arbgradient() {
		return new STsim_arbgradient;
	}
	virtual double get_SAR(ST_scanner_RF_event *event) {
		if (!event) return false;
		STsim_RF *ptr=(STsim_RF *)event;
		return ptr->get_SAR();
	}
	///////////////////////////////////////////////////////////////////////////////////////
};
*/
#endif
