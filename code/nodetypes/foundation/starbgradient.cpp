/* BEGIN EXCLUDE */
/* END EXCLUDE */

STArbGradient::STArbGradient() {
	ST_CLASS(STArbGradient,STNode)
	GX=0;
	GY=0;
	GZ=0;
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal, ramp_time_1, 200,microsec)
	ST_PARAMETER(STReal, plateau_time, 5000,microsec)
	ST_PARAMETER(STReal, ramp_time_2, 200,microsec)
	ST_PARAMETER(STVector3, kspace_offset, (0,0,0),)
	ST_PARAMETER(STReal, peakamp, 0, uT/mm)
	ST_PARAMETER(STReal, peakslew, 0, [uT/mm]/us)
	/* ST_CHILDREN */
}
STArbGradient::~STArbGradient() {
	delete_events();
}
void STArbGradient::delete_events() {
	if (GX) delete GX; GX=0;
	if (GY) delete GY; GY=0;
	if (GZ) delete GZ; GZ=0;
}
bool STArbGradient::initialize() {
	clearChildren();
	if (!scanner()) return false;
	delete_events();
	GX=scanner()->allocateArbGradient(this);
	GY=scanner()->allocateArbGradient(this);
	GZ=scanner()->allocateArbGradient(this);
	return true;
}
bool STArbGradient::prepare() {
	peakamp=0;
	peakslew=0;
	if (!GX) return false; if (!GY) return false; if (!GZ) return false;
	double hold_peakamp=0;
	double hold_peakslew=0;
	long N=(long)(duration()/10)+1;
	m_total_moment=Vector3(0,0,0);
	if ((plateau_time<=10)||(N==0)||(ramp_time_1<=0)||(ramp_time_2<=0)) {
		GX->setArbGradient(1,0,0,10);
		GY->setArbGradient(2,0,0,10);
		GZ->setArbGradient(3,0,0,10);
	}
	else {
		Vector3 initial_amplitude=(momentAt(10/plateau_time)-momentAt(0))/10;
		Vector3 final_amplitude=(momentAt(1)-momentAt(1-10/plateau_time))/10;
		double *ampX=(double *)malloc(sizeof(double)*N);
		double *ampY=(double *)malloc(sizeof(double)*N);
		double *ampZ=(double *)malloc(sizeof(double)*N);
		for (long j=0; j<N; j++) {
			double t=(j*10-ramp_time_1)/plateau_time;
			if (t<0) {
				double holdt=(j*10/ramp_time_1);
				ampX[j]=holdt*initial_amplitude.x();
				ampY[j]=holdt*initial_amplitude.y();
				ampZ[j]=holdt*initial_amplitude.z();
			}
			else if (t<1) {
				Vector3 holdamp=(momentAt(t+10/plateau_time)-momentAt(t))/10;
				ampX[j]=holdamp.x();
				ampY[j]=holdamp.y();
				ampZ[j]=holdamp.z();
			}
			else {
				double holdt=1-(j*10-ramp_time_1-plateau_time)/ramp_time_2;
				ampX[j]=holdt*final_amplitude.x();
				ampY[j]=holdt*final_amplitude.y();
				ampZ[j]=holdt*final_amplitude.z();
			}
			m_total_moment.setX(m_total_moment.x()+ampX[j]*10);
			m_total_moment.setY(m_total_moment.y()+ampY[j]*10);
			m_total_moment.setZ(m_total_moment.z()+ampZ[j]*10);
			if (fabs(ampX[j])>hold_peakamp) hold_peakamp=fabs(ampX[j]);
			if (fabs(ampY[j])>hold_peakamp) hold_peakamp=fabs(ampY[j]);
			if (fabs(ampZ[j])>hold_peakamp) hold_peakamp=fabs(ampZ[j]);
			if (j>0) {
				double s1=fabs((ampX[j]-ampX[j-1])/10);
				double s2=fabs((ampY[j]-ampY[j-1])/10);
				double s3=fabs((ampZ[j]-ampZ[j-1])/10);
				if (s1>hold_peakslew) hold_peakslew=s1;
				if (s2>hold_peakslew) hold_peakslew=s2;
				if (s3>hold_peakslew) hold_peakslew=s3;
			}
		}
		GX->setArbGradient(1,N,ampX,10);
		GY->setArbGradient(2,N,ampY,10);
		GZ->setArbGradient(3,N,ampZ,10);
		free(ampX);
		free(ampY);
		free(ampZ);
	}
	peakamp=hold_peakamp;
	peakslew=hold_peakslew;
	setModified(false);
	return true;
}
bool STArbGradient::run() {
	if (!GX) return false; if (!GY) return false; if (!GZ) return false;
	if (!scanner()) return false;
	GX->setStartTime(startTime());
	GY->setStartTime(startTime());
	GZ->setStartTime(startTime());
	scanner()->addEvent(GX);
	scanner()->addEvent(GY);
	scanner()->addEvent(GZ);
	return true;
}
double STArbGradient::duration() {
	return ramp_time_1+plateau_time+ramp_time_2;
}
Vector3 STArbGradient::terminalMoment() {
	return initialMoment()+totalGradientMoment();
}
Vector3 STArbGradient::totalGradientMoment() {
	return m_total_moment;
}

Vector3 STArbGradient::gradientStartTimes() {
	return Vector3(0,0,0);
}
Vector3 STArbGradient::gradientEndTimes() {
	double dur=duration();
	return Vector3(dur,dur,dur);
}

Vector3 STArbGradient::momentAt(double t) {
	if (!sequence()) return Vector3(0,0,0);
	return sequence()->kspace2moment(gradientShape(t)+kspace_offset);
}

Vector3 STArbGradient::gradientShape(double t) {
	t=t+1; //dummy operation
	return Vector3(0,0,0);
}

Vector3 STArbGradient::ramp1Moment() {
	Vector3 initial_amplitude=(momentAt(10/plateau_time)-momentAt(0))/10;
	return initial_amplitude*ramp_time_1/2;
}

