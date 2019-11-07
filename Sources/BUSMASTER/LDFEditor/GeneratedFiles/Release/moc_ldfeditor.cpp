/****************************************************************************
** Meta object code from reading C++ file 'ldfeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ldfeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ldfeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LDFEditor_t {
    QByteArrayData data[17];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LDFEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LDFEditor_t qt_meta_stringdata_LDFEditor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "LDFEditor"
QT_MOC_LITERAL(1, 10, 16), // "onActionFileOpen"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 16), // "onActionFileSave"
QT_MOC_LITERAL(4, 45, 18), // "onActionFileSaveAs"
QT_MOC_LITERAL(5, 64, 13), // "onActionAbout"
QT_MOC_LITERAL(6, 78, 12), // "onActionHelp"
QT_MOC_LITERAL(7, 91, 14), // "onActionLIN1_3"
QT_MOC_LITERAL(8, 106, 14), // "onActionLIN2_0"
QT_MOC_LITERAL(9, 121, 14), // "onActionLIN2_1"
QT_MOC_LITERAL(10, 136, 11), // "onActionHex"
QT_MOC_LITERAL(11, 148, 22), // "onActionOpenRecentFile"
QT_MOC_LITERAL(12, 171, 19), // "onActionPreviewFile"
QT_MOC_LITERAL(13, 191, 25), // "OnitemElementViewSelected"
QT_MOC_LITERAL(14, 217, 25), // "OnitemNetworkViewSelected"
QT_MOC_LITERAL(15, 243, 18), // "OnDocumentModified"
QT_MOC_LITERAL(16, 262, 10) // "onFileExit"

    },
    "LDFEditor\0onActionFileOpen\0\0"
    "onActionFileSave\0onActionFileSaveAs\0"
    "onActionAbout\0onActionHelp\0onActionLIN1_3\0"
    "onActionLIN2_0\0onActionLIN2_1\0onActionHex\0"
    "onActionOpenRecentFile\0onActionPreviewFile\0"
    "OnitemElementViewSelected\0"
    "OnitemNetworkViewSelected\0OnDocumentModified\0"
    "onFileExit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LDFEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x0a /* Public */,
       3,    0,   90,    2, 0x0a /* Public */,
       4,    0,   91,    2, 0x0a /* Public */,
       5,    0,   92,    2, 0x0a /* Public */,
       6,    0,   93,    2, 0x0a /* Public */,
       7,    0,   94,    2, 0x0a /* Public */,
       8,    0,   95,    2, 0x0a /* Public */,
       9,    0,   96,    2, 0x0a /* Public */,
      10,    0,   97,    2, 0x0a /* Public */,
      11,    0,   98,    2, 0x0a /* Public */,
      12,    0,   99,    2, 0x0a /* Public */,
      13,    0,  100,    2, 0x0a /* Public */,
      14,    0,  101,    2, 0x0a /* Public */,
      15,    0,  102,    2, 0x0a /* Public */,
      16,    0,  103,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,

       0        // eod
};

void LDFEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LDFEditor *_t = static_cast<LDFEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->onActionFileOpen();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->onActionFileSave();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->onActionFileSaveAs();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->onActionAbout();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->onActionHelp();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->onActionLIN1_3();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->onActionLIN2_0();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { int _r = _t->onActionLIN2_1();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->onActionHex();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->onActionOpenRecentFile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { int _r = _t->onActionPreviewFile();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 11: _t->OnitemElementViewSelected(); break;
        case 12: _t->OnitemNetworkViewSelected(); break;
        case 13: _t->OnDocumentModified(); break;
        case 14: { int _r = _t->onFileExit();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject LDFEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_LDFEditor.data,
      qt_meta_data_LDFEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LDFEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LDFEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LDFEditor.stringdata0))
        return static_cast<void*>(const_cast< LDFEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int LDFEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
