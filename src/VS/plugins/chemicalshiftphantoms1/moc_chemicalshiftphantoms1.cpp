/****************************************************************************
** Meta object code from reading C++ file 'chemicalshiftphantoms1.h'
**
** Created: Wed May 22 14:49:21 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chemicalshiftphantoms1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chemicalshiftphantoms1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChemicalShiftPhantoms1[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_ChemicalShiftPhantoms1[] = {
    "ChemicalShiftPhantoms1\0"
};

const QMetaObject ChemicalShiftPhantoms1::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ChemicalShiftPhantoms1,
      qt_meta_data_ChemicalShiftPhantoms1, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChemicalShiftPhantoms1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChemicalShiftPhantoms1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChemicalShiftPhantoms1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChemicalShiftPhantoms1))
        return static_cast<void*>(const_cast< ChemicalShiftPhantoms1*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin"))
        return static_cast< VSPhantomPlugin*>(const_cast< ChemicalShiftPhantoms1*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin/3.23.2009"))
        return static_cast< VSPhantomPlugin*>(const_cast< ChemicalShiftPhantoms1*>(this));
    return QObject::qt_metacast(_clname);
}

int ChemicalShiftPhantoms1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ChemicalShiftPhantom1[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      29,   22, 0x06095103,
      44,   22, 0x06095103,
      59,   22, 0x06095103,
      77,   22, 0x06095103,
      95,   22, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_ChemicalShiftPhantom1[] = {
    "ChemicalShiftPhantom1\0double\0"
    "chemicalShift1\0chemicalShift2\0"
    "cylinderDiameterX\0cylinderDiameterY\0"
    "length\0"
};

const QMetaObject ChemicalShiftPhantom1::staticMetaObject = {
    { &VSPhantom::staticMetaObject, qt_meta_stringdata_ChemicalShiftPhantom1,
      qt_meta_data_ChemicalShiftPhantom1, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChemicalShiftPhantom1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChemicalShiftPhantom1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChemicalShiftPhantom1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChemicalShiftPhantom1))
        return static_cast<void*>(const_cast< ChemicalShiftPhantom1*>(this));
    return VSPhantom::qt_metacast(_clname);
}

int ChemicalShiftPhantom1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSPhantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = chemicalShift1(); break;
        case 1: *reinterpret_cast< double*>(_v) = chemicalShift2(); break;
        case 2: *reinterpret_cast< double*>(_v) = cylinderDiameterX(); break;
        case 3: *reinterpret_cast< double*>(_v) = cylinderDiameterY(); break;
        case 4: *reinterpret_cast< double*>(_v) = length(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setChemicalShift1(*reinterpret_cast< double*>(_v)); break;
        case 1: setChemicalShift2(*reinterpret_cast< double*>(_v)); break;
        case 2: setCylinderDiameterX(*reinterpret_cast< double*>(_v)); break;
        case 3: setCylinderDiameterY(*reinterpret_cast< double*>(_v)); break;
        case 4: setLength(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_ChemicalShiftPhantom2[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      29,   22, 0x06095103,
      44,   22, 0x06095103,
      59,   22, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_ChemicalShiftPhantom2[] = {
    "ChemicalShiftPhantom2\0double\0"
    "chemicalShift1\0chemicalShift2\0length\0"
};

const QMetaObject ChemicalShiftPhantom2::staticMetaObject = {
    { &VSPhantom::staticMetaObject, qt_meta_stringdata_ChemicalShiftPhantom2,
      qt_meta_data_ChemicalShiftPhantom2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChemicalShiftPhantom2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChemicalShiftPhantom2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChemicalShiftPhantom2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChemicalShiftPhantom2))
        return static_cast<void*>(const_cast< ChemicalShiftPhantom2*>(this));
    return VSPhantom::qt_metacast(_clname);
}

int ChemicalShiftPhantom2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSPhantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = chemicalShift1(); break;
        case 1: *reinterpret_cast< double*>(_v) = chemicalShift2(); break;
        case 2: *reinterpret_cast< double*>(_v) = length(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setChemicalShift1(*reinterpret_cast< double*>(_v)); break;
        case 1: setChemicalShift2(*reinterpret_cast< double*>(_v)); break;
        case 2: setLength(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
