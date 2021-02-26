/********************************************************************************
** Form generated from reading UI file 'dlgtrain3d.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGTRAIN3D_H
#define UI_DLGTRAIN3D_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgTrain3D
{
public:
    QWidget *centralwidget;
    QToolBar *toolBar;

    void setupUi(QMainWindow *DlgTrain3D)
    {
        if (DlgTrain3D->objectName().isEmpty())
            DlgTrain3D->setObjectName(QStringLiteral("DlgTrain3D"));
        DlgTrain3D->resize(800, 600);
        centralwidget = new QWidget(DlgTrain3D);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        DlgTrain3D->setCentralWidget(centralwidget);
        toolBar = new QToolBar(DlgTrain3D);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        DlgTrain3D->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(DlgTrain3D);

        QMetaObject::connectSlotsByName(DlgTrain3D);
    } // setupUi

    void retranslateUi(QMainWindow *DlgTrain3D)
    {
        DlgTrain3D->setWindowTitle(QApplication::translate("DlgTrain3D", "MainWindow", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("DlgTrain3D", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DlgTrain3D: public Ui_DlgTrain3D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGTRAIN3D_H
