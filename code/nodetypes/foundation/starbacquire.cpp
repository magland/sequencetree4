/* BEGIN EXCLUDE */
/* END EXCLUDE */

STArbAcquire::STArbAcquire() {
	ST_CLASS(STArbAcquire,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3,kspace_offset,(0,0,0),)
	/* ST_CHILDREN */
	ST_CHILD(STGradientMom, Encode)
	ST_CHILD(STArbGradient, ReadoutGradient)
	ST_CHILD(STReadout, Readout)
	setReferenceChildIndex(2);
}
bool STArbAcquire::prepare() {
	if (!sequence()) return false;
	if (!scanner()) return false;
	//Prepare Readout
	if (Readout->isModified()) 
		if (!Readout->prepare()) return false;
	double readout_duration=Readout->duration();
	readout_duration=scanner()->rounduptime(readout_duration);
	
	//Prepare Readout Gradient
	ReadoutGradient->plateau_time=readout_duration;
	ReadoutGradient->kspace_offset=kspace_offset;
	if (ReadoutGradient->isModified()) 
		if (!ReadoutGradient->prepare()) return false;

	//Prepare Encode
	Encode->setMoment(initialMoment()*(-1)-ReadoutGradient->ramp1Moment()+ReadoutGradient->momentAt(0));
	if (Encode->isModified())
		if (!Encode->prepare()) return false;
	
	//Set start times
	Encode->setRelativeStartTime(0);
	ReadoutGradient->setRelativeStartTime(Encode->duration());
	Readout->setRelativeStartTime(ReadoutGradient->relativeStartTime()+ReadoutGradient->ramp_time_1);
	
	setModified(false);
	
	return true;
}

double STArbAcquire::duration() {
	return ReadoutGradient->relativeStartTime()+ReadoutGradient->duration();
}

