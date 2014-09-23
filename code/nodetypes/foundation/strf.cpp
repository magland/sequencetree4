/* BEGIN EXCLUDE */
/* END EXCLUDE */

STRF::STRF() {
	ST_CLASS(STRF,STNode)
	RF=0;
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal, flip_angle, 90, degrees)
	ST_PARAMETER(STReal, pulse_duration, 1000, microsec)
	ST_PARAMETER(STReal, reference_fraction, 0.5,)
	ST_PARAMETER(STReal, bandwidth, 1000, Hz)
	ST_PARAMETER(STReal, time_step, 10, microsec)
	ST_PARAMETER(STReal, phase, 0, degrees)
	ST_PARAMETER(STReal, frequency, 0, Hz)
	ST_PARAMETER(STInteger, pulse_type, 1, 1=Excite; 2=Refocus)
	ST_PARAMETER(STVector3, gradient_amplitude, (0,0,0), uT/mm; for internal use)
	ST_PARAMETER(STReal, slice_thickness, 0, mm; for internal use)
	/* ST_CHILDREN */
}
STRF::~STRF() {
	delete_events();
}
void STRF::delete_events() {
	if (RF) delete RF; RF=0;
}
void STRF::pulseShape(double cycles,double &re,double &im) {
	re=1;
	im=0;
}
bool STRF::initialize() {
	clearChildren();
	if (!scanner()) return false;
	delete_events();
	RF=scanner()->allocateRFPulse(this);
	m_shape_changed=true;
	return true;
}
bool STRF::prepare() {
	if (!RF) return false;
	if (time_step<=0) return false;
	if (pulse_duration<0) return false;
	if (!sequence()) return false;
	if ((shapeChanged())||(pulse_duration.isModified())||(time_step.isModified())||(reference_fraction.isModified())||(bandwidth.isModified())) {
		m_shape_changed=false;
		long N=(long)(pulse_duration/time_step);
		if (N==0) {
			RF->setPulse(0,0,0,time_step,flip_angle,scanner()->rounduptime(reference_fraction*pulse_duration));
		}
		else {
			double *data_mag=(double *)malloc(sizeof(double)*N);
			double *data_phase=(double *)malloc(sizeof(double)*N);
			double reftime=scanner()->rounduptime(reference_fraction*pulse_duration);
			double sum_re=0;
			double sum_im=0;
			{for (long j=0; j<N; j++) {
				double re,im;
				pulseShape((j*time_step-reftime)*bandwidth/1.0E6,re,im);
				sum_re+=re;
				sum_im+=im;
				data_mag[j]=sqrt(re*re+im*im);
				data_phase[j]=atan2(im,re)/(2*3.1415926535)*360;
				if (data_phase[j]<0) data_phase[j]+=360;
			}}
			//uT * sec * Hz/uT * degrees = degrees  
			double factor=sqrt(sum_re*sum_re+sum_im*sum_im)*time_step/1.0E6*sequence()->gamma*360/90;
			{for (long j=0; j<N; j++) {
				data_mag[j]/=factor;
			}}
			RF->setPulse(N,data_mag,data_phase,time_step,90,reftime);
			free(data_mag);
			free(data_phase);
		}
		RF->setFlipAngle(flip_angle);
	}
	else if (flip_angle.isModified()) {
		RF->setFlipAngle(flip_angle);
	}
	RF->setReferenceInfo(gradient_amplitude.abs(),fabs(slice_thickness),pulse_type);
	
	setModified(false);
	return true;
}
bool STRF::run() {
	if (!RF) return false;
	if (!scanner()) return false;
	if (!sequence()) return false;
	RF->setStartTime(startTime());
	double ph=phase;
	Vector3 totmom=initialTotalMoment();
	ph+=sequence()->phaseAdjustment();
	ph+=(totmom.x()*sequence()->getFOVShiftX()
		+totmom.y()*sequence()->getFOVShiftY()
		+totmom.z()*sequence()->getFOVShiftZ())*sequence()->gamma/1.0E6*360;
	RF->setPhase(ph);
	double fr=frequency;
	fr+=(gradient_amplitude.x()*sequence()->getFOVShiftX()
		+gradient_amplitude.y()*sequence()->getFOVShiftY()
		+gradient_amplitude.z()*sequence()->getFOVShiftZ())*sequence()->gamma;
	RF->setFrequency(fr);
	if (RF->pulseCount()>0) {
		scanner()->addEvent(RF);
	}
	return true;
}
double STRF::duration() {
	if (!RF) return 0;
	return RF->pulseCount()*RF->timeStep();
}
double STRF::referenceTime() {
	if (!RF) return 0;
	return RF->referenceTime();
}
Vector3 STRF::terminalMoment() {
	if (pulse_type==1) return Vector3(0,0,0);
	if (pulse_type==2) return initialMoment()*(-1);
	return initialMoment();
}

Vector3 STRF::gradientStartTimes() {
	return Vector3(0,0,0);
}
Vector3 STRF::gradientEndTimes() {
	double dur=duration();
	return Vector3(dur,dur,dur);
}

void STRF::setGradientAmplitude(Vector3 amp) {
	gradient_amplitude=amp;
}


double STRF::SAR() {
	if (!RF) return 0;
	return RF->SAR();
}
