#include "contrastphantoms1.h"
#include "contrastphantoms1.h"
#include <QtPlugin>
#include <math.h>

Q_EXPORT_PLUGIN2(contrastphantoms1,ContrastPhantoms1)

ContrastPhantoms1::ContrastPhantoms1() {
	VSPhantom *hold;
	hold=new VSPhantom;
	hold->setName("Contrast Phantom 1");
	hold->addSubphantom(new ContrastPhantom1,"Contrast Phantom");
	m_phantom_list << hold;
}
ContrastPhantoms1::~ContrastPhantoms1() {
	qDeleteAll(m_phantom_list);
}
QList<VSPhantom *> ContrastPhantoms1::phantomList() {
	return m_phantom_list;
}

