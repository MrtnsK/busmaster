/********************************************************************************
** Form generated from reading UI file 'LDFCLusterPropsDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LDFCLUSTERPROPSDLG_H
#define UI_LDFCLUSTERPROPSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LDFCLusterPropsDlg
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLineEdit *editProtocolVer;
    QLabel *label_4;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QComboBox *comboBaudRate;
    QLineEdit *editLangaugeVer;
    QLabel *labelChannelName;
    QLineEdit *editChannelName;
    QDialogButtonBox *buttonOption;

    void setupUi(QDialog *LDFCLusterPropsDlg)
    {
        if (LDFCLusterPropsDlg->objectName().isEmpty())
            LDFCLusterPropsDlg->setObjectName(QStringLiteral("LDFCLusterPropsDlg"));
        LDFCLusterPropsDlg->resize(315, 154);
        horizontalLayout = new QHBoxLayout(LDFCLusterPropsDlg);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        editProtocolVer = new QLineEdit(LDFCLusterPropsDlg);
        editProtocolVer->setObjectName(QStringLiteral("editProtocolVer"));

        gridLayout->addWidget(editProtocolVer, 0, 1, 1, 1);

        label_4 = new QLabel(LDFCLusterPropsDlg);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 2, 2, 1, 1);

        label_2 = new QLabel(LDFCLusterPropsDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(108, 0));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(LDFCLusterPropsDlg);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(108, 0));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label = new QLabel(LDFCLusterPropsDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(108, 0));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboBaudRate = new QComboBox(LDFCLusterPropsDlg);
        comboBaudRate->setObjectName(QStringLiteral("comboBaudRate"));
        comboBaudRate->setEditable(true);

        gridLayout->addWidget(comboBaudRate, 2, 1, 1, 1);

        editLangaugeVer = new QLineEdit(LDFCLusterPropsDlg);
        editLangaugeVer->setObjectName(QStringLiteral("editLangaugeVer"));

        gridLayout->addWidget(editLangaugeVer, 1, 1, 1, 1);

        labelChannelName = new QLabel(LDFCLusterPropsDlg);
        labelChannelName->setObjectName(QStringLiteral("labelChannelName"));

        gridLayout->addWidget(labelChannelName, 3, 0, 1, 1);

        editChannelName = new QLineEdit(LDFCLusterPropsDlg);
        editChannelName->setObjectName(QStringLiteral("editChannelName"));

        gridLayout->addWidget(editChannelName, 3, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        buttonOption = new QDialogButtonBox(LDFCLusterPropsDlg);
        buttonOption->setObjectName(QStringLiteral("buttonOption"));
        buttonOption->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonOption);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(LDFCLusterPropsDlg);

        QMetaObject::connectSlotsByName(LDFCLusterPropsDlg);
    } // setupUi

    void retranslateUi(QDialog *LDFCLusterPropsDlg)
    {
        LDFCLusterPropsDlg->setWindowTitle(QApplication::translate("LDFCLusterPropsDlg", "LIN Cluster properties", 0));
        label_4->setText(QApplication::translate("LDFCLusterPropsDlg", "kbps", 0));
        label_2->setText(QApplication::translate("LDFCLusterPropsDlg", "LIN Langauge Version", 0));
        label_3->setText(QApplication::translate("LDFCLusterPropsDlg", "LIN Baud Rate", 0));
        label->setText(QApplication::translate("LDFCLusterPropsDlg", "LIN Protocol Version", 0));
        comboBaudRate->clear();
        comboBaudRate->insertItems(0, QStringList()
         << QApplication::translate("LDFCLusterPropsDlg", "2.400", 0)
         << QApplication::translate("LDFCLusterPropsDlg", "9.600", 0)
         << QApplication::translate("LDFCLusterPropsDlg", "10.417", 0)
         << QApplication::translate("LDFCLusterPropsDlg", "19.200", 0)
        );
        labelChannelName->setText(QApplication::translate("LDFCLusterPropsDlg", "Channel Name", 0));
    } // retranslateUi

};

namespace Ui {
    class LDFCLusterPropsDlg: public Ui_LDFCLusterPropsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LDFCLUSTERPROPSDLG_H
