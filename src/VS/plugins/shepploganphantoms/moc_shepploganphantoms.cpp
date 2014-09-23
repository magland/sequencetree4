/****************************************************************************
** Meta object code from reading C++ file 'shepploganphantoms.h'
**
** Created: Wed May 22 14:49:24 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "shepploganphantoms.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shepploganphantoms.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SheppLoganPhantoms[] = {

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

static const char qt_meta_stringdata_SheppLoganPhantoms[] = {
    "SheppLoganPhantoms\0"
};

const QMetaObject SheppLoganPhantoms::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SheppLoganPhantoms,
      qt_meta_data_SheppLoganPhantoms, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SheppLoganPhantoms::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SheppLoganPhantoms::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SheppLoganPhantoms::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SheppLoganPhantoms))
        return static_cast<void*>(const_cast< SheppLoganPhantoms*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin"))
        return static_cast< VSPhantomPlugin*>(const_cast< SheppLoganPhantoms*>(this));
    if (!strcmp(_clname, "VSPhantomPlugin/3.23.2009"))
        return static_cast< VSPhantomPlugin*>(const_cast< SheppLoganPhantoms*>(this));
    return QObject::qt_metacast(_clname);
}

int SheppLoganPhantoms::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SheppLoganPhantom[] = {

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
      24,   18, 0x87095103,
      33,   18, 0x87095103,
      43,   18, 0x87095103,
      53,   18, 0x87095103,
      60,   18, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_SheppLoganPhantom[] = {
    "SheppLoganPhantom\0float\0contrast\0"
    "diameterX\0diameterY\0length\0apodization\0"
};

const QMetaObject SheppLoganPhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_SheppLoganPhantom,
      qt_meta_data_SheppLoganPhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SheppLoganPhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SheppLoganPhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SheppLoganPhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SheppLoganPhantom))
        return static_cast<void*>(const_cast< SheppLoganPhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int SheppLoganPhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = contrast(); break;
        case 1: *reinterpret_cast< float*>(_v) = diameterX(); break;
        case 2: *reinterpret_cast< float*>(_v) = diameterY(); break;
        case 3: *reinterpret_cast< float*>(_v) = length(); break;
        case 4: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setContrast(*reinterpret_cast< float*>(_v)); break;
        case 1: setDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 2: setDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 3: setLength(*reinterpret_cast< float*>(_v)); break;
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
