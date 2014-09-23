#include "stobject.h"
#include "stparameter.h"
#include "stnode.h"

double normalizePhase(long double ph) {
	double sgn=1; 
	if (ph<0) sgn=-1;
	ph*=sgn; //make positive
	ph=ph-((long)(ph/360))*360;
	ph*=sgn;
	if (ph<0) ph+=360;
	return ph;
}


STObject::STObject() {
	m_object_type="STObject";
	m_name="";
	m_parent=0;
	m_is_modified=true;
}
	
SString STObject::name() {
	return m_name;
}
SString STObject::objectType() {
	return m_object_type;
}
STNode *STObject::parent() {
	return m_parent;
}

bool STObject::isParameter() {
	return false;
}
bool STObject::isNode() {
	return false;
}
bool STObject::isBlock() {
	return ((!isLoop())&&(parent())&&(parent()->isLoop()));
}
bool STObject::isLoop() {
	return false;
}

int STObject::parameterCount() {
	return 0;
}
STParameter *STObject::parameter(int index) {
	int dummy=index; dummy++;
	return 0;
}
int STObject::childCount() {
	return 0;
}
STNode *STObject::child(int index) {
	int dummy=index; dummy++;
	return 0;
}

bool STObject::isModified() {
	return m_is_modified;
}
void STObject::setModified(bool val) {
	if (m_is_modified==val) return;
	m_is_modified=val;
	if ((val)&&(parent())) m_parent->setModified(true);
	if (!val) {
		{for (int j=0; j<childCount(); j++) {
			child(j)->setModified(false);
		}}
		{for (int j=0; j<parameterCount(); j++) {
			parameter(j)->setModified(false);
		}}
	}
}
void STObject::setObjectType(SString str) {
	m_object_type=str;
}
void STObject::setName(SString name) {
	m_name=name;
}
void STObject::setParent(STNode *obj) {
	m_parent=obj;
}
