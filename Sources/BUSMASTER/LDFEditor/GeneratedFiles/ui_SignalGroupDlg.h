/********************************************************************************
** Form generated from reading UI file 'SignalGroupDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNALGROUPDLG_H
#define UI_SIGNALGROUPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_SignalGroupDlg
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QLineEdit *editSize;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    LDFCommonTableWidget *tableSignals;
    QDialogButtonBox *buttonOption;

    void setupUi(QDialog *SignalGroupDlg)
    {
        if (SignalGroupDlg->objectName().isEmpty())
            SignalGroupDlg->setObjectName(QStringLiteral("SignalGroupDlg"));
        SignalGroupDlg->resize(390, 345);
        horizontalLayout_3 = new QHBoxLayout(SignalGroupDlg);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(SignalGroupDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(SignalGroupDlg);
        editName->setObjectName(QStringLiteral("editName"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(editName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_2 = new QLabel(SignalGroupDlg);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        editSize = new QLineEdit(SignalGroupDlg);
        editSize->setObjectName(QStringLiteral("editSize"));

        horizontalLayout->addWidget(editSize);


        verticalLayout->addLayout(horizontalLayout);

        groupBox = new QGroupBox(SignalGroupDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tableSignals = new LDFCommonTableWidget(groupBox);
        if (tableSignals->columnCount() < 2)
            tableSignals->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableSignals->setObjectName(QStringLiteral("tableSignals"));

        horizontalLayout_2->addWidget(tableSignals);


        verticalLayout->addWidget(groupBox);

        buttonOption = new QDialogButtonBox(SignalGroupDlg);
        buttonOption->setObjectName(QStringLiteral("buttonOption"));
        buttonOption->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonOption);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(SignalGroupDlg);

        QMetaObject::connectSlotsByName(SignalGroupDlg);
    } // setupUi

    void retranslateUi(QDialog *SignalGroupDlg)
    {
        SignalGroupDlg->setWindowTitle(QApplication::translate("SignalGroupDlg", "SignalGroupDlg", 0));
        label->setText(QApplication::translate("SignalGroupDlg", "Name*", 0));
        label_2->setText(QApplication::translate("SignalGroupDlg", "Size ( decimal )*", 0));
        groupBox->setTitle(QApplication::translate("SignalGroupDlg", "Signals", 0));
        QTableWidgetItem *___qtablewidgetitem = tableSignals->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("SignalGroupDlg", "Signal Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableSignals->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("SignalGroupDlg", "Offset ( decimal )", 0));
    } // retranslateUi

};

namespace Ui {
    class SignalGroupDlg: public Ui_SignalGroupDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNALGROUPDLG_H
