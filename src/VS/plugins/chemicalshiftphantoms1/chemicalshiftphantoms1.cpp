#include "chemicalshiftphantoms1.h"
#include "chemicalshiftphantoms1.h"
#include <QtPlugin>
#include <math.h>

Q_EXPORT_PLUGIN2(chemicalshiftphantoms1,ChemicalShiftPhantoms1)

ChemicalShiftPhantoms1::ChemicalShiftPhantoms1() {
	m_phantom_list << new ChemicalShiftPhantom1;
	m_phantom_list << new ChemicalShiftPhantom2;
}
ChemicalShiftPhantoms1::~ChemicalShiftPhantoms1() {
	qDeleteAll(m_phantom_list);
}
QList<VSPhantom *> ChemicalShiftPhantoms1::phantomList() {
	return m_phantom_list;
}


ChemicalShiftPhantom1::ChemicalShiftPhantom1() {
	setName("Chemical Shift 1");
	m_cube=new VSCubeWithHolePhantom;
	m_cylinder=new VSCylinderPhantom;
	addSubphantom(m_cube,"Cube");
	addSubphantom(m_cylinder,"Cylinder");
	setChemicalShift1(0);
	setChemicalShift2(0);
	setCylinderDiameterX(40);
	setCylinderDiameterY(40);
	setLength(50);
}
ChemicalShiftPhantom1::~ChemicalShiftPhantom1() {
}

Complex ChemicalShiftPhantom1::evaluate(double kx,double ky,double kz,double tau) {
	return VSPhantom::evaluate(kx,ky,kz,tau);
}

ChemicalShiftPhantom2::ChemicalShiftPhantom2() {
	setName("Chemical Shift 2");
	m_cube=new VSCubeWithHolePhantom;
	m_cylinder=new VSCylinderPhantom;
	addSubphantom(m_cube,"Cube");
	addSubphantom(m_cylinder,"Cylinder");
	setChemicalShift1(0);
	setChemicalShift2(0);
	setLength(50);
}
ChemicalShiftPhantom2::~ChemicalShiftPhantom2() {
}

Complex ChemicalShiftPhantom2::evaluate(double kx,double ky,double kz,double tau) {
	return VSPhantom::evaluate(kx,ky,kz,tau);
}

