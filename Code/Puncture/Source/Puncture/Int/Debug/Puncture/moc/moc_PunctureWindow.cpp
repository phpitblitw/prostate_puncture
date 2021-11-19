/****************************************************************************
** Meta object code from reading C++ file 'PunctureWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../PunctureWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PunctureWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PunctureWindow_t {
    QByteArrayData data[16];
    char stringdata0[236];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PunctureWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PunctureWindow_t qt_meta_stringdata_PunctureWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "PunctureWindow"
QT_MOC_LITERAL(1, 15, 14), // "OnTimerTimeout"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 10), // "InitDevice"
QT_MOC_LITERAL(4, 42, 15), // "LoadPatientData"
QT_MOC_LITERAL(5, 58, 20), // "OnBtnRegisterClicked"
QT_MOC_LITERAL(6, 79, 25), // "OnBtnResetRegisterClicked"
QT_MOC_LITERAL(7, 105, 20), // "OnBtnUpdateUSClicked"
QT_MOC_LITERAL(8, 126, 4), // "Quit"
QT_MOC_LITERAL(9, 131, 11), // "MoveMRILeft"
QT_MOC_LITERAL(10, 143, 12), // "MoveMRIRight"
QT_MOC_LITERAL(11, 156, 9), // "MoveMRIUp"
QT_MOC_LITERAL(12, 166, 11), // "MoveMRIDown"
QT_MOC_LITERAL(13, 178, 14), // "MoveMRIForward"
QT_MOC_LITERAL(14, 193, 15), // "MoveMRIBackward"
QT_MOC_LITERAL(15, 209, 26) // "OnRadioBtnSaveDebugChanged"

    },
    "PunctureWindow\0OnTimerTimeout\0\0"
    "InitDevice\0LoadPatientData\0"
    "OnBtnRegisterClicked\0OnBtnResetRegisterClicked\0"
    "OnBtnUpdateUSClicked\0Quit\0MoveMRILeft\0"
    "MoveMRIRight\0MoveMRIUp\0MoveMRIDown\0"
    "MoveMRIForward\0MoveMRIBackward\0"
    "OnRadioBtnSaveDebugChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PunctureWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x0a /* Public */,
       4,    0,   86,    2, 0x0a /* Public */,
       5,    0,   87,    2, 0x0a /* Public */,
       6,    0,   88,    2, 0x0a /* Public */,
       7,    0,   89,    2, 0x0a /* Public */,
       8,    0,   90,    2, 0x0a /* Public */,
       9,    0,   91,    2, 0x0a /* Public */,
      10,    0,   92,    2, 0x0a /* Public */,
      11,    0,   93,    2, 0x0a /* Public */,
      12,    0,   94,    2, 0x0a /* Public */,
      13,    0,   95,    2, 0x0a /* Public */,
      14,    0,   96,    2, 0x0a /* Public */,
      15,    0,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PunctureWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PunctureWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnTimerTimeout(); break;
        case 1: { int _r = _t->InitDevice();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->LoadPatientData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->OnBtnRegisterClicked(); break;
        case 4: _t->OnBtnResetRegisterClicked(); break;
        case 5: _t->OnBtnUpdateUSClicked(); break;
        case 6: _t->Quit(); break;
        case 7: _t->MoveMRILeft(); break;
        case 8: _t->MoveMRIRight(); break;
        case 9: _t->MoveMRIUp(); break;
        case 10: _t->MoveMRIDown(); break;
        case 11: _t->MoveMRIForward(); break;
        case 12: _t->MoveMRIBackward(); break;
        case 13: _t->OnRadioBtnSaveDebugChanged(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PunctureWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_PunctureWindow.data,
    qt_meta_data_PunctureWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PunctureWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PunctureWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PunctureWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int PunctureWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
