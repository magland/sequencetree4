#ifndef vsobjectstate_H
#define vsobjectstate_H

#include "vsobjecttransformation.h"
#include <QObject>

//the state of the object, in terms of what has happened in the past
class VSObjectState : public QObject {
public:
	VSObjectState() {}
	virtual ~VSObjectState() {}
	virtual void applyTransformation(const VSObjectTransformation &T) {Q_UNUSED(T)}
	virtual VSObjectTransformation transformation() {VSObjectTransformation dummy; return dummy;}
};

#endif
