/* BEGIN EXCLUDE */
/* END EXCLUDE */

STReadout::STReadout() {
	ST_CLASS(STReadout,STNode)
	Readout=0;
	/* ST_PARAMETERS */
	ST_PARAMETER(STInteger,enabled,1,)
	ST_PARAMETER(STReal, dwell_time, 30, microsec)
	ST_PARAMETER(STInteger, N, 256,)
	ST_PARAMETER(STReal, reference_fraction, 0.5,)	
	ST_PARAMETER(STReal, actual_reference_fraction, 0.5,)
	ST_PARAMETER(STReal, reference_sample,128,)	
	ST_PARAMETER(STReal, phase, 0, degrees)
	ST_PARAMETER(STReal, frequency, 0, Hz)
	ST_PARAMETER(STVector3, gradient_amplitude, (0,0,0), uT/mm; for internal use)
	ST_PARAMETER(STInteger, round_up_reference_time, 1, 0 or 1)
	/* ST_CHILDREN */
	m_ADC_index=-1;
}
STReadout::~STReadout() {
	delete_events();
}
void STReadout::delete_events() {
	if (Readout) delete Readout; Readout=0;
}
bool STReadout::initialize() {
	clearChildren();
	if (!scanner()) return false;
	delete_events();
	Readout=scanner()->allocateReadout(this);
	return true;
}
bool STReadout::prepare() {
	if (!Readout) return false;
	if (dwell_time<=0) return false;
	if (!sequence()) return false;
	
	float reference_time=0;
	if (round_up_reference_time) 
		reference_time=scanner()->rounduptime(N*dwell_time*reference_fraction);
	else
		reference_time=scanner()->rounddowntime(N*dwell_time*reference_fraction);
	actual_reference_fraction=reference_time/(N*dwell_time);
	reference_sample=actual_reference_fraction*N+0.5;
	
	Readout->setReadout(N,dwell_time,reference_time,m_ADC_index);
	
	setModified(false);
	return true;
}
bool STReadout::run() {
	if (!Readout) return false;
	if (!scanner()) return false;
	if (!sequence()) return false;
	Readout->setADCIndex(m_ADC_index);
	Readout->setStartTime(startTime());
	double ph=phase;
	Vector3 totmom=initialTotalMoment();
	ph+=sequence()->phaseAdjustment();
	ph+=(totmom.x()*sequence()->getFOVShiftX()
		+totmom.y()*sequence()->getFOVShiftY()
		+totmom.z()*sequence()->getFOVShiftZ())*sequence()->gamma/1.0E6*360;
	Readout->setPhase(ph);
	double fr=frequency;
	fr+=(gradient_amplitude.x()*sequence()->getFOVShiftX()
		+gradient_amplitude.y()*sequence()->getFOVShiftY()
		+gradient_amplitude.z()*sequence()->getFOVShiftZ())*sequence()->gamma;
	Readout->setFrequency(fr);
	if (enabled) {
		if (Readout->readoutCount()>0) scanner()->addEvent(Readout);
	}
	return true;
}
double STReadout::duration() {
	if (!Readout) return 0;
	return Readout->readoutCount()*Readout->dwellTime();
}
double STReadout::referenceTime() {
	if (!Readout) return 0;
	return Readout->referenceTime();
}

Vector3 STReadout::gradientStartTimes() {
	return Vector3(0,0,0);
}
Vector3 STReadout::gradientEndTimes() {
	double dur=duration();
	return Vector3(dur,dur,dur);
}

void STReadout::setGradientAmplitude(Vector3 amp) {
	gradient_amplitude=amp;
}

void STReadout::setADCIndices(int &indx) {
	m_ADC_index=indx;
	indx++;
}

void STReadout::setReadoutIndex(int iterator_index,int indx) {
	if (!scanner()) return;
	scanner()->setReadoutIndex(m_ADC_index,iterator_index,indx);
}

void STReadout::setCurrentLine(long val) {
	Readout->setCurrentLine(val);
}
void STReadout::setCurrentPartition(long val) {
	Readout->setCurrentPartition(val);
}
void STReadout::setCurrentAverage(long val) {
	Readout->setCurrentAverage(val);
}
void STReadout::setCurrentSlice(long val) {
	Readout->setCurrentSlice(val);
}
void STReadout::setCurrentEcho(long val) {
	Readout->setCurrentEcho(val);
}
void STReadout::setCurrentPhase(long val) {
	Readout->setCurrentPhase(val);
}
void STReadout::setCurrentRepetition(long val) {
	Readout->setCurrentRepetition(val);
}
void STReadout::setCurrentSet(long val) {
	Readout->setCurrentSet(val);
}
void STReadout::setCurrentSegment(long val) {
	Readout->setCurrentSegment(val);
}
void STReadout::setCurrentIda(long val) {
	Readout->setCurrentIda(val);
}
void STReadout::setCurrentIdb(long val) {
	Readout->setCurrentIdb(val);
}
void STReadout::setCurrentIdc(long val) {
	Readout->setCurrentIdc(val);
}
void STReadout::setCurrentIdd(long val) {
	Readout->setCurrentIdd(val);
}
void STReadout::setCurrentIde(long val) {
	Readout->setCurrentIde(val);
}
void STReadout::setFirstScanInSlice(bool val) {
	Readout->setFirstScanInSlice(val);
}
void STReadout::setLastScanInSlice(bool val) {
	Readout->setLastScanInSlice(val);
}
void STReadout::setLastScanInConcatenation(bool val) {
	Readout->setLastScanInConcatenation(val);
}
void STReadout::setLastScanInMeasurement(bool val) {
	Readout->setLastScanInMeasurement(val);
}

void STReadout::scannerCommand(SString command_name,long param1,long param2,long param3) {
	if (!scanner()) return;
	scanner()->scannerCommand(command_name,Readout,&param1,&param2,&param3);
}





