/****************************************************************************
** Meta object code from reading C++ file 'LDFDatabaseManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LDFDatabaseManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LDFDatabaseManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LDFDatabaseManager_t {
    QByteArrayData data[5];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LDFDatabaseManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LDFDatabaseManager_t qt_meta_stringdata_LDFDatabaseManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "LDFDatabaseManager"
QT_MOC_LITERAL(1, 19, 11), // "DataUpdated"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 18), // "DisplayModeChanged"
QT_MOC_LITERAL(4, 51, 16) // "DocumentModified"

    },
    "LDFDatabaseManager\0DataUpdated\0\0"
    "DisplayModeChanged\0DocumentModified"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LDFDatabaseManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LDFDatabaseManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LDFDatabaseManager *_t = static_cast<LDFDatabaseManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DataUpdated(); break;
        case 1: _t->DisplayModeChanged(); break;
        case 2: _t->DocumentModified(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LDFDatabaseManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LDFDatabaseManager::DataUpdated)) {
                *result = 0;
            }
        }
        {
            typedef void (LDFDatabaseManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LDFDatabaseManager::DisplayModeChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (LDFDatabaseManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LDFDatabaseManager::DocumentModified)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject LDFDatabaseManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LDFDatabaseManager.data,
      qt_meta_data_LDFDatabaseManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LDFDatabaseManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LDFDatabaseManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LDFDatabaseManager.stringdata0))
        return static_cast<void*>(const_cast< LDFDatabaseManager*>(this));
    return QObject::qt_metacast(_clname);
}

int LDFDatabaseManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void LDFDatabaseManager::DataUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void LDFDatabaseManager::DisplayModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void LDFDatabaseManager::DocumentModified()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
