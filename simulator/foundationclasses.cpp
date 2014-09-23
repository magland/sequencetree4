#include "foundationclasses.h"

////// STSampledRF //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */ 

STSampledRF::STSampledRF() {
	ST_CLASS(STSampledRF,STRF)
	/* ST_PARAMETERS */
	ST_PARAMETER(STString,pulse_shape,pulse.mrp,)
	/* ST_CHILDREN */
	
	double data_real[100];
	double data_imag[100];
	for (long j=0; j<100; j++) {
		data_real[j]=1;
		data_imag[j]=0;
	}
	setSamples(100,data_real,data_imag,500,10,1000);
}

void STSampledRF::setSamples(long N, const double *data_real, const double *data_imag, double reference_center_time /*us*/, double reference_timestep /*us*/, double reference_bandwidth /*Hz*/) {
	m_num_samples=N;
	m_data_real.clear();
	m_data_imag.clear();
	for (long j=0; j<N; j++) {
		m_data_real << data_real[j];
		m_data_imag << data_imag[j];
	}
	m_reference_center_time=reference_center_time;
	m_reference_timestep=reference_timestep;
	m_reference_bandwidth=reference_bandwidth;
	setShapeChanged(true);
	setModified(true);
}

bool STSampledRF::prepare() {
	if (pulse_shape.isModified()) {
		if (!sequence()) return false;
		SString shape=pulse_shape.toString();
		STResource *R_data_real=sequence()->resource(shape+"->data_real");
		STResource *R_data_imag=sequence()->resource(shape+"->data_imag");
		STResource *R_timestep=sequence()->resource(shape+"->timestep");
		STResource *R_rephase_time=sequence()->resource(shape+"->rephase_time");
		STResource *R_bandwidth=sequence()->resource(shape+"->bandwidth");
		if ((!R_data_real)||(!R_data_imag)||(!R_timestep)||(!R_rephase_time)||(!R_bandwidth)) {
			printf("Cannot find resource: %s\n",shape.data());
			return false;
		}
		if ((R_data_real->doubleListCount()==0)||(R_data_real->doubleListCount()!=R_data_imag->doubleListCount())) {
			printf("Problem with pulse shape: %s\n",shape.data());
			return false;
		}
		double *data_real=(double *)malloc(sizeof(double)*R_data_real->doubleListCount());
		double *data_imag=(double *)malloc(sizeof(double)*R_data_real->doubleListCount());

		for (long j=0; j<R_data_real->doubleListCount(); j++) {
			data_real[j]=R_data_real->getDoubleList(j);
			data_imag[j]=R_data_imag->getDoubleList(j);
		}
		long N=R_data_real->doubleListCount();
		double timestep=R_timestep->getDouble();
		double bw=R_bandwidth->getDouble();
		double rt=R_rephase_time->getDouble();
		double reference_center_time=N*timestep-rt;
		setSamples(N,data_real,data_imag,reference_center_time,timestep,bw);

		free(data_real);
		free(data_imag);
	}
	if (bandwidth.isModified()) {
		setShapeChanged(true);
	}
	pulse_duration=m_num_samples*m_reference_timestep*m_reference_bandwidth/bandwidth;
	reference_fraction=m_reference_center_time/(m_num_samples*m_reference_timestep);
	return STRF::prepare();
}

void STSampledRF::pulseShape(double cycles,double &re,double &im) {
	double t2=cycles/m_reference_bandwidth*1.0E6+m_reference_center_time;
	long index=(long)(t2/m_reference_timestep);
	if ((index<0)||(index>=m_num_samples)) {
		re=im=0; 
	}
	else {
		re=m_data_real[index];
		im=m_data_imag[index];
	}
}


