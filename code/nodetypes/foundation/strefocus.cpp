/* BEGIN EXCLUDE */
/* END EXCLUDE */

STRefocus::STRefocus() {
	ST_CLASS(STRefocus,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3, gradient_amplitude, (0,0,0),uT/mm)
	ST_PARAMETER(STReal, slice_thickness, 0,mm)
	ST_PARAMETER(STReal, bandwidth, 1000,Hz)
	ST_PARAMETER(STVector3, crusher_moment, (0,0,0), [uT/mm]-us)
	ST_PARAMETER(STReal, flip_angle, 180, degrees);
	/* ST_CHILDREN */
	ST_CHILD(STGradientMom, Crusher1)
	ST_CHILD(STGradientAmp, SliceGradient)
	ST_CHILD(STRF, RF)
	ST_CHILD(STGradientMom, Crusher2)
	
	setReferenceChildIndex(2);
}
bool STRefocus::prepare() {
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
	if (flip_angle.isModified())
		RF->flip_angle=flip_angle;
	if (RF->isModified()) 
		if (!RF->prepare()) return false;
	double RF_duration=RF->duration();
	RF_duration=scanner()->rounduptime(RF_duration);
	
	//Prepare Slice Gradient
	SliceGradient->plateau_time=RF_duration;
	if (SliceGradient->isModified()) 
		if (!SliceGradient->prepare()) return false;

	//Prepare Crushers
	if (crusher_moment.isModified()) {
		Crusher1->setMoment(crusher_moment);
		Crusher2->setMoment(crusher_moment);
		if (!Crusher1->prepare()) return false;
		if (!Crusher2->prepare()) return false;
	}
	
	//set gradient information
	RF->setGradientAmplitude(SliceGradient->amplitude);
	RF->slice_thickness=slice_thickness;
	RF->setInitialTotalMoment(initialTotalMoment()+Crusher1->totalGradientMoment()+SliceGradient->amplitude*SliceGradient->ramp_time_1/2);
	
	//Set start times
	Crusher1->setRelativeStartTime(0);
	SliceGradient->setRelativeStartTime(Crusher1->duration());
	RF->setRelativeStartTime(SliceGradient->relativeStartTime()+SliceGradient->ramp_time_1);
	if (SliceGradient->duration()>0)
		Crusher2->setRelativeStartTime(SliceGradient->relativeStartTime()+SliceGradient->duration());
	else
		Crusher2->setRelativeStartTime(RF->relativeStartTime()+RF->duration());
	
	setModified(false);
	return true;
}
Vector3 STRefocus::terminalMoment() {
	Vector3 ret=initialMoment()+crusher_moment;
	double effective_time_1=SliceGradient->ramp_time_1/2+RF->referenceTime();
	double effective_time_2=SliceGradient->ramp_time_2/2+SliceGradient->plateau_time-RF->referenceTime();
	ret=ret+SliceGradient->amplitude*effective_time_1;
	ret=ret*(-1); //refocus
	ret=ret+SliceGradient->amplitude*effective_time_2;
	ret=ret+crusher_moment;
	return ret;
}

double STRefocus::duration() {
	return Crusher2->relativeStartTime()+Crusher2->duration();
}
