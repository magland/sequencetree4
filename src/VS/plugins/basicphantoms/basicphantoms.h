#ifndef basicphantoms_H
#define basicphantoms_H

#include "vsphantomplugin.h"

class BasicPhantoms : public QObject, public VSPhantomPlugin {
	Q_OBJECT
	Q_INTERFACES(VSPhantomPlugin)
public:
	BasicPhantoms();
	virtual ~BasicPhantoms();
	virtual QList<VSPhantom *> phantomList();
private:
	QList<VSPhantom *> m_phantom_list;
};



#endif
