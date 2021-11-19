/********************************************************************************
** Form generated from reading UI file 'PunctureWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUNCTUREWINDOW_H
#define UI_PUNCTUREWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GraphicsView2D.h"
#include "OpenGLWidget3D.h"

QT_BEGIN_NAMESPACE

class Ui_PunctureWindowClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    GraphicsView2D *view2D2;
    GraphicsView2D *view2D1;
    OpenGLWidget3D *view3D;
    QGroupBox *userZone;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *LabelScanCenter;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *ScanCenterX;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *ScanCenterY;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *ScanCenterZ;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *ScanCenterW;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *LabelScanCenter_2;
    QSpacerItem *horizontalSpacer_8;
    QLineEdit *RightDirX;
    QSpacerItem *horizontalSpacer_9;
    QLineEdit *RightDirY;
    QSpacerItem *horizontalSpacer_10;
    QLineEdit *RightDirZ;
    QSpacerItem *horizontalSpacer_11;
    QLineEdit *RightDirW;
    QSpacerItem *horizontalSpacer_12;
    QHBoxLayout *horizontalLayout_4;
    QLabel *LabelScanCenter_3;
    QSpacerItem *horizontalSpacer_13;
    QLineEdit *UpDirX;
    QSpacerItem *horizontalSpacer_14;
    QLineEdit *UpDirY;
    QSpacerItem *horizontalSpacer_15;
    QLineEdit *UpDirZ;
    QSpacerItem *horizontalSpacer_16;
    QLineEdit *UpDirW;
    QSpacerItem *horizontalSpacer_17;
    QHBoxLayout *horizontalLayout_5;
    QLabel *LabelScanCenter_4;
    QSpacerItem *horizontalSpacer_18;
    QLineEdit *MoveDirX;
    QSpacerItem *horizontalSpacer_19;
    QLineEdit *MoveDirY;
    QSpacerItem *horizontalSpacer_20;
    QLineEdit *MoveDirZ;
    QSpacerItem *horizontalSpacer_21;
    QLineEdit *MoveDirW;
    QSpacerItem *horizontalSpacer_22;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QPushButton *BtnInitDevice;
    QPushButton *BtnUpdateUS;
    QPushButton *BtnLoadPatientData;
    QPushButton *BtnQuit;
    QPushButton *BtnResetRegister;
    QPushButton *BtnRegister;
    QRadioButton *radioBtnSaveDebug;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PunctureWindowClass)
    {
        if (PunctureWindowClass->objectName().isEmpty())
            PunctureWindowClass->setObjectName(QString::fromUtf8("PunctureWindowClass"));
        PunctureWindowClass->resize(1117, 743);
        centralWidget = new QWidget(PunctureWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        view2D2 = new GraphicsView2D(centralWidget);
        view2D2->setObjectName(QString::fromUtf8("view2D2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(view2D2->sizePolicy().hasHeightForWidth());
        view2D2->setSizePolicy(sizePolicy);
        view2D2->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(view2D2, 1, 0, 1, 1);

        view2D1 = new GraphicsView2D(centralWidget);
        view2D1->setObjectName(QString::fromUtf8("view2D1"));
        sizePolicy.setHeightForWidth(view2D1->sizePolicy().hasHeightForWidth());
        view2D1->setSizePolicy(sizePolicy);
        view2D1->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(view2D1, 0, 0, 1, 1);

        view3D = new OpenGLWidget3D(centralWidget);
        view3D->setObjectName(QString::fromUtf8("view3D"));

        gridLayout->addWidget(view3D, 0, 1, 1, 1);

        userZone = new QGroupBox(centralWidget);
        userZone->setObjectName(QString::fromUtf8("userZone"));
        sizePolicy.setHeightForWidth(userZone->sizePolicy().hasHeightForWidth());
        userZone->setSizePolicy(sizePolicy);
        layoutWidget = new QWidget(userZone);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 50, 381, 127));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        LabelScanCenter = new QLabel(layoutWidget);
        LabelScanCenter->setObjectName(QString::fromUtf8("LabelScanCenter"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(30);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LabelScanCenter->sizePolicy().hasHeightForWidth());
        LabelScanCenter->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(LabelScanCenter);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        ScanCenterX = new QLineEdit(layoutWidget);
        ScanCenterX->setObjectName(QString::fromUtf8("ScanCenterX"));

        horizontalLayout_2->addWidget(ScanCenterX);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        ScanCenterY = new QLineEdit(layoutWidget);
        ScanCenterY->setObjectName(QString::fromUtf8("ScanCenterY"));

        horizontalLayout_2->addWidget(ScanCenterY);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        ScanCenterZ = new QLineEdit(layoutWidget);
        ScanCenterZ->setObjectName(QString::fromUtf8("ScanCenterZ"));

        horizontalLayout_2->addWidget(ScanCenterZ);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        ScanCenterW = new QLineEdit(layoutWidget);
        ScanCenterW->setObjectName(QString::fromUtf8("ScanCenterW"));

        horizontalLayout_2->addWidget(ScanCenterW);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        LabelScanCenter_2 = new QLabel(layoutWidget);
        LabelScanCenter_2->setObjectName(QString::fromUtf8("LabelScanCenter_2"));
        sizePolicy1.setHeightForWidth(LabelScanCenter_2->sizePolicy().hasHeightForWidth());
        LabelScanCenter_2->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(LabelScanCenter_2);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        RightDirX = new QLineEdit(layoutWidget);
        RightDirX->setObjectName(QString::fromUtf8("RightDirX"));

        horizontalLayout_3->addWidget(RightDirX);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_9);

        RightDirY = new QLineEdit(layoutWidget);
        RightDirY->setObjectName(QString::fromUtf8("RightDirY"));

        horizontalLayout_3->addWidget(RightDirY);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_10);

        RightDirZ = new QLineEdit(layoutWidget);
        RightDirZ->setObjectName(QString::fromUtf8("RightDirZ"));

        horizontalLayout_3->addWidget(RightDirZ);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_11);

        RightDirW = new QLineEdit(layoutWidget);
        RightDirW->setObjectName(QString::fromUtf8("RightDirW"));

        horizontalLayout_3->addWidget(RightDirW);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_12);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        LabelScanCenter_3 = new QLabel(layoutWidget);
        LabelScanCenter_3->setObjectName(QString::fromUtf8("LabelScanCenter_3"));
        sizePolicy1.setHeightForWidth(LabelScanCenter_3->sizePolicy().hasHeightForWidth());
        LabelScanCenter_3->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(LabelScanCenter_3);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_13);

        UpDirX = new QLineEdit(layoutWidget);
        UpDirX->setObjectName(QString::fromUtf8("UpDirX"));

        horizontalLayout_4->addWidget(UpDirX);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_14);

        UpDirY = new QLineEdit(layoutWidget);
        UpDirY->setObjectName(QString::fromUtf8("UpDirY"));

        horizontalLayout_4->addWidget(UpDirY);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_15);

        UpDirZ = new QLineEdit(layoutWidget);
        UpDirZ->setObjectName(QString::fromUtf8("UpDirZ"));

        horizontalLayout_4->addWidget(UpDirZ);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_16);

        UpDirW = new QLineEdit(layoutWidget);
        UpDirW->setObjectName(QString::fromUtf8("UpDirW"));

        horizontalLayout_4->addWidget(UpDirW);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_17);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        LabelScanCenter_4 = new QLabel(layoutWidget);
        LabelScanCenter_4->setObjectName(QString::fromUtf8("LabelScanCenter_4"));

        horizontalLayout_5->addWidget(LabelScanCenter_4);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_18);

        MoveDirX = new QLineEdit(layoutWidget);
        MoveDirX->setObjectName(QString::fromUtf8("MoveDirX"));

        horizontalLayout_5->addWidget(MoveDirX);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_19);

        MoveDirY = new QLineEdit(layoutWidget);
        MoveDirY->setObjectName(QString::fromUtf8("MoveDirY"));

        horizontalLayout_5->addWidget(MoveDirY);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_20);

        MoveDirZ = new QLineEdit(layoutWidget);
        MoveDirZ->setObjectName(QString::fromUtf8("MoveDirZ"));

        horizontalLayout_5->addWidget(MoveDirZ);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_21);

        MoveDirW = new QLineEdit(layoutWidget);
        MoveDirW->setObjectName(QString::fromUtf8("MoveDirW"));

        horizontalLayout_5->addWidget(MoveDirW);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_22);


        verticalLayout->addLayout(horizontalLayout_5);

        layoutWidget1 = new QWidget(userZone);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(190, 210, 195, 105));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        BtnInitDevice = new QPushButton(layoutWidget1);
        BtnInitDevice->setObjectName(QString::fromUtf8("BtnInitDevice"));

        gridLayout_2->addWidget(BtnInitDevice, 0, 1, 1, 1);

        BtnUpdateUS = new QPushButton(layoutWidget1);
        BtnUpdateUS->setObjectName(QString::fromUtf8("BtnUpdateUS"));

        gridLayout_2->addWidget(BtnUpdateUS, 2, 0, 1, 1);

        BtnLoadPatientData = new QPushButton(layoutWidget1);
        BtnLoadPatientData->setObjectName(QString::fromUtf8("BtnLoadPatientData"));

        gridLayout_2->addWidget(BtnLoadPatientData, 0, 0, 1, 1);

        BtnQuit = new QPushButton(layoutWidget1);
        BtnQuit->setObjectName(QString::fromUtf8("BtnQuit"));

        gridLayout_2->addWidget(BtnQuit, 2, 1, 1, 1);

        BtnResetRegister = new QPushButton(layoutWidget1);
        BtnResetRegister->setObjectName(QString::fromUtf8("BtnResetRegister"));

        gridLayout_2->addWidget(BtnResetRegister, 1, 1, 1, 1);

        BtnRegister = new QPushButton(layoutWidget1);
        BtnRegister->setObjectName(QString::fromUtf8("BtnRegister"));

        gridLayout_2->addWidget(BtnRegister, 1, 0, 1, 1);

        radioBtnSaveDebug = new QRadioButton(layoutWidget1);
        radioBtnSaveDebug->setObjectName(QString::fromUtf8("radioBtnSaveDebug"));

        gridLayout_2->addWidget(radioBtnSaveDebug, 3, 0, 1, 1);


        gridLayout->addWidget(userZone, 1, 1, 1, 1);

        PunctureWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PunctureWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1117, 23));
        PunctureWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PunctureWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PunctureWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PunctureWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PunctureWindowClass->setStatusBar(statusBar);

        retranslateUi(PunctureWindowClass);

        QMetaObject::connectSlotsByName(PunctureWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *PunctureWindowClass)
    {
        PunctureWindowClass->setWindowTitle(QApplication::translate("PunctureWindowClass", "PunctureWindow", nullptr));
        userZone->setTitle(QApplication::translate("PunctureWindowClass", "\347\224\250\346\210\267\345\214\272", nullptr));
        LabelScanCenter->setText(QApplication::translate("PunctureWindowClass", "ScanCenter", nullptr));
        LabelScanCenter_2->setText(QApplication::translate("PunctureWindowClass", "RightDir  ", nullptr));
        LabelScanCenter_3->setText(QApplication::translate("PunctureWindowClass", "UpDir     ", nullptr));
        LabelScanCenter_4->setText(QApplication::translate("PunctureWindowClass", "MoveDir   ", nullptr));
        BtnInitDevice->setText(QApplication::translate("PunctureWindowClass", "\345\210\235\345\247\213\345\214\226\350\256\276\345\244\207", nullptr));
        BtnUpdateUS->setText(QApplication::translate("PunctureWindowClass", "\346\233\264\346\226\260\350\266\205\345\243\260\345\217\202\346\225\260", nullptr));
        BtnLoadPatientData->setText(QApplication::translate("PunctureWindowClass", "\350\275\275\345\205\245\347\227\205\344\272\272\346\225\260\346\215\256", nullptr));
        BtnQuit->setText(QApplication::translate("PunctureWindowClass", "\351\200\200\345\207\272", nullptr));
        BtnResetRegister->setText(QApplication::translate("PunctureWindowClass", "\351\207\215\347\275\256\351\205\215\345\207\206", nullptr));
        BtnRegister->setText(QApplication::translate("PunctureWindowClass", "\351\224\201\345\256\232\345\235\220\346\240\207", nullptr));
        radioBtnSaveDebug->setText(QApplication::translate("PunctureWindowClass", "\345\255\230\345\202\250\346\265\213\350\257\225\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PunctureWindowClass: public Ui_PunctureWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUNCTUREWINDOW_H
