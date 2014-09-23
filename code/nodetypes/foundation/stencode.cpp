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
