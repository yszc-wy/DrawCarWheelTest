/****************************************************************************
** Meta object code from reading C++ file 'qgotowidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../qgotowidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgotowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QGoToWidget_t {
    QByteArrayData data[10];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QGoToWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QGoToWidget_t qt_meta_stringdata_QGoToWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QGoToWidget"
QT_MOC_LITERAL(1, 12, 30), // "widthLocateSpinBoxValueChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 5), // "value"
QT_MOC_LITERAL(4, 50, 32), // "sectionLocateSpinBoxValueChanged"
QT_MOC_LITERAL(5, 83, 19), // "setWidthLocateValue"
QT_MOC_LITERAL(6, 103, 21), // "setSectionLocateValue"
QT_MOC_LITERAL(7, 125, 19), // "setWidthLocateRange"
QT_MOC_LITERAL(8, 145, 5), // "start"
QT_MOC_LITERAL(9, 151, 3) // "end"

    },
    "QGoToWidget\0widthLocateSpinBoxValueChanged\0"
    "\0value\0sectionLocateSpinBoxValueChanged\0"
    "setWidthLocateValue\0setSectionLocateValue\0"
    "setWidthLocateRange\0start\0end"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QGoToWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   45,    2, 0x0a /* Public */,
       6,    1,   48,    2, 0x0a /* Public */,
       7,    2,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    8,    9,

       0        // eod
};

void QGoToWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QGoToWidget *_t = static_cast<QGoToWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->widthLocateSpinBoxValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->sectionLocateSpinBoxValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setWidthLocateValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setSectionLocateValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->setWidthLocateRange((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QGoToWidget::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGoToWidget::widthLocateSpinBoxValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QGoToWidget::*_t)(double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGoToWidget::sectionLocateSpinBoxValueChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QGoToWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QGoToWidget.data,
      qt_meta_data_QGoToWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QGoToWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGoToWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QGoToWidget.stringdata0))
        return static_cast<void*>(const_cast< QGoToWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QGoToWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void QGoToWidget::widthLocateSpinBoxValueChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QGoToWidget::sectionLocateSpinBoxValueChanged(double _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
