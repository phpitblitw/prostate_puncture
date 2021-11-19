/********************************************************************************
** Form generated from reading UI file 'AnnotateWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANNOTATEWINDOW_H
#define UI_ANNOTATEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GraphicsAnnotator.h"

QT_BEGIN_NAMESPACE

class Ui_AnnotateWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    GraphicsAnnotator *Annotate2D;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *LabelPosX;
    QLabel *LabelPosY;
    QLabel *LabelFilename;
    QHBoxLayout *horizontalLayout_4;
    QLabel *LabelPixelSizeX;
    QLineEdit *PixelSizeX;
    QLabel *LabelPixelSizeY;
    QLineEdit *PixelSizeY;
    QRadioButton *RadioBtnTransverse;
    QRadioButton *RadioBtnSagittal;
    QPushButton *BtnSetImageDir;
    QPushButton *BtnLoadPhantomTriangles;
    QHBoxLayout *horizontalLayout;
    QPushButton *BtnFormerImage;
    QPushButton *BtnNextImage;
    QPushButton *BtnFinish;
    QPushButton *BtnSave2D;
    QPushButton *BtnSave3DInImage;
    QPushButton *BtnSave3D;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AnnotateWindow)
    {
        if (AnnotateWindow->objectName().isEmpty())
            AnnotateWindow->setObjectName(QString::fromUtf8("AnnotateWindow"));
        AnnotateWindow->resize(601, 564);
        centralWidget = new QWidget(AnnotateWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        Annotate2D = new GraphicsAnnotator(centralWidget);
        Annotate2D->setObjectName(QString::fromUtf8("Annotate2D"));

        verticalLayout->addWidget(Annotate2D);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        LabelPosX = new QLabel(groupBox);
        LabelPosX->setObjectName(QString::fromUtf8("LabelPosX"));

        horizontalLayout_2->addWidget(LabelPosX);

        LabelPosY = new QLabel(groupBox);
        LabelPosY->setObjectName(QString::fromUtf8("LabelPosY"));

        horizontalLayout_2->addWidget(LabelPosY);

        LabelFilename = new QLabel(groupBox);
        LabelFilename->setObjectName(QString::fromUtf8("LabelFilename"));

        horizontalLayout_2->addWidget(LabelFilename);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        LabelPixelSizeX = new QLabel(groupBox);
        LabelPixelSizeX->setObjectName(QString::fromUtf8("LabelPixelSizeX"));

        horizontalLayout_4->addWidget(LabelPixelSizeX);

        PixelSizeX = new QLineEdit(groupBox);
        PixelSizeX->setObjectName(QString::fromUtf8("PixelSizeX"));

        horizontalLayout_4->addWidget(PixelSizeX);

        LabelPixelSizeY = new QLabel(groupBox);
        LabelPixelSizeY->setObjectName(QString::fromUtf8("LabelPixelSizeY"));

        horizontalLayout_4->addWidget(LabelPixelSizeY);

        PixelSizeY = new QLineEdit(groupBox);
        PixelSizeY->setObjectName(QString::fromUtf8("PixelSizeY"));

        horizontalLayout_4->addWidget(PixelSizeY);

        RadioBtnTransverse = new QRadioButton(groupBox);
        RadioBtnTransverse->setObjectName(QString::fromUtf8("RadioBtnTransverse"));

        horizontalLayout_4->addWidget(RadioBtnTransverse);

        RadioBtnSagittal = new QRadioButton(groupBox);
        RadioBtnSagittal->setObjectName(QString::fromUtf8("RadioBtnSagittal"));

        horizontalLayout_4->addWidget(RadioBtnSagittal);


        verticalLayout_2->addLayout(horizontalLayout_4);

        BtnSetImageDir = new QPushButton(groupBox);
        BtnSetImageDir->setObjectName(QString::fromUtf8("BtnSetImageDir"));

        verticalLayout_2->addWidget(BtnSetImageDir);

        BtnLoadPhantomTriangles = new QPushButton(groupBox);
        BtnLoadPhantomTriangles->setObjectName(QString::fromUtf8("BtnLoadPhantomTriangles"));

        verticalLayout_2->addWidget(BtnLoadPhantomTriangles);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        BtnFormerImage = new QPushButton(groupBox);
        BtnFormerImage->setObjectName(QString::fromUtf8("BtnFormerImage"));

        horizontalLayout->addWidget(BtnFormerImage);

        BtnNextImage = new QPushButton(groupBox);
        BtnNextImage->setObjectName(QString::fromUtf8("BtnNextImage"));

        horizontalLayout->addWidget(BtnNextImage);

        BtnFinish = new QPushButton(groupBox);
        BtnFinish->setObjectName(QString::fromUtf8("BtnFinish"));

        horizontalLayout->addWidget(BtnFinish);


        verticalLayout_2->addLayout(horizontalLayout);

        BtnSave2D = new QPushButton(groupBox);
        BtnSave2D->setObjectName(QString::fromUtf8("BtnSave2D"));

        verticalLayout_2->addWidget(BtnSave2D);

        BtnSave3DInImage = new QPushButton(groupBox);
        BtnSave3DInImage->setObjectName(QString::fromUtf8("BtnSave3DInImage"));

        verticalLayout_2->addWidget(BtnSave3DInImage);

        BtnSave3D = new QPushButton(groupBox);
        BtnSave3D->setObjectName(QString::fromUtf8("BtnSave3D"));

        verticalLayout_2->addWidget(BtnSave3D);


        verticalLayout->addWidget(groupBox);

        AnnotateWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AnnotateWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 601, 26));
        AnnotateWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AnnotateWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        AnnotateWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AnnotateWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        AnnotateWindow->setStatusBar(statusBar);

        retranslateUi(AnnotateWindow);

        QMetaObject::connectSlotsByName(AnnotateWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AnnotateWindow)
    {
        AnnotateWindow->setWindowTitle(QApplication::translate("AnnotateWindow", "AnnotateWindow", nullptr));
        groupBox->setTitle(QApplication::translate("AnnotateWindow", "GroupBox", nullptr));
        LabelPosX->setText(QApplication::translate("AnnotateWindow", "x", nullptr));
        LabelPosY->setText(QApplication::translate("AnnotateWindow", "y", nullptr));
        LabelFilename->setText(QApplication::translate("AnnotateWindow", "Filename", nullptr));
        LabelPixelSizeX->setText(QApplication::translate("AnnotateWindow", "\345\203\217\347\264\240\345\244\247\345\260\217X(mm)", nullptr));
        LabelPixelSizeY->setText(QApplication::translate("AnnotateWindow", "\345\203\217\347\264\240\345\244\247\345\260\217Y(mm)", nullptr));
        RadioBtnTransverse->setText(QApplication::translate("AnnotateWindow", "\346\250\252\346\226\255\351\235\242", nullptr));
        RadioBtnSagittal->setText(QApplication::translate("AnnotateWindow", "\347\237\242\347\212\266\351\235\242", nullptr));
        BtnSetImageDir->setText(QApplication::translate("AnnotateWindow", "\350\275\275\345\205\245\345\233\276\347\211\207", nullptr));
        BtnLoadPhantomTriangles->setText(QApplication::translate("AnnotateWindow", "\350\275\275\345\205\245\344\275\223\346\250\241N\347\272\277\351\241\266\347\202\271\345\235\220\346\240\207(\344\275\223\346\250\241\345\235\220\346\240\207\347\263\273 \347\211\251\347\220\206\345\235\220\346\240\207)", nullptr));
        BtnFormerImage->setText(QApplication::translate("AnnotateWindow", "\344\270\212\344\270\200\345\274\240", nullptr));
        BtnNextImage->setText(QApplication::translate("AnnotateWindow", "\344\270\213\344\270\200\345\274\240", nullptr));
        BtnFinish->setText(QApplication::translate("AnnotateWindow", "\346\240\207\346\263\250\345\256\214\346\210\220(\345\275\223\345\211\215\345\270\247)", nullptr));
        BtnSave2D->setText(QApplication::translate("AnnotateWindow", "\344\277\235\345\255\230(2D\345\233\276\347\211\207\345\235\220\346\240\207\347\263\273 \345\203\217\347\264\240\345\235\220\346\240\207)", nullptr));
        BtnSave3DInImage->setText(QApplication::translate("AnnotateWindow", "\344\277\235\345\255\230(3D\345\233\276\345\203\217\345\235\220\346\240\207\347\263\273 \347\211\251\347\220\206\345\235\220\346\240\207)", nullptr));
        BtnSave3D->setText(QApplication::translate("AnnotateWindow", "\344\277\235\345\255\230(3D\344\275\223\346\250\241\345\235\220\346\240\207\347\263\273 \347\211\251\347\220\206\345\235\220\346\240\207)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AnnotateWindow: public Ui_AnnotateWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANNOTATEWINDOW_H
