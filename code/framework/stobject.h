#ifndef stobject_H
#define stobject_H

#include "sstring.h"
#include "slist.h"
#include <stdlib.h>

class STScanner;
class STParameter;
class STNode;

double ST_EXPORT normalizePhase(long double ph);

class ST_EXPORT STObject {
public:
	STObject();
	virtual ~STObject() {}
	
	SString name();
	SString objectType();
	STNode *parent();
	
	virtual bool isParameter();
	virtual bool isNode();
	virtual bool isLoop();
	
	bool isBlock(); //not a loop, but parent is a loop

	bool isModified();
	void setModified(bool val);
	
	virtual int parameterCount();
	virtual STParameter *parameter(int index);
	virtual int childCount();
	virtual STNode *child(int index);

	void setObjectType(SString str);
	void setName(SString name);
	void setParent(STNode *obj);
private:
	SString m_object_type;
	SString m_name;
	STNode *m_parent;
	bool m_is_modified;
};

#endif
