/* BEGIN EXCLUDE */
/* END EXCLUDE */

STGradientAmp::STGradientAmp() {
	ST_CLASS(STGradientAmp,STNode)
	GX=0;
	GY=0;
	GZ=0;
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal, ramp_time_1, 200,microsec)
	ST_PARAMETER(STReal, plateau_time, 0,microsec)
	ST_PARAMETER(STReal, ramp_time_2, 200,microsec)
	ST_PARAMETER(STVector3, amplitude, (0,0,0),uT/mm)
	/* ST_CHILDREN */
}
STGradientAmp::~STGradientAmp() {
	delete_events();
}
void STGradientAmp::delete_events() {
	if (GX) delete GX; GX=0;
	if (GY) delete GY; GY=0;
	if (GZ) delete GZ; GZ=0;
}
bool STGradientAmp::initialize() {
	clearChildren();
	if (!scanner()) return false;
	delete_events();
	GX=scanner()->allocateGradient(this);
	GY=scanner()->allocateGradient(this);
	GZ=scanner()->allocateGradient(this);
	return true;
}
bool STGradientAmp::prepare() {
	if (!GX) return false; if (!GY) return false; if (!GZ) return false;
	GX->setGradient(1,amplitude.x(),ramp_time_1,plateau_time,ramp_time_2);
	GY->setGradient(2,amplitude.y(),ramp_time_1,plateau_time,ramp_time_2);
	GZ->setGradient(3,amplitude.z(),ramp_time_1,plateau_time,ramp_time_2);
	setModified(false);
	return true;
}
bool STGradientAmp::run() {
	if (!GX) return false; if (!GY) return false; if (!GZ) return false;
	if (!scanner()) return false;
	GX->setStartTime(startTime());
	GY->setStartTime(startTime());
	GZ->setStartTime(startTime());
	if (GX->amplitude()!=0) scanner()->addEvent(GX);
	if (GY->amplitude()!=0) scanner()->addEvent(GY);
	if (GZ->amplitude()!=0) scanner()->addEvent(GZ);
	return true;
}
double STGradientAmp::duration() {
	return ramp_time_1+plateau_time+ramp_time_2;
}
Vector3 STGradientAmp::terminalMoment() {
	return initialMoment()+totalGradientMoment();
}
Vector3 STGradientAmp::totalGradientMoment() {
	return amplitude*(ramp_time_1/2+plateau_time+ramp_time_2/2);
}

Vector3 STGradientAmp::gradientStartTimes() {
	double dur=duration();
	Vector3 ret(dur,dur,dur);
	if (amplitude.x()!=0) ret.setX(0);
	if (amplitude.y()!=0) ret.setY(0);
	if (amplitude.z()!=0) ret.setZ(0);
	return ret;
}
Vector3 STGradientAmp::gradientEndTimes() {
	double dur=duration();
	Vector3 ret(0,0,0);
	if (amplitude.x()!=0) ret.setX(dur);
	if (amplitude.y()!=0) ret.setY(dur);
	if (amplitude.z()!=0) ret.setZ(dur);
	return ret;
}
