#ifndef chemicalshiftphantoms1_H
#define chemicalshiftphantoms1_H

#include "vsphantomplugin.h"
#include "vsbasicphantoms.h"

class ChemicalShiftPhantoms1 : public QObject, public VSPhantomPlugin {
	Q_OBJECT
	Q_INTERFACES(VSPhantomPlugin)
public:
	ChemicalShiftPhantoms1();
	virtual ~ChemicalShiftPhantoms1();
	virtual QList<VSPhantom *> phantomList();
private:
	QList<VSPhantom *> m_phantom_list;
};

class ChemicalShiftPhantom1 : public VSPhantom {
	Q_OBJECT
	Q_PROPERTY(double chemicalShift1 READ chemicalShift1 WRITE setChemicalShift1)
	Q_PROPERTY(double chemicalShift2 READ chemicalShift2 WRITE setChemicalShift2)
	Q_PROPERTY(double cylinderDiameterX READ cylinderDiameterX WRITE setCylinderDiameterX)
	Q_PROPERTY(double cylinderDiameterY READ cylinderDiameterY WRITE setCylinderDiameterY)
	Q_PROPERTY(double length READ length WRITE setLength)
public:
	ChemicalShiftPhantom1();
	virtual ~ChemicalShiftPhantom1();
	virtual Complex evaluate(double kx,double ky,double kz,double tau);
	
	double chemicalShift1() {return m_cube->chemicalShift();}
	double chemicalShift2() {return m_cylinder->chemicalShift();}
	double cylinderDiameterX() {return m_cylinder->diameterX();}
	double cylinderDiameterY() {return m_cylinder->diameterY();}
	double length() {return m_cylinder->length();}
	void setChemicalShift1(double val) {m_cube->setChemicalShift(val);}
	void setChemicalShift2(double val) {m_cylinder->setChemicalShift(val);}	
	void setCylinderDiameterX(double val) {m_cylinder->setDiameterX(val); m_cube->setHoleDiameterX(val); m_cube->setHoleOffsetX(0);}	
	void setCylinderDiameterY(double val) {m_cylinder->setDiameterY(val); m_cube->setHoleDiameterY(val); m_cube->setHoleOffsetY(0);}	
	void setLength(double val) {m_cube->setLengthX(val); m_cylinder->setLength(val);}
private:
	VSCubeWithHolePhantom *m_cube;
	VSCylinderPhantom *m_cylinder;
};

class ChemicalShiftPhantom2 : public VSPhantom {
	Q_OBJECT
	Q_PROPERTY(double chemicalShift1 READ chemicalShift1 WRITE setChemicalShift1)
	Q_PROPERTY(double chemicalShift2 READ chemicalShift2 WRITE setChemicalShift2)
	Q_PROPERTY(double length READ length WRITE setLength)
public:
	ChemicalShiftPhantom2();
	virtual ~ChemicalShiftPhantom2();
	virtual Complex evaluate(double kx,double ky,double kz,double tau);
	
	double chemicalShift1() {return m_cube->chemicalShift();}
	double chemicalShift2() {return m_cylinder->chemicalShift();}
	double cylinderDiameterX() {return m_cylinder->diameterX();}
	double cylinderDiameterY() {return m_cylinder->diameterY();}
	double length() {return m_cylinder->length();}
	void setChemicalShift1(double val) {m_cube->setChemicalShift(val);}
	void setChemicalShift2(double val) {m_cylinder->setChemicalShift(val);}	
	void setLength(double val) {m_cube->setLengthX(val); m_cylinder->setLength(val);}
private:
	VSCubeWithHolePhantom *m_cube;
	VSCylinderPhantom *m_cylinder;
};


#endif
