/****************************************************************************
** Meta object code from reading C++ file 'basickspacefunctions.h'
**
** Created: Wed May 22 14:49:18 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../shared/basickspacefunctions.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'basickspacefunctions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KspaceCylinder[] = {

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
      21,   15, 0x87095103,
      31,   15, 0x87095103,
      41,   15, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_KspaceCylinder[] = {
    "KspaceCylinder\0float\0diameterX\0diameterY\0"
    "length\0"
};

const QMetaObject KspaceCylinder::staticMetaObject = {
    { &KspaceFunction::staticMetaObject, qt_meta_stringdata_KspaceCylinder,
      qt_meta_data_KspaceCylinder, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KspaceCylinder::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KspaceCylinder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KspaceCylinder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KspaceCylinder))
        return static_cast<void*>(const_cast< KspaceCylinder*>(this));
    return KspaceFunction::qt_metacast(_clname);
}

int KspaceCylinder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KspaceFunction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = diameterX(); break;
        case 1: *reinterpret_cast< float*>(_v) = diameterY(); break;
        case 2: *reinterpret_cast< float*>(_v) = length(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 1: setDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 2: setLength(*reinterpret_cast< float*>(_v)); break;
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
static const uint qt_meta_data_KspaceCube[] = {

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
      17,   11, 0x87095103,
      25,   11, 0x87095103,
      33,   11, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_KspaceCube[] = {
    "KspaceCube\0float\0lengthX\0lengthY\0"
    "lengthZ\0"
};

const QMetaObject KspaceCube::staticMetaObject = {
    { &KspaceFunction::staticMetaObject, qt_meta_stringdata_KspaceCube,
      qt_meta_data_KspaceCube, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KspaceCube::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KspaceCube::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KspaceCube::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KspaceCube))
        return static_cast<void*>(const_cast< KspaceCube*>(this));
    return KspaceFunction::qt_metacast(_clname);
}

int KspaceCube::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KspaceFunction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = lengthX(); break;
        case 1: *reinterpret_cast< float*>(_v) = lengthY(); break;
        case 2: *reinterpret_cast< float*>(_v) = lengthZ(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLengthX(*reinterpret_cast< float*>(_v)); break;
        case 1: setLengthY(*reinterpret_cast< float*>(_v)); break;
        case 2: setLengthZ(*reinterpret_cast< float*>(_v)); break;
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
static const uint qt_meta_data_KspaceSphere[] = {

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
      19,   13, 0x87095103,
      29,   13, 0x87095103,
      39,   13, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_KspaceSphere[] = {
    "KspaceSphere\0float\0diameterX\0diameterY\0"
    "diameterZ\0"
};

const QMetaObject KspaceSphere::staticMetaObject = {
    { &KspaceFunction::staticMetaObject, qt_meta_stringdata_KspaceSphere,
      qt_meta_data_KspaceSphere, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KspaceSphere::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KspaceSphere::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KspaceSphere::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KspaceSphere))
        return static_cast<void*>(const_cast< KspaceSphere*>(this));
    return KspaceFunction::qt_metacast(_clname);
}

int KspaceSphere::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KspaceFunction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = diameterX(); break;
        case 1: *reinterpret_cast< float*>(_v) = diameterY(); break;
        case 2: *reinterpret_cast< float*>(_v) = diameterZ(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 1: setDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 2: setDiameterZ(*reinterpret_cast< float*>(_v)); break;
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
