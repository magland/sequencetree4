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
