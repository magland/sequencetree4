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


