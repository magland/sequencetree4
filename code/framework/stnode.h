
/* BEGIN EXCLUDE */
#ifndef stnode_H
#define stnode_H

#include "stobject.h"
#include "stparameter.h"
#include "stscanner.h"
#include <stdio.h>
/* END EXCLUDE */
class STSequence;

#define ST_CHILD(_type_,_name_) \
	_name_=new _type_; \
	_name_->setName(# _name_); \
	addChild(_name_);
#define ST_PARAMETER(_type_,_name_,_defaultval_,_units_) \
	addParameter(&_name_,# _name_); \
	_name_.setFromString(# _defaultval_);
#define ST_ALIGN(_name_,_alignment_,_timing_,_relativeindex_) \
	alignChild(_name_,_alignment_,_timing_,_relativeindex_);
#define ST_CLASS(_name_,_base_name_) \
	setObjectType(# _name_);
#define ST_DEFAULT(_name_,_value_)

class ST_EXPORT STNode : public STObject {
public:
	STNode();
	virtual ~STNode();
	bool isNode() {return true;}
	
	STScanner *scanner();
	void setScanner(STScanner *S);
	
	STSequence *sequence();
	void setSequence(STSequence *S);
	
	int parameterCount();
	STParameter *parameter(int index);
	void addParameter(STParameter *,SString name);
	void removeParameter(int index);
	
	int childCount();
	STNode *child(int index);
	virtual void addChild(STNode *obj);
	virtual void removeChild(int index);
	STNode *child(SString child_name);
	STNode *replaceChild(STNode *oldchild,STNode *newchild);
	void setParameterFromString(SString param_name,SString value);
	
	int treeIndex() {return m_tree_index;}
	void setTreeIndices(int &ind);
	
	double startTime();
	double relativeStartTime();
	void setRelativeStartTime(double t); 
	int referenceChildIndex();
	void setReferenceChildIndex(int index);
	
	Vector3 initialMoment();
	void setInitialMoment(Vector3 X);
	Vector3 initialTotalMoment();
	void setInitialTotalMoment(Vector3 X);
	
	
	virtual bool initialize();
	virtual bool prepare();
	virtual bool run();
	virtual double SAR(); //by default, sum of SAR of children
	virtual double duration();
	virtual Vector3 gradientStartTimes(); //holds the effective start times for the gradients (or RF or readout), for purpose of computing overlaps
	virtual Vector3 gradientEndTimes();
	virtual Vector3 terminalMoment(); //must be computed from initial moment, by default it is initialMoment() plus the sum of terminalMoment() for all children
	virtual Vector3 totalGradientMoment();
	virtual double referenceTime(); //relative to startTime(), by default, it is defined according to referenceChildIndex
	virtual void setADCIndices(int &indx);
	virtual void setReadoutIndex(int iterator_index,int indx);
	
	void clearChildren();
	void reportError(SString err);
	bool prepareModifiedChildren();
	
	void resetHasBeenSet();
	void setAllModified();
protected:
	SList<STParameter *> m_parameters;
private:
	SList<STNode *> m_children;
	STScanner *m_scanner;
	STSequence *m_sequence;
	double m_relative_start_time;
	int m_reference_child_index;
	Vector3 m_initial_moment;
	Vector3 m_initial_total_moment;
	int m_tree_index;
};

/* BEGIN EXCLUDE */
#endif
/* END EXCLUDE */
