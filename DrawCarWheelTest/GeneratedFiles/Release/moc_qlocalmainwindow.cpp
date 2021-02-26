/****************************************************************************
** Meta object code from reading C++ file 'qlocalmainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qlocalmainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlocalmainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QLocalMainWindow_t {
    QByteArrayData data[22];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLocalMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLocalMainWindow_t qt_meta_stringdata_QLocalMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "QLocalMainWindow"
QT_MOC_LITERAL(1, 17, 10), // "ProcessLog"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "info"
QT_MOC_LITERAL(4, 34, 7), // "present"
QT_MOC_LITERAL(5, 42, 8), // "ErrorLog"
QT_MOC_LITERAL(6, 51, 10), // "InitGLOver"
QT_MOC_LITERAL(7, 62, 14), // "PickSectionMsg"
QT_MOC_LITERAL(8, 77, 18), // "QVector<QVector2D>"
QT_MOC_LITERAL(9, 96, 20), // "SpreadWheelOpenGLMsg"
QT_MOC_LITERAL(10, 117, 21), // "Wheel::WheelSpreadMsg"
QT_MOC_LITERAL(11, 139, 3), // "msg"
QT_MOC_LITERAL(12, 143, 10), // "FullScreen"
QT_MOC_LITERAL(13, 154, 2), // "fs"
QT_MOC_LITERAL(14, 157, 13), // "SetPickRadian"
QT_MOC_LITERAL(15, 171, 6), // "radian"
QT_MOC_LITERAL(16, 178, 12), // "SetPickAngle"
QT_MOC_LITERAL(17, 191, 5), // "angle"
QT_MOC_LITERAL(18, 197, 13), // "EnableActions"
QT_MOC_LITERAL(19, 211, 18), // "full_screen_enable"
QT_MOC_LITERAL(20, 230, 18), // "updateActionRotate"
QT_MOC_LITERAL(21, 249, 1) // "t"

    },
    "QLocalMainWindow\0ProcessLog\0\0info\0"
    "present\0ErrorLog\0InitGLOver\0PickSectionMsg\0"
    "QVector<QVector2D>\0SpreadWheelOpenGLMsg\0"
    "Wheel::WheelSpreadMsg\0msg\0FullScreen\0"
    "fs\0SetPickRadian\0radian\0SetPickAngle\0"
    "angle\0EnableActions\0full_screen_enable\0"
    "updateActionRotate\0t"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLocalMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       5,    1,   69,    2, 0x06 /* Public */,
       6,    0,   72,    2, 0x06 /* Public */,
       7,    1,   73,    2, 0x06 /* Public */,
       9,    1,   76,    2, 0x06 /* Public */,
      12,    1,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,   82,    2, 0x08 /* Private */,
      16,    1,   85,    2, 0x08 /* Private */,
      18,    1,   88,    2, 0x08 /* Private */,
      20,    1,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Bool,   13,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,   15,
    QMetaType::Void, QMetaType::Float,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Bool,   21,

       0        // eod
};

void QLocalMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLocalMainWindow *_t = static_cast<QLocalMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ProcessLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->ErrorLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->InitGLOver(); break;
        case 3: _t->PickSectionMsg((*reinterpret_cast< QVector<QVector2D>(*)>(_a[1]))); break;
        case 4: _t->SpreadWheelOpenGLMsg((*reinterpret_cast< Wheel::WheelSpreadMsg(*)>(_a[1]))); break;
        case 5: _t->FullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->SetPickRadian((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->SetPickAngle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->EnableActions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->updateActionRotate((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector2D> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QLocalMainWindow::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalMainWindow::ProcessLog)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QLocalMainWindow::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalMainWindow::ErrorLog)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QLocalMainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalMainWindow::InitGLOver)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QLocalMainWindow::*_t)(QVector<QVector2D> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalMainWindow::PickSectionMsg)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QLocalMainWindow::*_t)(Wheel::WheelSpreadMsg );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalMainWindow::SpreadWheelOpenGLMsg)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (QLocalMainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalMainWindow::FullScreen)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject QLocalMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_QLocalMainWindow.data,
      qt_meta_data_QLocalMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLocalMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLocalMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLocalMainWindow.stringdata0))
        return static_cast<void*>(const_cast< QLocalMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int QLocalMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void QLocalMainWindow::ProcessLog(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QLocalMainWindow::ErrorLog(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QLocalMainWindow::InitGLOver()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void QLocalMainWindow::PickSectionMsg(QVector<QVector2D> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QLocalMainWindow::SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QLocalMainWindow::FullScreen(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
