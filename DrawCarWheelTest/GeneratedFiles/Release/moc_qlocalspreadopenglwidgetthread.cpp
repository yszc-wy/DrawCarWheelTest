/****************************************************************************
** Meta object code from reading C++ file 'qlocalspreadopenglwidgetthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qlocalspreadopenglwidgetthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlocalspreadopenglwidgetthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QLocalSpreadOpenGLWidgetThread_t {
    QByteArrayData data[22];
    char stringdata0[354];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLocalSpreadOpenGLWidgetThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLocalSpreadOpenGLWidgetThread_t qt_meta_stringdata_QLocalSpreadOpenGLWidgetThread = {
    {
QT_MOC_LITERAL(0, 0, 30), // "QLocalSpreadOpenGLWidgetThread"
QT_MOC_LITERAL(1, 31, 11), // "ProgressLog"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 4), // "info"
QT_MOC_LITERAL(4, 49, 7), // "present"
QT_MOC_LITERAL(5, 57, 8), // "ErrorLog"
QT_MOC_LITERAL(6, 66, 6), // "Result"
QT_MOC_LITERAL(7, 73, 8), // "wheel_id"
QT_MOC_LITERAL(8, 82, 21), // "Wheel::WheelSpreadMsg"
QT_MOC_LITERAL(9, 104, 5), // "spmsg"
QT_MOC_LITERAL(10, 110, 16), // "current_wheel_id"
QT_MOC_LITERAL(11, 127, 3), // "Run"
QT_MOC_LITERAL(12, 131, 18), // "QVector<QVector2D>"
QT_MOC_LITERAL(13, 150, 12), // "surveyFlange"
QT_MOC_LITERAL(14, 163, 27), // "QMap<float,QVector<float> >"
QT_MOC_LITERAL(15, 191, 11), // "surveyTread"
QT_MOC_LITERAL(16, 203, 26), // "std::map<std::string,int>*"
QT_MOC_LITERAL(17, 230, 9), // "wheel_ids"
QT_MOC_LITERAL(18, 240, 37), // "std::vector<Wheel::WheelFlang..."
QT_MOC_LITERAL(19, 278, 19), // "wheel_flange_treads"
QT_MOC_LITERAL(20, 298, 36), // "std::vector<Wheel::WheelSprea..."
QT_MOC_LITERAL(21, 335, 18) // "wheel_spread_parms"

    },
    "QLocalSpreadOpenGLWidgetThread\0"
    "ProgressLog\0\0info\0present\0ErrorLog\0"
    "Result\0wheel_id\0Wheel::WheelSpreadMsg\0"
    "spmsg\0current_wheel_id\0Run\0"
    "QVector<QVector2D>\0surveyFlange\0"
    "QMap<float,QVector<float> >\0surveyTread\0"
    "std::map<std::string,int>*\0wheel_ids\0"
    "std::vector<Wheel::WheelFlangeTread>*\0"
    "wheel_flange_treads\0"
    "std::vector<Wheel::WheelSpreadParm>*\0"
    "wheel_spread_parms"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLocalSpreadOpenGLWidgetThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    1,   44,    2, 0x06 /* Public */,
       6,    2,   47,    2, 0x06 /* Public */,
       6,    1,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    6,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8,    7,    9,
    QMetaType::Void, QMetaType::Int,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 12, 0x80000000 | 14, 0x80000000 | 16, 0x80000000 | 18, 0x80000000 | 20,    7,   13,   15,   17,   19,   21,

       0        // eod
};

void QLocalSpreadOpenGLWidgetThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLocalSpreadOpenGLWidgetThread *_t = static_cast<QLocalSpreadOpenGLWidgetThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ProgressLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->ErrorLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->Result((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const Wheel::WheelSpreadMsg(*)>(_a[2]))); break;
        case 3: _t->Result((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->Run((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVector<QVector2D>(*)>(_a[2])),(*reinterpret_cast< const QMap<float,QVector<float> >(*)>(_a[3])),(*reinterpret_cast< std::map<std::string,int>*(*)>(_a[4])),(*reinterpret_cast< std::vector<Wheel::WheelFlangeTread>*(*)>(_a[5])),(*reinterpret_cast< std::vector<Wheel::WheelSpreadParm>*(*)>(_a[6]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector2D> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QLocalSpreadOpenGLWidgetThread::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadOpenGLWidgetThread::ProgressLog)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QLocalSpreadOpenGLWidgetThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadOpenGLWidgetThread::ErrorLog)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QLocalSpreadOpenGLWidgetThread::*_t)(int , const Wheel::WheelSpreadMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadOpenGLWidgetThread::Result)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QLocalSpreadOpenGLWidgetThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalSpreadOpenGLWidgetThread::Result)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject QLocalSpreadOpenGLWidgetThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QLocalSpreadOpenGLWidgetThread.data,
      qt_meta_data_QLocalSpreadOpenGLWidgetThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLocalSpreadOpenGLWidgetThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLocalSpreadOpenGLWidgetThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLocalSpreadOpenGLWidgetThread.stringdata0))
        return static_cast<void*>(const_cast< QLocalSpreadOpenGLWidgetThread*>(this));
    return QObject::qt_metacast(_clname);
}

int QLocalSpreadOpenGLWidgetThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QLocalSpreadOpenGLWidgetThread::ProgressLog(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QLocalSpreadOpenGLWidgetThread::ErrorLog(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QLocalSpreadOpenGLWidgetThread::Result(int _t1, const Wheel::WheelSpreadMsg & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QLocalSpreadOpenGLWidgetThread::Result(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
