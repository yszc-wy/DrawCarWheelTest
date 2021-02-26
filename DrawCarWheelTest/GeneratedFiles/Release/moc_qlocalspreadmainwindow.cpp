/****************************************************************************
** Meta object code from reading C++ file 'qlocalspreadmainwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qlocalspreadmainwindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlocalspreadmainwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QLocalSpreadMainWindow_t {
    QByteArrayData data[19];
    char stringdata0[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLocalSpreadMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLocalSpreadMainWindow_t qt_meta_stringdata_QLocalSpreadMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 22), // "QLocalSpreadMainWindow"
QT_MOC_LITERAL(1, 23, 17), // "PickSectionRadian"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 6), // "radian"
QT_MOC_LITERAL(4, 49, 16), // "PickSectionAngle"
QT_MOC_LITERAL(5, 66, 5), // "angle"
QT_MOC_LITERAL(6, 72, 10), // "FullScreen"
QT_MOC_LITERAL(7, 83, 2), // "fs"
QT_MOC_LITERAL(8, 86, 10), // "ProcessLog"
QT_MOC_LITERAL(9, 97, 4), // "info"
QT_MOC_LITERAL(10, 102, 7), // "present"
QT_MOC_LITERAL(11, 110, 8), // "ErrorLog"
QT_MOC_LITERAL(12, 119, 10), // "AddTestMsg"
QT_MOC_LITERAL(13, 130, 13), // "SetPickRadian"
QT_MOC_LITERAL(14, 144, 12), // "SetPickAngle"
QT_MOC_LITERAL(15, 157, 13), // "EnableActions"
QT_MOC_LITERAL(16, 171, 18), // "full_screen_enable"
QT_MOC_LITERAL(17, 190, 18), // "updateActionRotate"
QT_MOC_LITERAL(18, 209, 1) // "t"

    },
    "QLocalSpreadMainWindow\0PickSectionRadian\0"
    "\0radian\0PickSectionAngle\0angle\0"
    "FullScreen\0fs\0ProcessLog\0info\0present\0"
    "ErrorLog\0AddTestMsg\0SetPickRadian\0"
    "SetPickAngle\0EnableActions\0"
    "full_screen_enable\0updateActionRotate\0"
    "t"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLocalSpreadMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       6,    1,   70,    2, 0x06 /* Public */,
       8,    2,   73,    2, 0x06 /* Public */,
      11,    1,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   81,    2, 0x08 /* Private */,
      13,    1,   82,    2, 0x08 /* Private */,
      14,    1,   85,    2, 0x08 /* Private */,
      15,    1,   88,    2, 0x08 /* Private */,
      17,    1,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void, QMetaType::Bool,   18,

       0        // eod
};

void QLocalSpreadMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLocalSpreadMainWindow *_t = static_cast<QLocalSpreadMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PickSectionRadian((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->PickSectionAngle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->FullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->ProcessLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->ErrorLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->AddTestMsg(); break;
        case 6: _t->SetPickRadian((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->SetPickAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->EnableActions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->updateActionRotate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QLocalSpreadMainWindow::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadMainWindow::PickSectionRadian)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QLocalSpreadMainWindow::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadMainWindow::PickSectionAngle)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QLocalSpreadMainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadMainWindow::FullScreen)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QLocalSpreadMainWindow::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadMainWindow::ProcessLog)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QLocalSpreadMainWindow::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadMainWindow::ErrorLog)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject QLocalSpreadMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QLocalSpreadMainWindow.data,
      qt_meta_data_QLocalSpreadMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLocalSpreadMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLocalSpreadMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLocalSpreadMainWindow.stringdata0))
        return static_cast<void*>(const_cast< QLocalSpreadMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QLocalSpreadMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QLocalSpreadMainWindow::PickSectionRadian(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QLocalSpreadMainWindow::PickSectionAngle(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QLocalSpreadMainWindow::FullScreen(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QLocalSpreadMainWindow::ProcessLog(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QLocalSpreadMainWindow::ErrorLog(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
