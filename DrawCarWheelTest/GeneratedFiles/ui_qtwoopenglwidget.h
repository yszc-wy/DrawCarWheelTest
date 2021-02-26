/********************************************************************************
** Form generated from reading UI file 'qtwoopenglwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTWOOPENGLWIDGET_H
#define UI_QTWOOPENGLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTwoOpenglWidget
{
public:

    void setupUi(QWidget *QTwoOpenglWidget)
    {
        if (QTwoOpenglWidget->objectName().isEmpty())
            QTwoOpenglWidget->setObjectName(QStringLiteral("QTwoOpenglWidget"));
        QTwoOpenglWidget->resize(400, 300);

        retranslateUi(QTwoOpenglWidget);

        QMetaObject::connectSlotsByName(QTwoOpenglWidget);
    } // setupUi

    void retranslateUi(QWidget *QTwoOpenglWidget)
    {
        QTwoOpenglWidget->setWindowTitle(QApplication::translate("QTwoOpenglWidget", "QTwoOpenglWidget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QTwoOpenglWidget: public Ui_QTwoOpenglWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTWOOPENGLWIDGET_H
