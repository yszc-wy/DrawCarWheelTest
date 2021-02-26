/********************************************************************************
** Form generated from reading UI file 'drawcarwheeltest.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAWCARWHEELTEST_H
#define UI_DRAWCARWHEELTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DrawCarWheelTestClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DrawCarWheelTestClass)
    {
        if (DrawCarWheelTestClass->objectName().isEmpty())
            DrawCarWheelTestClass->setObjectName(QStringLiteral("DrawCarWheelTestClass"));
        DrawCarWheelTestClass->resize(600, 400);
        menuBar = new QMenuBar(DrawCarWheelTestClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        DrawCarWheelTestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DrawCarWheelTestClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DrawCarWheelTestClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(DrawCarWheelTestClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        DrawCarWheelTestClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(DrawCarWheelTestClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DrawCarWheelTestClass->setStatusBar(statusBar);

        retranslateUi(DrawCarWheelTestClass);

        QMetaObject::connectSlotsByName(DrawCarWheelTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *DrawCarWheelTestClass)
    {
        DrawCarWheelTestClass->setWindowTitle(QApplication::translate("DrawCarWheelTestClass", "DrawCarWheelTest", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DrawCarWheelTestClass: public Ui_DrawCarWheelTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAWCARWHEELTEST_H
