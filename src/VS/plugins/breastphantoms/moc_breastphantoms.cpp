/****************************************************************************
** Meta object code from reading C++ file 'breastphantoms.h'
**
** Created: Wed May 22 14:49:28 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "breastphantoms.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'breastphantoms.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BreastPhantoms[] = {

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

static const char qt_meta_stringdata_BreastPhantoms[] = {
    "BreastPhantoms\0"
};

const QMetaObject BreastPhantoms::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BreastPhantoms,
      qt_meta_data_BreastPhantoms, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BreastPhantoms::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BreastPhantoms::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BreastPhantoms::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BreastPhantoms))
        return static_cast<void*>(const_cast< BreastPhantoms*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin"))
        return static_cast< VSPhantomPlugin*>(const_cast< BreastPhantoms*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin/3.23.2009"))
        return static_cast< VSPhantomPlugin*>(const_cast< BreastPhantoms*>(this));
    return QObject::qt_metacast(_clname);
}

int BreastPhantoms::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SampledPhantom[] = {

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

static const char qt_meta_stringdata_SampledPhantom[] = {
    "SampledPhantom\0"
};

const QMetaObject SampledPhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_SampledPhantom,
      qt_meta_data_SampledPhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SampledPhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SampledPhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SampledPhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SampledPhantom))
        return static_cast<void*>(const_cast< SampledPhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int SampledPhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
