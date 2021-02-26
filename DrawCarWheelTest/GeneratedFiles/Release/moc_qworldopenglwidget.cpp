/****************************************************************************
** Meta object code from reading C++ file 'qworldopenglwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qworldopenglwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qworldopenglwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QWorldOpenglWidget_t {
    QByteArrayData data[21];
    char stringdata0[289];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QWorldOpenglWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QWorldOpenglWidget_t qt_meta_stringdata_QWorldOpenglWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QWorldOpenglWidget"
QT_MOC_LITERAL(1, 19, 11), // "PickFrameID"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 2), // "id"
QT_MOC_LITERAL(4, 35, 12), // "rotateSignal"
QT_MOC_LITERAL(5, 48, 1), // "t"
QT_MOC_LITERAL(6, 50, 6), // "Bigger"
QT_MOC_LITERAL(7, 57, 7), // "Smaller"
QT_MOC_LITERAL(8, 65, 10), // "InitCamera"
QT_MOC_LITERAL(9, 76, 6), // "Rotate"
QT_MOC_LITERAL(10, 83, 15), // "TrainBoxVisible"
QT_MOC_LITERAL(11, 99, 20), // "FrontMoveCameraFocus"
QT_MOC_LITERAL(12, 120, 19), // "BackMoveCameraFocus"
QT_MOC_LITERAL(13, 140, 29), // "MoveCameraFocusBasisOfPercent"
QT_MOC_LITERAL(14, 170, 5), // "value"
QT_MOC_LITERAL(15, 176, 27), // "MoveCameraFocusBasisOfFrame"
QT_MOC_LITERAL(16, 204, 8), // "frame_id"
QT_MOC_LITERAL(17, 213, 25), // "MoveCameraFocusBasisOfBox"
QT_MOC_LITERAL(18, 239, 6), // "box_id"
QT_MOC_LITERAL(19, 246, 33), // "MoveCameraFocusBasisOfMouseOf..."
QT_MOC_LITERAL(20, 280, 8) // "x_offset"

    },
    "QWorldOpenglWidget\0PickFrameID\0\0id\0"
    "rotateSignal\0t\0Bigger\0Smaller\0InitCamera\0"
    "Rotate\0TrainBoxVisible\0FrontMoveCameraFocus\0"
    "BackMoveCameraFocus\0MoveCameraFocusBasisOfPercent\0"
    "value\0MoveCameraFocusBasisOfFrame\0"
    "frame_id\0MoveCameraFocusBasisOfBox\0"
    "box_id\0MoveCameraFocusBasisOfMouseOffset\0"
    "x_offset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QWorldOpenglWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   85,    2, 0x08 /* Private */,
       7,    0,   86,    2, 0x08 /* Private */,
       8,    0,   87,    2, 0x08 /* Private */,
       9,    0,   88,    2, 0x08 /* Private */,
      10,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    0,   91,    2, 0x08 /* Private */,
      13,    1,   92,    2, 0x08 /* Private */,
      15,    1,   95,    2, 0x08 /* Private */,
      17,    1,   98,    2, 0x08 /* Private */,
      19,    1,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   20,

       0        // eod
};

void QWorldOpenglWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QWorldOpenglWidget *_t = static_cast<QWorldOpenglWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PickFrameID((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->rotateSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->Bigger(); break;
        case 3: _t->Smaller(); break;
        case 4: _t->InitCamera(); break;
        case 5: _t->Rotate(); break;
        case 6: _t->TrainBoxVisible(); break;
        case 7: _t->FrontMoveCameraFocus(); break;
        case 8: _t->BackMoveCameraFocus(); break;
        case 9: _t->MoveCameraFocusBasisOfPercent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->MoveCameraFocusBasisOfFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->MoveCameraFocusBasisOfBox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->MoveCameraFocusBasisOfMouseOffset((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QWorldOpenglWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QWorldOpenglWidget::PickFrameID)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QWorldOpenglWidget::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QWorldOpenglWidget::rotateSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QWorldOpenglWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_QWorldOpenglWidget.data,
      qt_meta_data_QWorldOpenglWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QWorldOpenglWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QWorldOpenglWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QWorldOpenglWidget.stringdata0))
        return static_cast<void*>(const_cast< QWorldOpenglWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions_3_3_Compatibility"))
        return static_cast< QOpenGLFunctions_3_3_Compatibility*>(const_cast< QWorldOpenglWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int QWorldOpenglWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void QWorldOpenglWidget::PickFrameID(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QWorldOpenglWidget::rotateSignal(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
