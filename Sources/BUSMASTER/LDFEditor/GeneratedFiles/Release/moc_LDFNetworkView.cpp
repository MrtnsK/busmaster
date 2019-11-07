/****************************************************************************
** Meta object code from reading C++ file 'LDFNetworkView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LDFNetworkView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LDFNetworkView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LDFNetworkView_t {
    QByteArrayData data[6];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LDFNetworkView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LDFNetworkView_t qt_meta_stringdata_LDFNetworkView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "LDFNetworkView"
QT_MOC_LITERAL(1, 15, 12), // "onDataUpdate"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "OnElementCreated"
QT_MOC_LITERAL(4, 46, 11), // "UID_ELEMENT"
QT_MOC_LITERAL(5, 58, 16) // "OnElementDeleted"

    },
    "LDFNetworkView\0onDataUpdate\0\0"
    "OnElementCreated\0UID_ELEMENT\0"
    "OnElementDeleted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LDFNetworkView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    1,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 4,    2,

       0        // eod
};

void LDFNetworkView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LDFNetworkView *_t = static_cast<LDFNetworkView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDataUpdate(); break;
        case 1: _t->OnElementCreated((*reinterpret_cast< UID_ELEMENT(*)>(_a[1]))); break;
        case 2: _t->OnElementDeleted((*reinterpret_cast< UID_ELEMENT(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject LDFNetworkView::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_LDFNetworkView.data,
      qt_meta_data_LDFNetworkView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LDFNetworkView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LDFNetworkView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LDFNetworkView.stringdata0))
        return static_cast<void*>(const_cast< LDFNetworkView*>(this));
    if (!strcmp(_clname, "INotifyClusterChange"))
        return static_cast< INotifyClusterChange*>(const_cast< LDFNetworkView*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int LDFNetworkView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
