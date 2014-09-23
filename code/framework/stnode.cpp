

/* BEGIN EXCLUDE */
#include "stnode.h"
#include "stsequence.h"
#include <stdio.h>
/* END EXCLUDE */

STNode::STNode() {
	ST_CLASS(STNode,STObject)
	m_scanner=0;
	m_sequence=0;
	m_reference_child_index=0;
	m_relative_start_time=0;
	m_tree_index=-2;
	/* ST_PARAMETERS */
	/* ST_CHILDREN */
}

STNode::~STNode() {
	clearChildren();
}

STScanner *STNode::scanner() {
	return m_scanner;
}

void STNode::setScanner(STScanner *S) {
	m_scanner=S;
	for (int j=0; j<childCount(); j++)
		child(j)->setScanner(S);
}

STSequence *STNode::sequence() {
	return m_sequence;
}

void STNode::setSequence(STSequence *S) {
	m_sequence=S;
	for (int j=0; j<childCount(); j++)
		child(j)->setSequence(S);
}

	
int STNode::parameterCount() {
	return m_parameters.count(); 
}
STParameter *STNode::parameter(int index) {
	return m_parameters[index];
}
void STNode::addParameter(STParameter *ptr,SString name) {
	ptr->setName(name);
	m_parameters.append(ptr);
	ptr->setParent(this);
}
void STNode::removeParameter(int index) {
	m_parameters[index]->setParent(0);
	m_parameters.removeAt(index);
}
	
int STNode::childCount() {
	return m_children.count();
}
STNode *STNode::child(int index) {
	return m_children[index];
}
void STNode::addChild(STNode *obj) {
	m_children.append(obj);
	obj->setScanner(m_scanner);
	obj->setSequence(m_sequence);
	obj->setParent(this);
}
void STNode::removeChild(int index) {
	m_children[index]->setParent(0);
	m_children.removeAt(index);
}

double STNode::startTime() {
	if ((parent())&&(!parent()->isLoop())) 
		return parent()->startTime()+m_relative_start_time;
	return m_relative_start_time;
}
double STNode::relativeStartTime() {
	return m_relative_start_time;
}
void STNode::setRelativeStartTime(double t) {
	m_relative_start_time=t;
}
int STNode::referenceChildIndex() {
	return m_reference_child_index;
}
void STNode::setReferenceChildIndex(int index) {
	m_reference_child_index=index;
}
	
bool STNode::initialize() {
	for (int j=0; j<childCount(); j++)
		if (!child(j)->initialize()) return false;
	return true;
}
bool STNode::prepare() {
	if (!prepareModifiedChildren()) return false;
	setModified(false);
	return true;
}
bool STNode::run() {
	if (!scanner()) return false;
	if (!sequence()) return false;
	if (sequence()->statisticsMode()) {
		if (isBlock()) {
			sequence()->incrementStatBlockCount(1);
			sequence()->incrementStatDuration(duration());
			sequence()->incrementStatSAR(SAR());
		}
		return true;
	}
	bool ret=true;
	if (isModified()) if (!prepare()) ret=false;
	if (isBlock()) {
		if (!sequence()->checkButDontRunMode()) {
			scanner()->beginBlock(duration());
		}
	}
	if ((!isBlock())||(scanner()->inBlockRange())) {
		for (int j=0; j<childCount(); j++)
			if (!child(j)->run()) ret=false;
	}
	if (isBlock()) {
		if (!sequence()->checkButDontRunMode()) {
			scanner()->endBlock(this);
		}
	}
	return ret;
}
double STNode::SAR() {
	double ret=0;
	for (int j=0; j<childCount(); j++) {
		ret+=child(j)->SAR();
	}
	return ret;
}
double STNode::duration() {
	return 0;
}

double STNode::referenceTime() {
	if (m_reference_child_index<0) return 0;
	if (m_reference_child_index>=m_children.count()) return 0;
	return m_children[m_reference_child_index]->relativeStartTime()+m_children[m_reference_child_index]->referenceTime();
}

bool STNode::prepareModifiedChildren() {
	for (int j=0; j<childCount(); j++) {
		if (child(j)->isModified()) {
			if (!child(j)->prepare()) return false;
		}
	}
	return true;
}

