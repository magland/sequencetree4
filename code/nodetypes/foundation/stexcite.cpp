/* BEGIN EXCLUDE */
/* END EXCLUDE */

STExcite::STExcite() {
	ST_CLASS(STExcite,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3, gradient_amplitude, (0,0,0),uT/mm)
	ST_PARAMETER(STReal, slice_thickness, 0,mm)
	ST_PARAMETER(STReal, bandwidth, 1000,Hz)
	ST_PARAMETER(STInteger, prephase, 1, 0 or 1)
	/* ST_CHILDREN */
	ST_CHILD(STGradientMom, Prephase)
	ST_CHILD(STGradientAmp, SliceGradient)
	ST_CHILD(STRF, RF)
	
	setReferenceChildIndex(2);
}
bool STExcite::prepare() {
	if (!sequence()) return false;
	if (!scanner()) return false;
	//Prepare RF
	if ((slice_thickness.isModified())||(bandwidth.isModified())||(gradient_amplitude.isModified())) {
		SliceGradient->amplitude=gradient_amplitude;
		if ((slice_thickness>0)&&(gradient_amplitude.abs()>0)) {
			double gradamp=gradient_amplitude.abs();
			bandwidth=gradamp*slice_thickness*sequence()->gamma;
		}
		RF->bandwidth=bandwidth;
	}
	if (RF->isModified()) 
		if (!RF->prepare()) return false;
	double RF_duration=RF->duration();
	RF_duration=scanner()->rounduptime(RF_duration);
	
	//Prepare Slice Gradient
	SliceGradient->plateau_time=RF_duration;
	if (SliceGradient->isModified()) 
		if (!SliceGradient->prepare()) return false;

	//Prepare Prephase
	if (prephase) {
		double effective_time_before_excite=SliceGradient->ramp_time_1*0.5+RF->referenceTime();
		Prephase->setMoment(initialMoment()*(-1)+SliceGradient->amplitude*(effective_time_before_excite)*(-1));
	}
	else {
		Prephase->setMoment(Vector3(0,0,0));
	}
	if (Prephase->isModified())
		if (!Prephase->prepare()) return false;
		
	//set gradient information
	RF->setGradientAmplitude(SliceGradient->amplitude);
	RF->slice_thickness=slice_thickness;
	RF->setInitialTotalMoment(initialTotalMoment()+Prephase->totalGradientMoment()+SliceGradient->amplitude*SliceGradient->ramp_time_1/2);
	
	//Set start times
	Prephase->setRelativeStartTime(0);
	SliceGradient->setRelativeStartTime(Prephase->duration());
	RF->setRelativeStartTime(SliceGradient->relativeStartTime()+SliceGradient->ramp_time_1);
	
	setModified(false);
	
	return true;
}
Vector3 STExcite::terminalMoment() {
	if (!SliceGradient) return Vector3(0,0,0);
	if (!RF) return Vector3(0,0,0);
	double effective_time=SliceGradient->ramp_time_2/2+SliceGradient->plateau_time-RF->referenceTime();
	return SliceGradient->amplitude*effective_time;
}

double STExcite::duration() {
	return SliceGradient->relativeStartTime()+SliceGradient->duration();
}
