/********************************************************************************
** Form generated from reading UI file 'LocatePhantomDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCATEPHANTOMDLG_H
#define UI_LOCATEPHANTOMDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LocatePhantomDlg
{
public:
    QHBoxLayout *horizontalLayout_3;
    QPlainTextEdit *LocatedPts;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *Label;
    QSpacerItem *horizontalSpacer;
    QSpinBox *SensorNumber;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *BtnRecord;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *BtnRecordRepeat;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *BtnDelete;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *BtnSave;

    void setupUi(QDialog *LocatePhantomDlg)
    {
        if (LocatePhantomDlg->objectName().isEmpty())
            LocatePhantomDlg->setObjectName(QString::fromUtf8("LocatePhantomDlg"));
        LocatePhantomDlg->resize(837, 457);
        horizontalLayout_3 = new QHBoxLayout(LocatePhantomDlg);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        LocatedPts = new QPlainTextEdit(LocatePhantomDlg);
        LocatedPts->setObjectName(QString::fromUtf8("LocatedPts"));

        horizontalLayout_3->addWidget(LocatedPts);

        groupBox = new QGroupBox(LocatePhantomDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        Label = new QLabel(groupBox);
        Label->setObjectName(QString::fromUtf8("Label"));

        horizontalLayout->addWidget(Label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        SensorNumber = new QSpinBox(groupBox);
        SensorNumber->setObjectName(QString::fromUtf8("SensorNumber"));
        SensorNumber->setMinimum(-1);

        horizontalLayout->addWidget(SensorNumber);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        BtnRecord = new QPushButton(groupBox);
        BtnRecord->setObjectName(QString::fromUtf8("BtnRecord"));

        horizontalLayout_2->addWidget(BtnRecord);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        BtnRecordRepeat = new QPushButton(groupBox);
        BtnRecordRepeat->setObjectName(QString::fromUtf8("BtnRecordRepeat"));

        horizontalLayout_2->addWidget(BtnRecordRepeat);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        BtnDelete = new QPushButton(groupBox);
        BtnDelete->setObjectName(QString::fromUtf8("BtnDelete"));

        horizontalLayout_2->addWidget(BtnDelete);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        BtnSave = new QPushButton(groupBox);
        BtnSave->setObjectName(QString::fromUtf8("BtnSave"));

        horizontalLayout_2->addWidget(BtnSave);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addWidget(groupBox);


        retranslateUi(LocatePhantomDlg);

        QMetaObject::connectSlotsByName(LocatePhantomDlg);
    } // setupUi

    void retranslateUi(QDialog *LocatePhantomDlg)
    {
        LocatePhantomDlg->setWindowTitle(QApplication::translate("LocatePhantomDlg", "LocatePhantomDlg", nullptr));
        groupBox->setTitle(QString());
        Label->setText(QApplication::translate("LocatePhantomDlg", "\344\274\240\346\204\237\345\231\250\345\272\217\345\217\267", nullptr));
        BtnRecord->setText(QApplication::translate("LocatePhantomDlg", "\350\256\260\345\275\225(\345\215\225\346\254\241)", nullptr));
        BtnRecordRepeat->setText(QApplication::translate("LocatePhantomDlg", "\350\256\260\345\275\225(\345\244\232\346\254\241)", nullptr));
        BtnDelete->setText(QApplication::translate("LocatePhantomDlg", "\345\210\240\351\231\244(\345\215\225\346\254\241)", nullptr));
        BtnSave->setText(QApplication::translate("LocatePhantomDlg", "\345\255\230\345\202\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LocatePhantomDlg: public Ui_LocatePhantomDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCATEPHANTOMDLG_H