////// STEncode //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STEncode::STEncode() {
	ST_CLASS(STEncode,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3, moment,(0,0,0),[uT/mm]-us)
	ST_PARAMETER(STInteger, do_rewind, 1, 0 or 1)
	/* ST_CHILDREN */
	ST_CHILD(STGradientMom, Gradient)
}
bool STEncode::prepare() {
	if (!sequence()) return false;
	if (!scanner()) return false;

	//Prepare Gradient
	if (do_rewind)
		Gradient->setMoment(initialMoment()*(-1)+moment);
	else
		Gradient->setMoment(moment);
	if (Gradient->isModified())
		if (!Gradient->prepare()) return false;
	
	setModified(false);
	return true;
}

double STEncode::duration() {
	return Gradient->duration();
}


////// STRefocus //////
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


////// STArbAcquire //////
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


////// STReadout //////
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


////// STChain //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STChain::STChain() {
	ST_CLASS(STChain,STNode)
	/* ST_PARAMETERS */
	/* ST_CHILDREN */
}

bool overlaps(STNode *C1,STNode *C2) {
	Vector3 vec1=C1->gradientEndTimes();
	Vector3 vec2=C2->gradientStartTimes();
	double rel1=C1->relativeStartTime();
	double rel2=C2->relativeStartTime();
	if (vec1.x()+rel1>vec2.x()+rel2) return true;
	if (vec1.y()+rel1>vec2.y()+rel2) return true;
	if (vec1.z()+rel1>vec2.z()+rel2) return true;
	return false;
}

bool STChain::prepare() {
	//prepare children and set initial moments
	if (childCount()>0) {
		child(0)->setInitialMoment(initialMoment());
		child(0)->setInitialTotalMoment(initialTotalMoment());
	}
	{for (int j=0; j<childCount(); j++) {
		if (child(j)->isModified()) {
			if (!child(j)->prepare()) return false;
			if (j+1<childCount()) {
				child(j+1)->setInitialMoment(child(j)->terminalMoment());
				child(j+1)->setInitialTotalMoment(child(j)->initialTotalMoment()+child(j)->totalGradientMoment());
			}
		}
	}}
	
	SList<int> has_been_positioned;
	for (int j=0; j<childCount(); j++)
		has_been_positioned.append(0);
	
	//set timing
	bool something_changed=true;
	while (something_changed) {
		something_changed=false;
		for (int j=0; j<childCount(); j++) {
			if (has_been_positioned[j]==0) {
				if (m_alignment_data[j].alignment==ST_ALIGN_LEFT) {
					if (j==0) {
						child(j)->setRelativeStartTime(m_alignment_data[j].timing);
						something_changed=true;
						has_been_positioned[j]=1;
					}
					else if (has_been_positioned[j-1]) {
						Vector3 hold1=child(j-1)->gradientEndTimes();
						Vector3 hold2=child(j)->gradientStartTimes();
						double reltime1=child(j-1)->relativeStartTime();
						double timex=reltime1+hold1.x()-hold2.x();
						double timey=reltime1+hold1.y()-hold2.y();
						double timez=reltime1+hold1.z()-hold2.z();
						double reltime2=timex; if (timey>reltime2) reltime2=timey; if (timez>reltime2) reltime2=timez;
						child(j)->setRelativeStartTime(reltime2+m_alignment_data[j].timing);
						something_changed=true;
						has_been_positioned[j]=1;
					}
				}
				else if (m_alignment_data[j].alignment==ST_ALIGN_RIGHT) {
					if (j==childCount()-1) {
						child(j)->setRelativeStartTime(duration()-child(j)->duration()+m_alignment_data[j].timing);
						something_changed=true;
						has_been_positioned[j]=1;
					}
					else if (has_been_positioned[j+1]) {
						Vector3 hold1=child(j+1)->gradientStartTimes();
						Vector3 hold2=child(j)->gradientEndTimes();
						double reltime1=child(j+1)->relativeStartTime();
						double timex=reltime1-hold2.x()+hold1.x();
						double timey=reltime1-hold2.y()+hold1.y();
						double timez=reltime1-hold2.z()+hold1.z();
						double reltime2=timex; if (timey<reltime2) reltime2=timey; if (timez<reltime2) reltime2=timez;
						child(j)->setRelativeStartTime(reltime2);
						something_changed=true;
						has_been_positioned[j]=1;
					}
				}
				else if (m_alignment_data[j].alignment==ST_ALIGN_RELATIVE) {
					int index=m_alignment_data[j].relative_index;
					if ((index>=0)&&(index<childCount())) {
						if (has_been_positioned[index]) {
							child(j)->setRelativeStartTime(child(index)->relativeStartTime()+child(index)->referenceTime()-child(j)->referenceTime()+m_alignment_data[j].timing);
							something_changed=true;
							has_been_positioned[j]=1;
						}
					}
				}
				else if (m_alignment_data[j].alignment==ST_ALIGN_ABSOLUTE) {
					child(j)->setRelativeStartTime(m_alignment_data[j].timing-child(j)->referenceTime());
					something_changed=true;
					has_been_positioned[j]=1;
				}
				
			}
		}
	}
	
	//check for overlaps
	{for (int j=0; j<childCount(); j++) {
		if ((j==0)&&(child(j)->relativeStartTime()<0)) {
			SString str=SString("Error in ")+name()+", start time for "+child(j)->name()+" is negative";
			reportError(str);
			return false;
		}
		if ((j>0)&&(overlaps(child(j-1),child(j)))) {
			SString str=SString("Error in ")+name()+", "+child(j-1)->name()+" overlaps "+child(j)->name();
			reportError(str);
			return false;
		}
		if (j==childCount()-1) {
			if (child(j)->relativeStartTime()+child(j)->duration()>m_duration) {
				SString str=SString("Error in ")+name()+", end time for "+child(j)->name()+" overlaps end of chain";
				reportError(str);
				return false;
			}
		}
	}}
	
	setModified(false);
	
	
	return true;
}

