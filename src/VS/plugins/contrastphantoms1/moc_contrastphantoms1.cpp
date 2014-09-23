/****************************************************************************
** Meta object code from reading C++ file 'contrastphantoms1.h'
**
** Created: Wed May 22 14:49:13 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "contrastphantoms1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contrastphantoms1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ContrastPhantoms1[] = {

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

static const char qt_meta_stringdata_ContrastPhantoms1[] = {
    "ContrastPhantoms1\0"
};

const QMetaObject ContrastPhantoms1::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ContrastPhantoms1,
      qt_meta_data_ContrastPhantoms1, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ContrastPhantoms1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ContrastPhantoms1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ContrastPhantoms1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ContrastPhantoms1))
        return static_cast<void*>(const_cast< ContrastPhantoms1*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin"))
        return static_cast< VSPhantomPlugin*>(const_cast< ContrastPhantoms1*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin/3.23.2009"))
        return static_cast< VSPhantomPlugin*>(const_cast< ContrastPhantoms1*>(this));
    return QObject::qt_metacast(_clname);
}

int ContrastPhantoms1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ContrastPhantom1[] = {

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
      24,   17, 0x06095103,
      31,   17, 0x06095103,
      49,   41, 0x0a095103,
      56,   41, 0x0a095103,
      76,   70, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_ContrastPhantom1[] = {
    "ContrastPhantom1\0double\0period\0startTime\0"
    "QString\0length\0innerDiameter\0float\0"
    "apodization\0"
};

const QMetaObject ContrastPhantom1::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_ContrastPhantom1,
      qt_meta_data_ContrastPhantom1, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ContrastPhantom1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ContrastPhantom1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ContrastPhantom1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ContrastPhantom1))
        return static_cast<void*>(const_cast< ContrastPhantom1*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int ContrastPhantom1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = period(); break;
        case 1: *reinterpret_cast< double*>(_v) = startTime(); break;
        case 2: *reinterpret_cast< QString*>(_v) = length(); break;
        case 3: *reinterpret_cast< QString*>(_v) = innerDiameter(); break;
        case 4: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPeriod(*reinterpret_cast< double*>(_v)); break;
        case 1: setStartTime(*reinterpret_cast< double*>(_v)); break;
        case 2: setLength(*reinterpret_cast< QString*>(_v)); break;
        case 3: setInnerDiameter(*reinterpret_cast< QString*>(_v)); break;
        case 4: setApodization(*reinterpret_cast< float*>(_v)); break;
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
QT_END_MOC_NAMESPACE
