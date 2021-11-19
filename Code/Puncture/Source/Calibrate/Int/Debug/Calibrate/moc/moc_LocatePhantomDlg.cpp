/****************************************************************************
** Meta object code from reading C++ file 'LocatePhantomDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../LocatePhantomDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LocatePhantomDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LocatePhantomDlg_t {
    QByteArrayData data[8];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocatePhantomDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocatePhantomDlg_t qt_meta_stringdata_LocatePhantomDlg = {
    {
QT_MOC_LITERAL(0, 0, 16), // "LocatePhantomDlg"
QT_MOC_LITERAL(1, 17, 12), // "ChangeSensor"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 1), // "n"
QT_MOC_LITERAL(4, 33, 18), // "OnBtnRecordClicked"
QT_MOC_LITERAL(5, 52, 24), // "OnBtnRecordRepeatClicked"
QT_MOC_LITERAL(6, 77, 18), // "OnBtnDeleteClicked"
QT_MOC_LITERAL(7, 96, 16) // "OnBtnSaveClicked"

    },
    "LocatePhantomDlg\0ChangeSensor\0\0n\0"
    "OnBtnRecordClicked\0OnBtnRecordRepeatClicked\0"
    "OnBtnDeleteClicked\0OnBtnSaveClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocatePhantomDlg[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LocatePhantomDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LocatePhantomDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ChangeSensor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->OnBtnRecordClicked(); break;
        case 2: _t->OnBtnRecordRepeatClicked(); break;
        case 3: _t->OnBtnDeleteClicked(); break;
        case 4: _t->OnBtnSaveClicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LocatePhantomDlg::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_LocatePhantomDlg.data,
    qt_meta_data_LocatePhantomDlg,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LocatePhantomDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocatePhantomDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LocatePhantomDlg.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LocatePhantomDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