void STChain::addChild(STNode *obj) {
	STNode::addChild(obj);
	STChainAlignmentData data;
	data.alignment=ST_ALIGN_LEFT;
	data.timing=0;
	data.relative_index=0;
	m_alignment_data.append(data);
}
void STChain::removeChild(int index) {
	STNode::removeChild(index);
	m_alignment_data.removeAt(index);
}
void STChain::setAlignment(int index,int alignment,double timing,int relative_index) {
	bool something_changed=false;
	if (m_alignment_data[index].alignment!=alignment) {
		m_alignment_data[index].alignment=alignment;
		something_changed=true;
	}
	if (m_alignment_data[index].timing!=timing) {
		m_alignment_data[index].timing=timing;
		something_changed=true;
	}
	if (m_alignment_data[index].relative_index!=relative_index) {
		m_alignment_data[index].relative_index=relative_index;
		something_changed=true;
	}
	if (something_changed) setModified(true);
}

void STChain::alignChild(STNode *childptr,int alignment,double timing,int relative_index) {
	for (int j=0; j<childCount(); j++) 
		if (child(j)==childptr) {
			if (j<m_alignment_data.count()) {
				m_alignment_data[j].alignment=alignment;
				m_alignment_data[j].timing=timing;
				m_alignment_data[j].relative_index=relative_index;
				return;
			}
		}
	printf("Error setting alignment for %s",childptr->name().data());
}


////// STAcquire //////
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


