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

