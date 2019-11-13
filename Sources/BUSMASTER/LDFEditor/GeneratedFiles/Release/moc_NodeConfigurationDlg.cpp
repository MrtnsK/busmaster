/****************************************************************************
** Meta object code from reading C++ file 'NodeConfigurationDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../NodeConfigurationDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NodeConfigurationDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NodeConfigurationDlg_t {
    QByteArrayData data[8];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NodeConfigurationDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NodeConfigurationDlg_t qt_meta_stringdata_NodeConfigurationDlg = {
    {
QT_MOC_LITERAL(0, 0, 20), // "NodeConfigurationDlg"
QT_MOC_LITERAL(1, 21, 13), // "OnEditFocused"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 13), // "onSelectionOk"
QT_MOC_LITERAL(4, 50, 16), // "onButtonClickAdd"
QT_MOC_LITERAL(5, 67, 19), // "onButtonClickDelete"
QT_MOC_LITERAL(6, 87, 21), // "onEditSelectionChange"
QT_MOC_LITERAL(7, 109, 18) // "onSelecetionCancel"

    },
    "NodeConfigurationDlg\0OnEditFocused\0\0"
    "onSelectionOk\0onButtonClickAdd\0"
    "onButtonClickDelete\0onEditSelectionChange\0"
    "onSelecetionCancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NodeConfigurationDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   45,    2, 0x08 /* Private */,
       4,    1,   46,    2, 0x08 /* Private */,
       5,    1,   49,    2, 0x08 /* Private */,
       6,    1,   52,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void NodeConfigurationDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NodeConfigurationDlg *_t = static_cast<NodeConfigurationDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnEditFocused(); break;
        case 1: _t->onSelectionOk(); break;
        case 2: _t->onButtonClickAdd((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onButtonClickDelete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onEditSelectionChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onSelecetionCancel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NodeConfigurationDlg::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NodeConfigurationDlg::OnEditFocused)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject NodeConfigurationDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_NodeConfigurationDlg.data,
      qt_meta_data_NodeConfigurationDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NodeConfigurationDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NodeConfigurationDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NodeConfigurationDlg.stringdata0))
        return static_cast<void*>(const_cast< NodeConfigurationDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int NodeConfigurationDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NodeConfigurationDlg::OnEditFocused()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