Vector3 STNode::initialMoment() {
	return m_initial_moment;
}
void STNode::setInitialMoment(Vector3 X) {
	if (X!=m_initial_moment) {
		m_initial_moment=X;
		setModified(true);
	}
}
Vector3 STNode::initialTotalMoment() {
	return m_initial_total_moment;
}
void STNode::setInitialTotalMoment(Vector3 X) {
	if (X!=m_initial_total_moment) {
		m_initial_total_moment=X;
		setModified(true);
	}
}

Vector3 STNode::terminalMoment() {
	return m_initial_moment+totalGradientMoment();
}
Vector3 STNode::totalGradientMoment() {
	Vector3 ret;
	for (int j=0; j<childCount(); j++) {
		ret=ret+child(j)->totalGradientMoment();
	}
	return ret;
}

void STNode::reportError(SString err) {
	if (scanner()) scanner()->reportError(err);
}

void STNode::clearChildren() {
	for (int j=0; j<childCount(); j++)
		delete child(j);
	m_children.clear();
}

void STNode::resetHasBeenSet() {
	{for (int j=0; j<parameterCount(); j++)
		parameter(j)->setHasBeenSet(false);}
	{for (int j=0; j<childCount(); j++)
		child(j)->resetHasBeenSet();}
}

void STNode::setAllModified() {
	setModified(true);
	{for (int j=0; j<parameterCount(); j++)
		parameter(j)->setModified(true);}
	{for (int j=0; j<childCount(); j++)
		child(j)->setAllModified();}
	
}

STNode *STNode::child(SString child_name) {
	for (int j=0; j<childCount(); j++)
		if (child(j)->name()==child_name)
			return child(j);
	return 0;
}
void STNode::setParameterFromString(SString param_name,SString value) {
	for (int j=0; j<parameterCount(); j++)
		if (parameter(j)->name()==param_name) {
			parameter(j)->setFromString(value);
			return;
		}		
}

STNode *STNode::replaceChild(STNode *oldchild,STNode *newchild) {
	for (int j=0; j<childCount(); j++) {
		if (child(j)==oldchild) {
			m_children[j]=newchild;
			newchild->setParent(this);
			newchild->setScanner(m_scanner);
			newchild->setSequence(m_sequence);
			newchild->setName(oldchild->name());
			delete oldchild;
			return m_children[j];
		}
	}
	return oldchild;
}

Vector3 STNode::gradientStartTimes() {
	double dur=duration();
	Vector3 ret(dur,dur,dur);
	for (int j=0; j<childCount(); j++) {
		Vector3 hold=child(j)->gradientStartTimes();
		double reltime=child(j)->relativeStartTime();
		if (hold.x()+reltime<ret.x()) ret.setX(hold.x()+reltime);
		if (hold.y()+reltime<ret.y()) ret.setY(hold.y()+reltime);
		if (hold.z()+reltime<ret.z()) ret.setZ(hold.z()+reltime);
	}
	return ret;
}
Vector3 STNode::gradientEndTimes() {
	Vector3 ret(0,0,0);
	for (int j=0; j<childCount(); j++) {
		Vector3 hold=child(j)->gradientEndTimes();
		double reltime=child(j)->relativeStartTime();
		if (hold.x()+reltime > ret.x()) ret.setX(hold.x()+reltime);
		if (hold.y()+reltime > ret.y()) ret.setY(hold.y()+reltime);
		if (hold.z()+reltime > ret.z()) ret.setZ(hold.z()+reltime);
	}
	return ret;
}

void STNode::setTreeIndices(int &ind) {
	m_tree_index=ind;
	ind++;
	for (int j=0; j<childCount(); j++)
		child(j)->setTreeIndices(ind);
}

void STNode::setADCIndices(int &indx) {
	for (int j=0; j<childCount(); j++)
		child(j)->setADCIndices(indx);
}

void STNode::setReadoutIndex(int iterator_index,int indx) {
	for (int j=0; j<childCount(); j++) 
		child(j)->setReadoutIndex(iterator_index,indx);
}

