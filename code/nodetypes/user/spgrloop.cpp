SPGRLoop::SPGRLoop() {
	ST_CLASS(SPGRLoop,STLoop)
	/* ST_PARAMETERS */	
	ST_PARAMETER(STIterator,PE1,0:1:0,)
	ST_PARAMETER(STIterator,PE2,0:1:0,)
	ST_PARAMETER(STVector3,readout_dir,(1,0,0),)
	ST_PARAMETER(STVector3,PE1_dir,(0,1,0),)
	ST_PARAMETER(STVector3,PE2_dir,(0,0,1),)
	ST_PARAMETER(STInteger,RF_spoiling,0,0 or 1)
	/* ST_CHILDREN */
	ST_CHILD(STGradientEchoBlock,Block)
}
bool SPGRLoop::prepare() {
	Block->kspace_dir=readout_dir;
	return STLoop::prepare();
}


bool SPGRLoop::loopRun() {
    if (RF_spoiling) {
        sequence()->phase_shift=rand()%360;
    }
	Block->kspace_echo=PE1_dir*PE1+PE2_dir*PE2;
	return Block->run();
}
