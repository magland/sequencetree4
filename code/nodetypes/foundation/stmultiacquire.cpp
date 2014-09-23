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