////// STSpinEchoBlock //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STSpinEchoBlock::STSpinEchoBlock() {
	ST_CLASS(STSpinEchoBlock,STChain)
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal,TE,20000,microsec)
	ST_PARAMETER(STReal,TR,50000,microsec)
	ST_PARAMETER(STVector3, kspace_dir, (1,0,0),)
	ST_PARAMETER(STVector3, kspace_echo, (0,0,0),)
	/* ST_CHILDREN */
	ST_CHILD(STExcite, Excite)
	ST_CHILD(STRefocus, Refocus)
	ST_CHILD(STAcquire, Acquire)
	ST_CHILD(STEncode, Rewind)
}
bool STSpinEchoBlock::initialize() {
	STChain::initialize();
	return true;
}
bool STSpinEchoBlock::prepare() {
	/* ST_ALIGNMENTS */
	ST_ALIGN(Excite, ST_ALIGN_LEFT, 1000, 0)
	ST_ALIGN(Refocus, ST_ALIGN_RELATIVE, TE/2, 0)
	ST_ALIGN(Acquire, ST_ALIGN_RELATIVE, TE, 0)
	ST_ALIGN(Rewind, ST_ALIGN_LEFT, 0, 0)
	
	if (!sequence()) return false;
	if (TR.isModified()) setDuration(TR);
	Acquire->moment_per_point=sequence()->kspace2moment(kspace_dir);
	Acquire->echo_moment=sequence()->kspace2moment(kspace_echo);
	return STChain::prepare();
}


////// STRadialLoop //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STRadialLoop::STRadialLoop() {
	ST_CLASS(STRadialLoop,STLoop)
	/* ST_PARAMETERS */	
	ST_PARAMETER(STIterator,angle,0:1:0 check: 0;90;180;270,degrees)
	ST_PARAMETER(STIterator,PE,0:1:0,)
	ST_PARAMETER(STVector3,readout_dir1,(1,0,0),)
	ST_PARAMETER(STVector3,readout_dir2,(0,1,0),)
	ST_PARAMETER(STVector3,PE_dir,(0,0,1),)
	/* ST_CHILDREN */
	ST_CHILD(STGradientEchoBlock,Block)
}
bool STRadialLoop::prepare() {
	return STLoop::prepare();
}


bool STRadialLoop::loopRun() {
	Block->kspace_dir=readout_dir1*cos(angle*3.141592/180)+readout_dir2*sin(angle*3.141592/180);
	Block->kspace_echo=PE_dir*PE;
	return Block->run();
}


////// STGradientEchoBlock //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STGradientEchoBlock::STGradientEchoBlock() {
	ST_CLASS(STGradientEchoBlock,STChain)
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal,TE,6000,microsec)
	ST_PARAMETER(STReal,TR,50000,microsec)
	ST_PARAMETER(STVector3, kspace_dir, (1,0,0),)
	ST_PARAMETER(STVector3, kspace_echo, (0,0,0),)
	ST_PARAMETER(STReal, excite_time, 1000, microsec)
	/* ST_CHILDREN */
	ST_CHILD(STExcite, Excite)
	ST_CHILD(STAcquire, Acquire)
	ST_CHILD(STEncode, Rewind)
	
	ST_DEFAULT(Excite->RF,STSincRF)
	ST_DEFAULT(Excite->RF->num_lobes_left,4)
	ST_DEFAULT(Excite->RF->num_lobes_right,1)
	ST_DEFAULT(Excite->RF->flip_angle,45)
	ST_DEFAULT(Excite->gradient_amplitude,(0,0,10))
	ST_DEFAULT(Excite->slice_thickness,10)
	ST_DEFAULT(Rewind->moment,(0,0,100000))
}
bool STGradientEchoBlock::initialize() {
	STChain::initialize();
	return true;
}
bool STGradientEchoBlock::prepare() {
	/* ST_ALIGNMENTS */
	ST_ALIGN(Excite, ST_ALIGN_LEFT, excite_time, 0)
	ST_ALIGN(Acquire, ST_ALIGN_RELATIVE, TE, 0)
	ST_ALIGN(Rewind, ST_ALIGN_LEFT, 0, 0)
	
	if (!sequence()) return false;
	if (TR.isModified()) setDuration(TR);
	Acquire->moment_per_point=sequence()->kspace2moment(kspace_dir);
	Acquire->echo_moment=sequence()->kspace2moment(kspace_echo);
	return STChain::prepare();
}


