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
