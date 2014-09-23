#include "basicphantoms.h"
#include "vsbasicphantoms.h"
#include <QtPlugin>

Q_EXPORT_PLUGIN2(basicphantoms,BasicPhantoms)

BasicPhantoms::BasicPhantoms() {
	VSPhantom *hold;
	
	hold=new VSPhantom; 
	hold->addSubphantom(new VSCylinderPhantom,"Cylinder");
	hold->setName("Cylinder");
	m_phantom_list << hold;
	
	hold=new VSPhantom; 
	hold->addSubphantom(new VSCubePhantom,"Cube");
	hold->setName("Cube");
	m_phantom_list << hold;
	
	hold=new VSPhantom; 
	hold->addSubphantom(new VSSpherePhantom,"Sphere");
	hold->setName("Sphere");
	m_phantom_list << hold;
	
	hold=new VSPhantom; 
	hold->addSubphantom(new VSAnnulusPhantom,"Annulus");
	hold->setName("Annulus");
	m_phantom_list << hold;
	
	hold=new VSPhantom; 
	hold->addSubphantom(new VSCubeWithHolePhantom,"Cube Minus Cylinder");
	hold->setName("Cube With Hole");
	m_phantom_list << hold;
	
}
BasicPhantoms::~BasicPhantoms() {
	qDeleteAll(m_phantom_list);
}
QList<VSPhantom *> BasicPhantoms::phantomList() {
	return m_phantom_list;
}


