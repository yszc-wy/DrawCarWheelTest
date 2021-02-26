/****************************************************************************
** Meta object code from reading C++ file 'qlocalopenglwidgetthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qlocalopenglwidgetthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlocalopenglwidgetthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QLocalOpenGLWidgetThread_t {
    QByteArrayData data[31];
    char stringdata0[543];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLocalOpenGLWidgetThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLocalOpenGLWidgetThread_t qt_meta_stringdata_QLocalOpenGLWidgetThread = {
    {
QT_MOC_LITERAL(0, 0, 24), // "QLocalOpenGLWidgetThread"
QT_MOC_LITERAL(1, 25, 11), // "ProgressLog"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 4), // "info"
QT_MOC_LITERAL(4, 43, 7), // "present"
QT_MOC_LITERAL(5, 51, 8), // "ErrorLog"
QT_MOC_LITERAL(6, 60, 6), // "Result"
QT_MOC_LITERAL(7, 67, 8), // "wheel_id"
QT_MOC_LITERAL(8, 76, 25), // "OpenglObj::OpenglPointMsg"
QT_MOC_LITERAL(9, 102, 19), // "wheelFlangeTreadMsg"
QT_MOC_LITERAL(10, 122, 18), // "wheelConnectionMsg"
QT_MOC_LITERAL(11, 141, 11), // "std::string"
QT_MOC_LITERAL(12, 153, 16), // "float_plant_path"
QT_MOC_LITERAL(13, 170, 15), // "new_float_plant"
QT_MOC_LITERAL(14, 186, 13), // "floatPlantMsg"
QT_MOC_LITERAL(15, 200, 16), // "current_wheel_id"
QT_MOC_LITERAL(16, 217, 3), // "Run"
QT_MOC_LITERAL(17, 221, 18), // "QVector<QVector2D>"
QT_MOC_LITERAL(18, 240, 12), // "surveyFlange"
QT_MOC_LITERAL(19, 253, 27), // "QMap<float,QVector<float> >"
QT_MOC_LITERAL(20, 281, 11), // "surveyTread"
QT_MOC_LITERAL(21, 293, 20), // "FilePath::FloatPlant"
QT_MOC_LITERAL(22, 314, 11), // "float_plant"
QT_MOC_LITERAL(23, 326, 26), // "std::map<std::string,int>*"
QT_MOC_LITERAL(24, 353, 9), // "wheel_ids"
QT_MOC_LITERAL(25, 363, 37), // "std::vector<Wheel::WheelFlang..."
QT_MOC_LITERAL(26, 401, 19), // "wheel_flange_treads"
QT_MOC_LITERAL(27, 421, 39), // "std::vector<Wheel::WheelColor..."
QT_MOC_LITERAL(28, 461, 21), // "wheel_color_contrasts"
QT_MOC_LITERAL(29, 483, 46), // "std::map<std::string,std::vec..."
QT_MOC_LITERAL(30, 530, 12) // "float_plants"

    },
    "QLocalOpenGLWidgetThread\0ProgressLog\0"
    "\0info\0present\0ErrorLog\0Result\0wheel_id\0"
    "OpenglObj::OpenglPointMsg\0wheelFlangeTreadMsg\0"
    "wheelConnectionMsg\0std::string\0"
    "float_plant_path\0new_float_plant\0"
    "floatPlantMsg\0current_wheel_id\0Run\0"
    "QVector<QVector2D>\0surveyFlange\0"
    "QMap<float,QVector<float> >\0surveyTread\0"
    "FilePath::FloatPlant\0float_plant\0"
    "std::map<std::string,int>*\0wheel_ids\0"
    "std::vector<Wheel::WheelFlangeTread>*\0"
    "wheel_flange_treads\0"
    "std::vector<Wheel::WheelColorContrast>*\0"
    "wheel_color_contrasts\0"
    "std::map<std::string,std::vector<glm::vec2> >*\0"
    "float_plants"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLocalOpenGLWidgetThread[] = {

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
       6,    6,   47,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    8,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8, 0x80000000 | 8, 0x80000000 | 11, QMetaType::Bool, 0x80000000 | 8,    7,    9,   10,   12,   13,   14,
    QMetaType::Void, QMetaType::Int,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 17, 0x80000000 | 19, 0x80000000 | 21, 0x80000000 | 23, 0x80000000 | 25, 0x80000000 | 27, 0x80000000 | 29,    7,   18,   20,   22,   24,   26,   28,   30,

       0        // eod
};

void QLocalOpenGLWidgetThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLocalOpenGLWidgetThread *_t = static_cast<QLocalOpenGLWidgetThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ProgressLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->ErrorLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->Result((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const OpenglObj::OpenglPointMsg(*)>(_a[2])),(*reinterpret_cast< const OpenglObj::OpenglPointMsg(*)>(_a[3])),(*reinterpret_cast< const std::string(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< const OpenglObj::OpenglPointMsg(*)>(_a[6]))); break;
        case 3: _t->Result((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->Run((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVector<QVector2D>(*)>(_a[2])),(*reinterpret_cast< const QMap<float,QVector<float> >(*)>(_a[3])),(*reinterpret_cast< FilePath::FloatPlant(*)>(_a[4])),(*reinterpret_cast< std::map<std::string,int>*(*)>(_a[5])),(*reinterpret_cast< std::vector<Wheel::WheelFlangeTread>*(*)>(_a[6])),(*reinterpret_cast< std::vector<Wheel::WheelColorContrast>*(*)>(_a[7])),(*reinterpret_cast< std::map<std::string,std::vector<glm::vec2> >*(*)>(_a[8]))); break;
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
            typedef void (QLocalOpenGLWidgetThread::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidgetThread::ProgressLog)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidgetThread::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidgetThread::ErrorLog)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidgetThread::*_t)(int , const OpenglObj::OpenglPointMsg & , const OpenglObj::OpenglPointMsg & , const std::string & , bool , const OpenglObj::OpenglPointMsg & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidgetThread::Result)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidgetThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidgetThread::Result)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject QLocalOpenGLWidgetThread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QLocalOpenGLWidgetThread.data,
      qt_meta_data_QLocalOpenGLWidgetThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLocalOpenGLWidgetThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLocalOpenGLWidgetThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLocalOpenGLWidgetThread.stringdata0))
        return static_cast<void*>(const_cast< QLocalOpenGLWidgetThread*>(this));
    return QObject::qt_metacast(_clname);
}

int QLocalOpenGLWidgetThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QLocalOpenGLWidgetThread::ProgressLog(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QLocalOpenGLWidgetThread::ErrorLog(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QLocalOpenGLWidgetThread::Result(int _t1, const OpenglObj::OpenglPointMsg & _t2, const OpenglObj::OpenglPointMsg & _t3, const std::string & _t4, bool _t5, const OpenglObj::OpenglPointMsg & _t6)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QLocalOpenGLWidgetThread::Result(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
