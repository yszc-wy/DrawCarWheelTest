/****************************************************************************
** Meta object code from reading C++ file 'qlocalopenglwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qlocalopenglwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlocalopenglwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QLocalOpenGLWidget_t {
    QByteArrayData data[50];
    char stringdata0[769];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLocalOpenGLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLocalOpenGLWidget_t qt_meta_stringdata_QLocalOpenGLWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QLocalOpenGLWidget"
QT_MOC_LITERAL(1, 19, 10), // "InitGLOver"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "PickSectionMsg"
QT_MOC_LITERAL(4, 46, 18), // "QVector<QVector2D>"
QT_MOC_LITERAL(5, 65, 16), // "PickSectionAngle"
QT_MOC_LITERAL(6, 82, 5), // "angle"
QT_MOC_LITERAL(7, 88, 20), // "SpreadWheelOpenGLMsg"
QT_MOC_LITERAL(8, 109, 21), // "Wheel::WheelSpreadMsg"
QT_MOC_LITERAL(9, 131, 3), // "msg"
QT_MOC_LITERAL(10, 135, 12), // "RotateSignal"
QT_MOC_LITERAL(11, 148, 1), // "t"
QT_MOC_LITERAL(12, 150, 10), // "FullScreen"
QT_MOC_LITERAL(13, 161, 2), // "fs"
QT_MOC_LITERAL(14, 164, 19), // "StartAddWheelThread"
QT_MOC_LITERAL(15, 184, 8), // "wheel_id"
QT_MOC_LITERAL(16, 193, 12), // "surveyFlange"
QT_MOC_LITERAL(17, 206, 27), // "QMap<float,QVector<float> >"
QT_MOC_LITERAL(18, 234, 11), // "surveyTread"
QT_MOC_LITERAL(19, 246, 20), // "FilePath::FloatPlant"
QT_MOC_LITERAL(20, 267, 11), // "float_plant"
QT_MOC_LITERAL(21, 279, 26), // "std::map<std::string,int>*"
QT_MOC_LITERAL(22, 306, 9), // "wheel_ids"
QT_MOC_LITERAL(23, 316, 37), // "std::vector<Wheel::WheelFlang..."
QT_MOC_LITERAL(24, 354, 19), // "wheel_flange_treads"
QT_MOC_LITERAL(25, 374, 39), // "std::vector<Wheel::WheelColor..."
QT_MOC_LITERAL(26, 414, 21), // "wheel_color_contrasts"
QT_MOC_LITERAL(27, 436, 46), // "std::map<std::string,std::vec..."
QT_MOC_LITERAL(28, 483, 12), // "float_plants"
QT_MOC_LITERAL(29, 496, 10), // "ProcessLog"
QT_MOC_LITERAL(30, 507, 4), // "info"
QT_MOC_LITERAL(31, 512, 7), // "present"
QT_MOC_LITERAL(32, 520, 8), // "ErrorLog"
QT_MOC_LITERAL(33, 529, 15), // "SetCurrentWheel"
QT_MOC_LITERAL(34, 545, 2), // "id"
QT_MOC_LITERAL(35, 548, 17), // "EnableSectionPick"
QT_MOC_LITERAL(36, 566, 15), // "EnableWidthPick"
QT_MOC_LITERAL(37, 582, 16), // "ColorMarkVisible"
QT_MOC_LITERAL(38, 599, 6), // "Bigger"
QT_MOC_LITERAL(39, 606, 7), // "Smaller"
QT_MOC_LITERAL(40, 614, 10), // "InitCamera"
QT_MOC_LITERAL(41, 625, 6), // "Rotate"
QT_MOC_LITERAL(42, 632, 12), // "AddOpenglMsg"
QT_MOC_LITERAL(43, 645, 25), // "OpenglObj::OpenglPointMsg"
QT_MOC_LITERAL(44, 671, 19), // "wheelFlangeTreadMsg"
QT_MOC_LITERAL(45, 691, 18), // "wheelConnectionMsg"
QT_MOC_LITERAL(46, 710, 11), // "std::string"
QT_MOC_LITERAL(47, 722, 16), // "float_plant_path"
QT_MOC_LITERAL(48, 739, 15), // "new_float_plant"
QT_MOC_LITERAL(49, 755, 13) // "floatPlantMsg"

    },
    "QLocalOpenGLWidget\0InitGLOver\0\0"
    "PickSectionMsg\0QVector<QVector2D>\0"
    "PickSectionAngle\0angle\0SpreadWheelOpenGLMsg\0"
    "Wheel::WheelSpreadMsg\0msg\0RotateSignal\0"
    "t\0FullScreen\0fs\0StartAddWheelThread\0"
    "wheel_id\0surveyFlange\0QMap<float,QVector<float> >\0"
    "surveyTread\0FilePath::FloatPlant\0"
    "float_plant\0std::map<std::string,int>*\0"
    "wheel_ids\0std::vector<Wheel::WheelFlangeTread>*\0"
    "wheel_flange_treads\0"
    "std::vector<Wheel::WheelColorContrast>*\0"
    "wheel_color_contrasts\0"
    "std::map<std::string,std::vector<glm::vec2> >*\0"
    "float_plants\0ProcessLog\0info\0present\0"
    "ErrorLog\0SetCurrentWheel\0id\0"
    "EnableSectionPick\0EnableWidthPick\0"
    "ColorMarkVisible\0Bigger\0Smaller\0"
    "InitCamera\0Rotate\0AddOpenglMsg\0"
    "OpenglObj::OpenglPointMsg\0wheelFlangeTreadMsg\0"
    "wheelConnectionMsg\0std::string\0"
    "float_plant_path\0new_float_plant\0"
    "floatPlantMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLocalOpenGLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    1,  105,    2, 0x06 /* Public */,
       5,    1,  108,    2, 0x06 /* Public */,
       7,    1,  111,    2, 0x06 /* Public */,
      10,    1,  114,    2, 0x06 /* Public */,
      12,    1,  117,    2, 0x06 /* Public */,
      14,    8,  120,    2, 0x06 /* Public */,
      29,    2,  137,    2, 0x06 /* Public */,
      32,    1,  142,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      33,    1,  145,    2, 0x0a /* Public */,
      35,    0,  148,    2, 0x08 /* Private */,
      36,    0,  149,    2, 0x08 /* Private */,
      37,    0,  150,    2, 0x08 /* Private */,
      38,    0,  151,    2, 0x08 /* Private */,
      39,    0,  152,    2, 0x08 /* Private */,
      40,    0,  153,    2, 0x08 /* Private */,
      41,    0,  154,    2, 0x08 /* Private */,
      42,    6,  155,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, QMetaType::Float,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4, 0x80000000 | 17, 0x80000000 | 19, 0x80000000 | 21, 0x80000000 | 23, 0x80000000 | 25, 0x80000000 | 27,   15,   16,   18,   20,   22,   24,   26,   28,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   30,   31,
    QMetaType::Void, QMetaType::QString,   30,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 43, 0x80000000 | 43, 0x80000000 | 46, QMetaType::Bool, 0x80000000 | 43,   15,   44,   45,   47,   48,   49,

       0        // eod
};

void QLocalOpenGLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLocalOpenGLWidget *_t = static_cast<QLocalOpenGLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->InitGLOver(); break;
        case 1: _t->PickSectionMsg((*reinterpret_cast< QVector<QVector2D>(*)>(_a[1]))); break;
        case 2: _t->PickSectionAngle((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->SpreadWheelOpenGLMsg((*reinterpret_cast< Wheel::WheelSpreadMsg(*)>(_a[1]))); break;
        case 4: _t->RotateSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->FullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->StartAddWheelThread((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVector<QVector2D>(*)>(_a[2])),(*reinterpret_cast< const QMap<float,QVector<float> >(*)>(_a[3])),(*reinterpret_cast< FilePath::FloatPlant(*)>(_a[4])),(*reinterpret_cast< std::map<std::string,int>*(*)>(_a[5])),(*reinterpret_cast< std::vector<Wheel::WheelFlangeTread>*(*)>(_a[6])),(*reinterpret_cast< std::vector<Wheel::WheelColorContrast>*(*)>(_a[7])),(*reinterpret_cast< std::map<std::string,std::vector<glm::vec2> >*(*)>(_a[8]))); break;
        case 7: _t->ProcessLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->ErrorLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->SetCurrentWheel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->EnableSectionPick(); break;
        case 11: _t->EnableWidthPick(); break;
        case 12: _t->ColorMarkVisible(); break;
        case 13: _t->Bigger(); break;
        case 14: _t->Smaller(); break;
        case 15: _t->InitCamera(); break;
        case 16: _t->Rotate(); break;
        case 17: _t->AddOpenglMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const OpenglObj::OpenglPointMsg(*)>(_a[2])),(*reinterpret_cast< const OpenglObj::OpenglPointMsg(*)>(_a[3])),(*reinterpret_cast< const std::string(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< const OpenglObj::OpenglPointMsg(*)>(_a[6]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QVector2D> >(); break;
            }
            break;
        case 6:
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
            typedef void (QLocalOpenGLWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::InitGLOver)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(QVector<QVector2D> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::PickSectionMsg)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::PickSectionAngle)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(Wheel::WheelSpreadMsg );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::SpreadWheelOpenGLMsg)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::RotateSignal)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::FullScreen)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(const QString & , const QVector<QVector2D> & , const QMap<float,QVector<float>> & , FilePath::FloatPlant , std::map<std::string,int> * , std::vector<Wheel::WheelFlangeTread> * , std::vector<Wheel::WheelColorContrast> * , std::map<std::string,std::vector<glm::vec2>> * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::StartAddWheelThread)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::ProcessLog)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (QLocalOpenGLWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLocalOpenGLWidget::ErrorLog)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject QLocalOpenGLWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_QLocalOpenGLWidget.data,
      qt_meta_data_QLocalOpenGLWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLocalOpenGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLocalOpenGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLocalOpenGLWidget.stringdata0))
        return static_cast<void*>(const_cast< QLocalOpenGLWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Compatibility"))
        return static_cast< QOpenGLFunctions_3_3_Compatibility*>(const_cast< QLocalOpenGLWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int QLocalOpenGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void QLocalOpenGLWidget::InitGLOver()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void QLocalOpenGLWidget::PickSectionMsg(QVector<QVector2D> _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QLocalOpenGLWidget::PickSectionAngle(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QLocalOpenGLWidget::SpreadWheelOpenGLMsg(Wheel::WheelSpreadMsg _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QLocalOpenGLWidget::RotateSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QLocalOpenGLWidget::FullScreen(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QLocalOpenGLWidget::StartAddWheelThread(const QString & _t1, const QVector<QVector2D> & _t2, const QMap<float,QVector<float>> & _t3, FilePath::FloatPlant _t4, std::map<std::string,int> * _t5, std::vector<Wheel::WheelFlangeTread> * _t6, std::vector<Wheel::WheelColorContrast> * _t7, std::map<std::string,std::vector<glm::vec2>> * _t8)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QLocalOpenGLWidget::ProcessLog(const QString & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QLocalOpenGLWidget::ErrorLog(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
