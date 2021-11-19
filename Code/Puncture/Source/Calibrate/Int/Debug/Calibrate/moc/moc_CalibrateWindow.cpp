/****************************************************************************
** Meta object code from reading C++ file 'CalibrateWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../CalibrateWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalibrateWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalibrateWindow_t {
    QByteArrayData data[9];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrateWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrateWindow_t qt_meta_stringdata_CalibrateWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CalibrateWindow"
QT_MOC_LITERAL(1, 16, 22), // "OnBtnInitDeviceClicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 24), // "OnBtnStartCaptureClicked"
QT_MOC_LITERAL(4, 65, 20), // "OnBtnAnnotateClicked"
QT_MOC_LITERAL(5, 86, 25), // "OnBtnLocatePhantomClicked"
QT_MOC_LITERAL(6, 112, 13), // "ReleaseDevice"
QT_MOC_LITERAL(7, 126, 14), // "OnTimerDisplay"
QT_MOC_LITERAL(8, 141, 14) // "OnTimerCapture"

    },
    "CalibrateWindow\0OnBtnInitDeviceClicked\0"
    "\0OnBtnStartCaptureClicked\0"
    "OnBtnAnnotateClicked\0OnBtnLocatePhantomClicked\0"
    "ReleaseDevice\0OnTimerDisplay\0"
    "OnTimerCapture"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrateWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CalibrateWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrateWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->OnBtnInitDeviceClicked();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->OnBtnStartCaptureClicked(); break;
        case 2: _t->OnBtnAnnotateClicked(); break;
        case 3: _t->OnBtnLocatePhantomClicked(); break;
        case 4: _t->ReleaseDevice(); break;
        case 5: _t->OnTimerDisplay(); break;
        case 6: _t->OnTimerCapture(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrateWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_CalibrateWindow.data,
    qt_meta_data_CalibrateWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrateWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrateWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrateWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CalibrateWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
