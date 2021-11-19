/****************************************************************************
** Meta object code from reading C++ file 'AnnotateWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../AnnotateWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnnotateWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnnotateWindow_t {
    QByteArrayData data[13];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AnnotateWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AnnotateWindow_t qt_meta_stringdata_AnnotateWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AnnotateWindow"
QT_MOC_LITERAL(1, 15, 14), // "UpdateMousePos"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 1), // "x"
QT_MOC_LITERAL(4, 33, 1), // "y"
QT_MOC_LITERAL(5, 35, 26), // "OnBtnBtnSetImageDirClicked"
QT_MOC_LITERAL(6, 62, 23), // "OnBtnFormerImageClicked"
QT_MOC_LITERAL(7, 86, 21), // "OnBtnNextImageClicked"
QT_MOC_LITERAL(8, 108, 18), // "OnBtnFinishClicked"
QT_MOC_LITERAL(9, 127, 18), // "OnBtnSave2DClicked"
QT_MOC_LITERAL(10, 146, 25), // "OnBtnSave3DInImageClicked"
QT_MOC_LITERAL(11, 172, 27), // "OnBtnSave3DInPhantomClicked"
QT_MOC_LITERAL(12, 200, 25) // "OnBtnLoadPhantomTriangles"

    },
    "AnnotateWindow\0UpdateMousePos\0\0x\0y\0"
    "OnBtnBtnSetImageDirClicked\0"
    "OnBtnFormerImageClicked\0OnBtnNextImageClicked\0"
    "OnBtnFinishClicked\0OnBtnSave2DClicked\0"
    "OnBtnSave3DInImageClicked\0"
    "OnBtnSave3DInPhantomClicked\0"
    "OnBtnLoadPhantomTriangles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnnotateWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x0a /* Public */,
       5,    0,   64,    2, 0x08 /* Private */,
       6,    0,   65,    2, 0x08 /* Private */,
       7,    0,   66,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
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

void AnnotateWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AnnotateWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateMousePos((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->OnBtnBtnSetImageDirClicked(); break;
        case 2: _t->OnBtnFormerImageClicked(); break;
        case 3: _t->OnBtnNextImageClicked(); break;
        case 4: _t->OnBtnFinishClicked(); break;
        case 5: _t->OnBtnSave2DClicked(); break;
        case 6: _t->OnBtnSave3DInImageClicked(); break;
        case 7: _t->OnBtnSave3DInPhantomClicked(); break;
        case 8: _t->OnBtnLoadPhantomTriangles(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AnnotateWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_AnnotateWindow.data,
    qt_meta_data_AnnotateWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AnnotateWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnnotateWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnnotateWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AnnotateWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
