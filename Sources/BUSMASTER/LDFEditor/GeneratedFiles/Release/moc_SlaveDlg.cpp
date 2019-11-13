/****************************************************************************
** Meta object code from reading C++ file 'SlaveDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SlaveDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SlaveDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SlaveDlg_t {
    QByteArrayData data[6];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SlaveDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SlaveDlg_t qt_meta_stringdata_SlaveDlg = {
    {
QT_MOC_LITERAL(0, 0, 8), // "SlaveDlg"
QT_MOC_LITERAL(1, 9, 26), // "onProtocolSelectionChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "onSelectionOk"
QT_MOC_LITERAL(4, 51, 18), // "onSelecetionCancel"
QT_MOC_LITERAL(5, 70, 18) // "onEditFaultSignals"

    },
    "SlaveDlg\0onProtocolSelectionChanged\0"
    "\0onSelectionOk\0onSelecetionCancel\0"
    "onEditFaultSignals"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlaveDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       3,    0,   37,    2, 0x0a /* Public */,
       4,    0,   38,    2, 0x0a /* Public */,
       5,    0,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SlaveDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SlaveDlg *_t = static_cast<SlaveDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onProtocolSelectionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onSelectionOk(); break;
        case 2: _t->onSelecetionCancel(); break;
        case 3: _t->onEditFaultSignals(); break;
        default: ;
        }
    }
}

const QMetaObject SlaveDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SlaveDlg.data,
      qt_meta_data_SlaveDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SlaveDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlaveDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SlaveDlg.stringdata0))
        return static_cast<void*>(const_cast< SlaveDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int SlaveDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
