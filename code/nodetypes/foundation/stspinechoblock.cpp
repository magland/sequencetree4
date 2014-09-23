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
