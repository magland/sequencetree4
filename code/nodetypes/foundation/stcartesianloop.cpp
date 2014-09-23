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
