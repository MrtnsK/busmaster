/****************************************************************************
** Meta object code from reading C++ file 'CodingDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CodingDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CodingDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CodingDlg_t {
    QByteArrayData data[13];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CodingDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CodingDlg_t qt_meta_stringdata_CodingDlg = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CodingDlg"
QT_MOC_LITERAL(1, 10, 13), // "AddLogicalRow"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "AddPhysicalRow"
QT_MOC_LITERAL(4, 40, 16), // "DeleteLogicalRow"
QT_MOC_LITERAL(5, 57, 17), // "DeletePhysicalRow"
QT_MOC_LITERAL(6, 75, 13), // "onSelectionOk"
QT_MOC_LITERAL(7, 89, 18), // "onSelecetionCancel"
QT_MOC_LITERAL(8, 108, 26), // "ItemSelectionChangeLogical"
QT_MOC_LITERAL(9, 135, 17), // "FillLogicalValues"
QT_MOC_LITERAL(10, 153, 16), // "LINCompuMethods&"
QT_MOC_LITERAL(11, 170, 12), // "ouCompuProps"
QT_MOC_LITERAL(12, 183, 18) // "FillPhysicalValues"

    },
    "CodingDlg\0AddLogicalRow\0\0AddPhysicalRow\0"
    "DeleteLogicalRow\0DeletePhysicalRow\0"
    "onSelectionOk\0onSelecetionCancel\0"
    "ItemSelectionChangeLogical\0FillLogicalValues\0"
    "LINCompuMethods&\0ouCompuProps\0"
    "FillPhysicalValues"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CodingDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    1,   66,    2, 0x08 /* Private */,
      12,    1,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Int, 0x80000000 | 10,   11,

       0        // eod
};

void CodingDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CodingDlg *_t = static_cast<CodingDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->AddLogicalRow(); break;
        case 1: _t->AddPhysicalRow(); break;
        case 2: _t->DeleteLogicalRow(); break;
        case 3: _t->DeletePhysicalRow(); break;
        case 4: _t->onSelectionOk(); break;
        case 5: _t->onSelecetionCancel(); break;
        case 6: _t->ItemSelectionChangeLogical(); break;
        case 7: _t->FillLogicalValues((*reinterpret_cast< LINCompuMethods(*)>(_a[1]))); break;
        case 8: { int _r = _t->FillPhysicalValues((*reinterpret_cast< LINCompuMethods(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject CodingDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CodingDlg.data,
      qt_meta_data_CodingDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CodingDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CodingDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CodingDlg.stringdata0))
        return static_cast<void*>(const_cast< CodingDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CodingDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