////// STSincRF //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */ 

STSincRF::STSincRF() {
	ST_CLASS(STSincRF,STRF)
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal,num_lobes_left,1,);
	ST_PARAMETER(STReal,num_lobes_right,1,);
	/* ST_CHILDREN */
}

bool STSincRF::prepare() {
	if ((num_lobes_left.isModified())||(num_lobes_right.isModified())||(bandwidth.isModified())) {
		double left_duration=(num_lobes_left+1)*1.0E6/bandwidth;
		double right_duration=(num_lobes_right+1)*1.0E6/bandwidth;
		if (left_duration+right_duration<=0) return false;
		pulse_duration=left_duration+right_duration;
		reference_fraction=left_duration/pulse_duration;
		setShapeChanged(true);
	}
	return STRF::prepare();
}

void STSincRF::pulseShape(double cycles,double &re,double &im) {
	double t=cycles*3.141592;
	if (fabs(t)<0.01)
		re=1-t*t/6;
	else
		re=sin(t)/t;
	im=0;
}


////// STGradientAmp //////
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


////// STCartesianLoop //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STCartesianLoop::STCartesianLoop() {
	ST_CLASS(STCartesianLoop,STLoop)
	/* ST_PARAMETERS */	
	ST_PARAMETER(STIterator,PE1,-64:1:63,)
	ST_PARAMETER(STIterator,PE2,0:1:0,)
	ST_PARAMETER(STVector3,readout_dir,(1,0,0),)
	ST_PARAMETER(STVector3,PE1_dir,(0,1,0),)
	ST_PARAMETER(STVector3,PE2_dir,(0,0,1),)
	/* ST_CHILDREN */
	ST_CHILD(STGradientEchoBlock,Block)
}
bool STCartesianLoop::prepare() {
	Block->kspace_dir=readout_dir;
	return STLoop::prepare();
}


bool STCartesianLoop::loopRun() {
	Block->kspace_echo=PE1_dir*PE1+PE2_dir*PE2;
	return Block->run();
}


////// STMultiAcquire //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STMultiAcquire::STMultiAcquire() {
	ST_CLASS(STMultiAcquire,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STInteger, num_echoes, 1, )
	ST_PARAMETER(STReal, echo_spacing, 5000, )
	ST_PARAMETER(STInteger, reference_echo, 0, )
	ST_PARAMETER(STInteger, alternating, 0, 0 or 1)
	ST_PARAMETER(STInteger, num_readout_points, 256,)
	ST_PARAMETER(STReal, dwell_time, 20, microsec)
	ST_PARAMETER(STReal, ramp_time, 100, microsec)
	ST_PARAMETER(STVector3, echo_moment, (0,0,0), [uT/mm]-us)
	ST_PARAMETER(STVector3, moment_per_point, (0,0,0), [uT/mm]-us)
	ST_PARAMETER(STVector3, step_moment, (0,0,0), [uT/mm]-us)
	ST_PARAMETER(STReal, maxamp, 0, [uT/mm]-us)
	ST_PARAMETER(STReal, ramprate, 0, [uT/mm]/us)
	/* ST_CHILDREN */
}

