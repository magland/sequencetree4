/* BEGIN EXCLUDE */
/* END EXCLUDE */

STGradientMom::STGradientMom() {
	ST_CLASS(STGradientMom,STNode)
	GX=0;
	GY=0;
	GZ=0;
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3, ramp_times_1, (0,0,0),microsec)
	ST_PARAMETER(STVector3, plateau_times, (0,0,0),microsec)
	ST_PARAMETER(STVector3, ramp_times_2, (0,0,0),microsec)
	ST_PARAMETER(STVector3, start_times, (0,0,0),microsec)
	ST_PARAMETER(STVector3, amplitude, (0,0,0),uT/mm)
	ST_PARAMETER(STInteger, alignment, 0, 0=left; 1=right; 2=center)
	ST_PARAMETER(STInteger, always_min_dur, 0,0 or 1)
	ST_PARAMETER(STReal, maxamp, 0, [uT/mm])
	ST_PARAMETER(STReal, ramprate, 0, [uT/mm]/us)
	/* ST_CHILDREN */
}
STGradientMom::~STGradientMom() {
	delete_events();
}
void STGradientMom::delete_events() {
	if (GX) delete GX; GX=0;
	if (GY) delete GY; GY=0;
	if (GZ) delete GZ; GZ=0;
	
}
bool STGradientMom::initialize() {
	clearChildren();
	//important to initialize these parameters to zero
	ramp_times_1=Vector3(0,0,0);
	plateau_times=Vector3(0,0,0);
	ramp_times_2=Vector3(0,0,0);
	amplitude=Vector3(0,0,0);
	if (!scanner()) return false;
	delete_events();
	GX=scanner()->allocateGradient(this);
	GY=scanner()->allocateGradient(this);
	GZ=scanner()->allocateGradient(this);
	return true;
}
bool STGradientMom::setMoment(Vector3 mom) {
	if (!sequence()) return false;
	if (!scanner()) return false;
	double hold_ramprate=sequence()->ramprate;
	double hold_maxamp=sequence()->maxamp;
	if (maxamp!=0) hold_maxamp=maxamp;
	if (ramprate!=0) hold_ramprate=ramprate;
	if (hold_maxamp<=0) return false;
	if (hold_ramprate<=0) return false;
	double amps[3]; amps[0]=amplitude.x(); amps[1]=amplitude.y(); amps[2]=amplitude.z();
	double moms[3]; moms[0]=mom.x(); moms[1]=mom.y(); moms[2]=mom.z();
	double rampups[3]; rampups[0]=ramp_times_1.x(); rampups[1]=ramp_times_1.y(); rampups[2]=ramp_times_1.z();
	double plateaus[3]; plateaus[0]=plateau_times.x(); plateaus[1]=plateau_times.y(); plateaus[2]=plateau_times.z();
	double rampdowns[3]; rampdowns[0]=ramp_times_2.x(); rampdowns[1]=ramp_times_2.y(); rampdowns[2]=ramp_times_2.z();
	for (int direc=0; direc<3; direc++) {
		double hold=rampups[direc]/2+plateaus[direc]+rampdowns[direc]/2;
		if ((hold>0)&&(fabs(moms[direc])/hold<=hold_maxamp)&&(!always_min_dur)) { //maintain shape
			amps[direc]=moms[direc]/hold;
		}
		else if (fabs(moms[direc])<hold_maxamp*hold_maxamp/hold_ramprate) { //triangle
			double ramp=sqrt(fabs(moms[direc]/hold_ramprate));
			ramp=scanner()->rounduptime(ramp);
			if (ramp>0) 
				amps[direc]=moms[direc]/ramp;
			else
				amps[direc]=0;
			rampups[direc]=ramp;
			plateaus[direc]=0;
			rampdowns[direc]=ramp;
		}
		else { //trapezoid
			double ramp=hold_maxamp/hold_ramprate;
			double plateau=fabs(moms[direc])/hold_maxamp-hold_maxamp/hold_ramprate;
			ramp=scanner()->rounduptime(ramp);
			plateau=scanner()->rounduptime(plateau);
			amps[direc]=moms[direc]/(ramp+plateau);
			rampups[direc]=ramp;
			plateaus[direc]=plateau;
			rampdowns[direc]=ramp;
		}
	}
	ramp_times_1=Vector3(rampups[0],rampups[1],rampups[2]);
	plateau_times=Vector3(plateaus[0],plateaus[1],plateaus[2]);
	ramp_times_2=Vector3(rampdowns[0],rampdowns[1],rampdowns[2]);
	amplitude=Vector3(amps[0],amps[1],amps[2]);
	return true;
}		
bool STGradientMom::prepare() {
	if (!GX) return false; if (!GY) return false; if (!GZ) return false;
	GX->setGradient(1,amplitude.x(),ramp_times_1.x(),plateau_times.x(),ramp_times_2.x());
	GY->setGradient(2,amplitude.y(),ramp_times_1.y(),plateau_times.y(),ramp_times_2.y());
	GZ->setGradient(3,amplitude.z(),ramp_times_1.z(),plateau_times.z(),ramp_times_2.z());
	start_times=Vector3(0,0,0);
	if (alignment==1) {
		double dx=ramp_times_1.x()+plateau_times.x()+ramp_times_2.x();
		double dy=ramp_times_1.y()+plateau_times.y()+ramp_times_2.y();
		double dz=ramp_times_1.z()+plateau_times.z()+ramp_times_2.z();
		double dur=dx; if (dy>dur) dur=dy; if (dz>dur) dur=dz;
		start_times.setX(dur-dx);
		start_times.setY(dur-dy);
		start_times.setZ(dur-dz);
	}
	else if (alignment==2) {
		double dx=ramp_times_1.x()+plateau_times.x()+ramp_times_2.x();
		double dy=ramp_times_1.y()+plateau_times.y()+ramp_times_2.y();
		double dz=ramp_times_1.z()+plateau_times.z()+ramp_times_2.z();
		double dur=dx; if (dy>dur) dur=dy; if (dz>dur) dur=dz;
		start_times.setX(scanner()->rounddowntime((dur-dx)/2));
		start_times.setY(scanner()->rounddowntime((dur-dy)/2));
		start_times.setZ(scanner()->rounddowntime((dur-dz)/2));
	}
	setModified(false);
	return true;
}
bool STGradientMom::run() {
	if (!GX) return false; if (!GY) return false; if (!GZ) return false;
	if (!scanner()) return false;
	GX->setStartTime(startTime()+start_times.x());
	GY->setStartTime(startTime()+start_times.y());
	GZ->setStartTime(startTime()+start_times.z());
	if (GX->amplitude()!=0) scanner()->addEvent(GX);
	if (GY->amplitude()!=0) scanner()->addEvent(GY);
	if (GZ->amplitude()!=0) scanner()->addEvent(GZ);
	return true;
}
double STGradientMom::duration() {
	double dx=start_times.x()+ramp_times_1.x()+plateau_times.x()+ramp_times_2.x();
	double dy=start_times.y()+ramp_times_1.y()+plateau_times.y()+ramp_times_2.y();
	double dz=start_times.z()+ramp_times_1.z()+plateau_times.z()+ramp_times_2.z();
	double ret=0;
	if (dx>ret) ret=dx;
	if (dy>ret) ret=dy;
	if (dz>ret) ret=dz;
	return ret;
}
Vector3 STGradientMom::terminalMoment() {
	return initialMoment()+totalGradientMoment();
}
Vector3 STGradientMom::totalGradientMoment() {
	double mx=amplitude.x()*(ramp_times_1.x()/2+plateau_times.x()+ramp_times_2.x()/2);
	double my=amplitude.y()*(ramp_times_1.y()/2+plateau_times.y()+ramp_times_2.y()/2);
	double mz=amplitude.z()*(ramp_times_1.z()/2+plateau_times.z()+ramp_times_2.z()/2);
	return Vector3(mx,my,mz);
}

Vector3 STGradientMom::gradientStartTimes() {
	double dur=duration();
	Vector3 ret(dur,dur,dur);
	if (amplitude.x()!=0) ret.setX(start_times.x());
	if (amplitude.y()!=0) ret.setY(start_times.y());
	if (amplitude.z()!=0) ret.setZ(start_times.z());
	return ret;
}
Vector3 STGradientMom::gradientEndTimes() {
	Vector3 ret(0,0,0);
	if (amplitude.x()!=0) ret.setX(start_times.x()+ramp_times_1.x()+plateau_times.x()+ramp_times_2.x());
	if (amplitude.y()!=0) ret.setY(start_times.y()+ramp_times_1.y()+plateau_times.y()+ramp_times_2.y());
	if (amplitude.z()!=0) ret.setZ(start_times.z()+ramp_times_1.z()+plateau_times.z()+ramp_times_2.z());
	return ret;
}
