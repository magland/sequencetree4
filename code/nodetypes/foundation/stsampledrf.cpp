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
