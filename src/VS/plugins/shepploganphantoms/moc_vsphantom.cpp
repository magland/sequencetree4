/****************************************************************************
** Meta object code from reading C++ file 'vsphantom.h'
**
** Created: Wed May 22 14:49:25 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vsphantom.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vsphantom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VSSubphantom[] = {

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
      20,   13, 0x06095103,
      23,   13, 0x06095103,
      26,   13, 0x06095103,

       0        // eod
};

static const char qt_meta_stringdata_VSSubphantom[] = {
    "VSSubphantom\0double\0T1\0T2\0chemicalShift\0"
};

const QMetaObject VSSubphantom::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VSSubphantom,
      qt_meta_data_VSSubphantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VSSubphantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VSSubphantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VSSubphantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VSSubphantom))
        return static_cast<void*>(const_cast< VSSubphantom*>(this));
    return QObject::qt_metacast(_clname);
}

int VSSubphantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = T1(); break;
        case 1: *reinterpret_cast< double*>(_v) = T2(); break;
        case 2: *reinterpret_cast< double*>(_v) = chemicalShift(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setT1(*reinterpret_cast< double*>(_v)); break;
        case 1: setT2(*reinterpret_cast< double*>(_v)); break;
        case 2: setChemicalShift(*reinterpret_cast< double*>(_v)); break;
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
