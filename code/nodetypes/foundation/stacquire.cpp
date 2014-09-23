/* BEGIN EXCLUDE */

/* END EXCLUDE */

STAcquire::STAcquire() {
	ST_CLASS(STAcquire,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3, echo_moment, (0,0,0), [uT/mm]-us)
	ST_PARAMETER(STVector3, moment_per_point, (0,0,0),[uT/mm]-us)
	/* ST_CHILDREN */
	ST_CHILD(STGradientMom, Encode)
	ST_CHILD(STGradientAmp, ReadoutGradient)
	ST_CHILD(STReadout, Readout)
	
	setReferenceChildIndex(2);
}
bool STAcquire::prepare() {
	if (!sequence()) return false;
	if (!scanner()) return false;
	//Prepare Readout
	if (Readout->isModified()) 
		if (!Readout->prepare()) return false;
	double readout_duration=Readout->duration();
	readout_duration=scanner()->rounduptime(readout_duration);
	
	//Prepare Readout Gradient
	ReadoutGradient->amplitude=moment_per_point/Readout->dwell_time;
	ReadoutGradient->plateau_time=readout_duration;
	if (ReadoutGradient->isModified()) 
		if (!ReadoutGradient->prepare()) return false;

	//Prepare Encode
	double effective_time_to_echo=ReadoutGradient->ramp_time_1/2+Readout->referenceTime();
	Encode->setMoment(initialMoment()*(-1)+echo_moment-ReadoutGradient->amplitude*effective_time_to_echo);
	if (Encode->isModified())
		if (!Encode->prepare()) return false;
		
	//set gradient information for readout
	Readout->setGradientAmplitude(ReadoutGradient->amplitude);
	Readout->setInitialTotalMoment(initialTotalMoment()+Encode->totalGradientMoment()+ReadoutGradient->amplitude*ReadoutGradient->ramp_time_1/2);
	
	//Set start times
	Encode->setRelativeStartTime(0);
	ReadoutGradient->setRelativeStartTime(Encode->duration());
	Readout->setRelativeStartTime(ReadoutGradient->relativeStartTime()+ReadoutGradient->ramp_time_1);
	
	setModified(false);
	
	return true;
}

double STAcquire::duration() {
	return ReadoutGradient->relativeStartTime()+ReadoutGradient->duration();
}

