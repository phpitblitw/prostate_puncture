/********************************************************************************
** Form generated from reading UI file 'CalibrateWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATEWINDOW_H
#define UI_CALIBRATEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "GraphicsAnnotator.h"

QT_BEGIN_NAMESPACE

class Ui_CalibrateWindowClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    GraphicsAnnotator *view2D;
    QGroupBox *groupBox;
    QPushButton *BtnInitDevice;
    QPushButton *BtnStartCapture;
    QPushButton *BtnAnnotate;
    QRadioButton *RadioBtnTransverse;
    QRadioButton *RadioBtnSagittal;
    QPushButton *BtnReleaseDevice;
    QPushButton *BtnLocatePhantom;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CalibrateWindowClass)
    {
        if (CalibrateWindowClass->objectName().isEmpty())
            CalibrateWindowClass->setObjectName(QString::fromUtf8("CalibrateWindowClass"));
        CalibrateWindowClass->resize(752, 519);
        centralWidget = new QWidget(CalibrateWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        view2D = new GraphicsAnnotator(centralWidget);
        view2D->setObjectName(QString::fromUtf8("view2D"));

        horizontalLayout->addWidget(view2D);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        BtnInitDevice = new QPushButton(groupBox);
        BtnInitDevice->setObjectName(QString::fromUtf8("BtnInitDevice"));
        BtnInitDevice->setGeometry(QRect(20, 250, 75, 23));
        BtnStartCapture = new QPushButton(groupBox);
        BtnStartCapture->setObjectName(QString::fromUtf8("BtnStartCapture"));
        BtnStartCapture->setGeometry(QRect(110, 250, 75, 23));
        BtnAnnotate = new QPushButton(groupBox);
        BtnAnnotate->setObjectName(QString::fromUtf8("BtnAnnotate"));
        BtnAnnotate->setGeometry(QRect(20, 300, 75, 23));
        RadioBtnTransverse = new QRadioButton(groupBox);
        RadioBtnTransverse->setObjectName(QString::fromUtf8("RadioBtnTransverse"));
        RadioBtnTransverse->setGeometry(QRect(30, 60, 89, 16));
        RadioBtnSagittal = new QRadioButton(groupBox);
        RadioBtnSagittal->setObjectName(QString::fromUtf8("RadioBtnSagittal"));
        RadioBtnSagittal->setGeometry(QRect(30, 90, 89, 16));
        BtnReleaseDevice = new QPushButton(groupBox);
        BtnReleaseDevice->setObjectName(QString::fromUtf8("BtnReleaseDevice"));
        BtnReleaseDevice->setGeometry(QRect(190, 250, 75, 23));
        BtnLocatePhantom = new QPushButton(groupBox);
        BtnLocatePhantom->setObjectName(QString::fromUtf8("BtnLocatePhantom"));
        BtnLocatePhantom->setGeometry(QRect(120, 300, 75, 23));

        horizontalLayout->addWidget(groupBox);

        CalibrateWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CalibrateWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 752, 23));
        CalibrateWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CalibrateWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        CalibrateWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CalibrateWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        CalibrateWindowClass->setStatusBar(statusBar);

        retranslateUi(CalibrateWindowClass);

        QMetaObject::connectSlotsByName(CalibrateWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *CalibrateWindowClass)
    {
        CalibrateWindowClass->setWindowTitle(QApplication::translate("CalibrateWindowClass", "CalibrateWindow", nullptr));
        groupBox->setTitle(QApplication::translate("CalibrateWindowClass", "GroupBox", nullptr));
        BtnInitDevice->setText(QApplication::translate("CalibrateWindowClass", "\345\210\235\345\247\213\345\214\226\350\256\276\345\244\207", nullptr));
        BtnStartCapture->setText(QApplication::translate("CalibrateWindowClass", "\345\274\200\345\247\213\351\207\207\351\233\206", nullptr));
        BtnAnnotate->setText(QApplication::translate("CalibrateWindowClass", "\346\240\207\346\263\250\345\233\276\347\211\207", nullptr));
        RadioBtnTransverse->setText(QApplication::translate("CalibrateWindowClass", "\346\250\252\346\226\255\351\235\242", nullptr));
        RadioBtnSagittal->setText(QApplication::translate("CalibrateWindowClass", "\347\237\242\347\212\266\351\235\242", nullptr));
        BtnReleaseDevice->setText(QApplication::translate("CalibrateWindowClass", "\345\205\263\351\227\255\350\256\276\345\244\207", nullptr));
        BtnLocatePhantom->setText(QApplication::translate("CalibrateWindowClass", "\345\256\232\344\275\215\344\275\223\346\250\241", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibrateWindowClass: public Ui_CalibrateWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATEWINDOW_H
