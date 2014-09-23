/****************************************************************************
** Meta object code from reading C++ file 'vsbasicphantoms.h'
**
** Created: Wed May 22 14:49:25 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../shared/vsbasicphantoms.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vsbasicphantoms.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VSCubePhantom[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      22,   14, 0x0a095103,
      35,   29, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_VSCubePhantom[] = {
    "VSCubePhantom\0QString\0length\0float\0"
    "apodization\0"
};

const QMetaObject VSCubePhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_VSCubePhantom,
      qt_meta_data_VSCubePhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VSCubePhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VSCubePhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VSCubePhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VSCubePhantom))
        return static_cast<void*>(const_cast< VSCubePhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int VSCubePhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = length(); break;
        case 1: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLength(*reinterpret_cast< QString*>(_v)); break;
        case 1: setApodization(*reinterpret_cast< float*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_VSCylinderPhantom[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      24,   18, 0x87095103,
      34,   18, 0x87095103,
      44,   18, 0x87095103,
      51,   18, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_VSCylinderPhantom[] = {
    "VSCylinderPhantom\0float\0diameterX\0"
    "diameterY\0length\0apodization\0"
};

const QMetaObject VSCylinderPhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_VSCylinderPhantom,
      qt_meta_data_VSCylinderPhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VSCylinderPhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VSCylinderPhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VSCylinderPhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VSCylinderPhantom))
        return static_cast<void*>(const_cast< VSCylinderPhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int VSCylinderPhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = diameterX(); break;
        case 1: *reinterpret_cast< float*>(_v) = diameterY(); break;
        case 2: *reinterpret_cast< float*>(_v) = length(); break;
        case 3: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 1: setDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 2: setLength(*reinterpret_cast< float*>(_v)); break;
        case 3: setApodization(*reinterpret_cast< float*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_VSSpherePhantom[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      24,   16, 0x0a095103,
      39,   33, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_VSSpherePhantom[] = {
    "VSSpherePhantom\0QString\0diameter\0float\0"
    "apodization\0"
};

const QMetaObject VSSpherePhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_VSSpherePhantom,
      qt_meta_data_VSSpherePhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VSSpherePhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VSSpherePhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VSSpherePhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VSSpherePhantom))
        return static_cast<void*>(const_cast< VSSpherePhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int VSSpherePhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = diameter(); break;
        case 1: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDiameter(*reinterpret_cast< QString*>(_v)); break;
        case 1: setApodization(*reinterpret_cast< float*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_VSAnnulusPhantom[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      23,   17, 0x87095103,
      33,   17, 0x87095103,
      43,   17, 0x87095103,
      58,   17, 0x87095103,
      73,   17, 0x87095103,
      80,   17, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_VSAnnulusPhantom[] = {
    "VSAnnulusPhantom\0float\0diameterX\0"
    "diameterY\0innerDiameterX\0innerDiameterY\0"
    "length\0apodization\0"
};

const QMetaObject VSAnnulusPhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_VSAnnulusPhantom,
      qt_meta_data_VSAnnulusPhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VSAnnulusPhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VSAnnulusPhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VSAnnulusPhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VSAnnulusPhantom))
        return static_cast<void*>(const_cast< VSAnnulusPhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int VSAnnulusPhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = diameterX(); break;
        case 1: *reinterpret_cast< float*>(_v) = diameterY(); break;
        case 2: *reinterpret_cast< float*>(_v) = innerDiameterX(); break;
        case 3: *reinterpret_cast< float*>(_v) = innerDiameterY(); break;
        case 4: *reinterpret_cast< float*>(_v) = length(); break;
        case 5: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 1: setDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 2: setInnerDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 3: setInnerDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 4: setLength(*reinterpret_cast< float*>(_v)); break;
        case 5: setApodization(*reinterpret_cast< float*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_VSCubeWithHolePhantom[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       8,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      28,   22, 0x87095103,
      36,   22, 0x87095103,
      44,   22, 0x87095103,
      52,   22, 0x87095103,
      66,   22, 0x87095103,
      80,   22, 0x87095103,
      92,   22, 0x87095103,
     104,   22, 0x87095103,

       0        // eod
};

static const char qt_meta_stringdata_VSCubeWithHolePhantom[] = {
    "VSCubeWithHolePhantom\0float\0lengthX\0"
    "lengthY\0lengthZ\0holeDiameterX\0"
    "holeDiameterY\0holeOffsetX\0holeOffsetY\0"
    "apodization\0"
};

const QMetaObject VSCubeWithHolePhantom::staticMetaObject = {
    { &VSSubphantom::staticMetaObject, qt_meta_stringdata_VSCubeWithHolePhantom,
      qt_meta_data_VSCubeWithHolePhantom, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VSCubeWithHolePhantom::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VSCubeWithHolePhantom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VSCubeWithHolePhantom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VSCubeWithHolePhantom))
        return static_cast<void*>(const_cast< VSCubeWithHolePhantom*>(this));
    return VSSubphantom::qt_metacast(_clname);
}

int VSCubeWithHolePhantom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VSSubphantom::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = lengthX(); break;
        case 1: *reinterpret_cast< float*>(_v) = lengthY(); break;
        case 2: *reinterpret_cast< float*>(_v) = lengthZ(); break;
        case 3: *reinterpret_cast< float*>(_v) = holeDiameterX(); break;
        case 4: *reinterpret_cast< float*>(_v) = holeDiameterY(); break;
        case 5: *reinterpret_cast< float*>(_v) = holeOffsetX(); break;
        case 6: *reinterpret_cast< float*>(_v) = holeOffsetY(); break;
        case 7: *reinterpret_cast< float*>(_v) = apodization(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLengthX(*reinterpret_cast< float*>(_v)); break;
        case 1: setLengthY(*reinterpret_cast< float*>(_v)); break;
        case 2: setLengthZ(*reinterpret_cast< float*>(_v)); break;
        case 3: setHoleDiameterX(*reinterpret_cast< float*>(_v)); break;
        case 4: setHoleDiameterY(*reinterpret_cast< float*>(_v)); break;
        case 5: setHoleOffsetX(*reinterpret_cast< float*>(_v)); break;
        case 6: setHoleOffsetY(*reinterpret_cast< float*>(_v)); break;
        case 7: setApodization(*reinterpret_cast< float*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
