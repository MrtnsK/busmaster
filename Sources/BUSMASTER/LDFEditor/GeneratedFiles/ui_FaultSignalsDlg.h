/********************************************************************************
** Form generated from reading UI file 'FaultSignalsDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FAULTSIGNALSDLG_H
#define UI_FAULTSIGNALSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_FaultSignalsDlg
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    LDFCommonTableWidget *tableSignalList;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *FaultSignalsDlg)
    {
        if (FaultSignalsDlg->objectName().isEmpty())
            FaultSignalsDlg->setObjectName(QStringLiteral("FaultSignalsDlg"));
        FaultSignalsDlg->resize(340, 300);
        horizontalLayout = new QHBoxLayout(FaultSignalsDlg);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableSignalList = new LDFCommonTableWidget(FaultSignalsDlg);
        if (tableSignalList->columnCount() < 1)
            tableSignalList->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignalList->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableSignalList->setObjectName(QStringLiteral("tableSignalList"));

        verticalLayout->addWidget(tableSignalList);

        buttonBox = new QDialogButtonBox(FaultSignalsDlg);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(FaultSignalsDlg);

        QMetaObject::connectSlotsByName(FaultSignalsDlg);
    } // setupUi

    void retranslateUi(QWidget *FaultSignalsDlg)
    {
        FaultSignalsDlg->setWindowTitle(QApplication::translate("FaultSignalsDlg", "FaultSignalsDlg", 0));
        QTableWidgetItem *___qtablewidgetitem = tableSignalList->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("FaultSignalsDlg", "Signal Name", 0));
    } // retranslateUi

};

namespace Ui {
    class FaultSignalsDlg: public Ui_FaultSignalsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FAULTSIGNALSDLG_H