bool STMultiAcquire::prepare() {
	if (!sequence()) return false;
	if (!scanner()) return false;
	
	if (num_echoes.isModified()) {
		clearChildren();
		m_echoes.clear();
		for (int j=0; j<num_echoes; j++) {
			SString namestr=SString("Echo")+SString::number(j);
			STAcquire *new_echo=new STAcquire;
			new_echo->setName(namestr);
			m_echoes << new_echo;
			addChild(new_echo);
			new_echo->initialize();
		}
	}
	if (echo_moment.isModified()||step_moment.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->echo_moment=echo_moment+step_moment*j;
		}
	}
	if (reference_echo.isModified()) {
		setReferenceChildIndex(reference_echo);
	}
	if (moment_per_point.isModified()||alternating.isModified()||reference_echo.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			double sgn=1;
			if ((alternating)&&((j-reference_echo)%2!=0)) sgn=-1;
			m_echoes[j]->moment_per_point=moment_per_point*sgn;
			m_echoes[j]->Readout->round_up_reference_time=(int)((sgn+1)/2);
		}
	}
	if (num_readout_points.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->Readout->N=num_readout_points;
		}
	}
	if (dwell_time.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->Readout->dwell_time=dwell_time;
		}
	}
	if (ramp_time.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->ReadoutGradient->ramp_time_1=ramp_time;
			m_echoes[j]->ReadoutGradient->ramp_time_2=ramp_time;
		}
	}
	if (maxamp.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->Encode->maxamp=maxamp;
		}
	}
	if (ramprate.isModified()) {
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->Encode->ramprate=ramprate;
		}
	}
	
	Vector3 mom=initialMoment();
	Vector3 totmom=initialTotalMoment();
	{for (int j=0; j<m_echoes.count(); j++) {
		m_echoes[j]->setInitialMoment(mom);
		m_echoes[j]->setInitialTotalMoment(totmom);
		if (!m_echoes[j]->prepare()) return false;
		mom=mom+m_echoes[j]->totalGradientMoment();
		totmom=totmom+m_echoes[j]->totalGradientMoment();
	}}
	
	if (m_echoes.count()>0) {
		double ref_time=m_echoes[0]->relativeStartTime()+m_echoes[0]->referenceTime();
		for (int j=0; j<m_echoes.count(); j++) {
			m_echoes[j]->setRelativeStartTime(ref_time+echo_spacing*j-m_echoes[j]->referenceTime());
		}
	}
	
	//check for overlaps
	{for (int j=0; j<m_echoes.count(); j++) {
		if (j==0) {
			if (m_echoes[j]->relativeStartTime()<0) {
				SString str=SString("Error in ")+name()+", start time for "+m_echoes[j]->name()+" is negative";
				reportError(str);
				return false;
			}
		}
		else {
			if (overlaps(m_echoes[j-1],m_echoes[j])) {
				SString str=SString("Error in ")+name()+", "+m_echoes[j-1]->name()+" overlaps "+m_echoes[j]->name();
				reportError(str);
				return false;
			}
		}
	}}
	
	
	
	return true;
}

double STMultiAcquire::duration() {
	if (m_echoes.count()==0) return 0;
	int index=m_echoes.count()-1;
	return m_echoes[index]->relativeStartTime()+m_echoes[index]->duration();
}


void STMultiAcquire::setMDHOnline() {
	for (int j=0; j<m_echoes.count(); j++) {
		m_echoes[j]->Readout->scannerCommand("setMDHOnline");
	}
}


////// STCircleGradient //////
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STCircleGradient::STCircleGradient() {
	ST_CLASS(STCircleGradient,STArbGradient)
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal, kspace_radius_1, 20,)
	ST_PARAMETER(STReal, kspace_radius_2, 20,)
	ST_PARAMETER(STReal, num_cycles, 1,)
	ST_PARAMETER(STVector3, kspace_direction_1, (10,0,0),)
	ST_PARAMETER(STVector3, kspace_direction_2, (0,10,0),)
	/* ST_CHILDREN */
}
STCircleGradient::~STCircleGradient() {
}
bool STCircleGradient::prepare() {
	return STArbGradient::prepare();
}

//t ranges between 0 and 1 (across the plateau), return k-space location
Vector3 STCircleGradient::gradientShape(double t) {
	double t2=t*2*3.141592*num_cycles;
	return kspace_direction_1*kspace_radius_1*cos(t2)+kspace_direction_2*kspace_radius_2*sin(t2)+kspace_offset;
}


////// STExcite //////
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


////// STRF //////
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


////// STGradientMom //////
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


////// STArbGradient //////
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


