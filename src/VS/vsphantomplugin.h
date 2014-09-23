#ifndef vsphantomplugin_H
#define vsphantomplugin_H

#include "vsphantom.h"

class VSPhantomPlugin {
public:
	virtual ~VSPhantomPlugin();
	virtual QList<VSPhantom *> phantomList();
};

Q_DECLARE_INTERFACE(VSPhantomPlugin,"VSPhantomPlugin/3.23.2009") 

#endif
