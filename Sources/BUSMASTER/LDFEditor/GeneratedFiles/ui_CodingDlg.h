/********************************************************************************
** Form generated from reading UI file 'CodingDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CODINGDLG_H
#define UI_CODINGDLG_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_CodingDlg
{
public:
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    LDFCommonTableWidget *tableLogicalValues;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonAddLogical;
    QPushButton *buttonDeleteLogical;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    LDFCommonTableWidget *tablePhysicalValues;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonAddPhysical;
    QPushButton *buttonDeletePhysical;
    QDialogButtonBox *buttonOption;

    void setupUi(QDialog *CodingDlg)
    {
        if (CodingDlg->objectName().isEmpty())
            CodingDlg->setObjectName(QStringLiteral("CodingDlg"));
        CodingDlg->resize(568, 616);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/32x32/Coding.png"), QSize(), QIcon::Normal, QIcon::Off);
        CodingDlg->setWindowIcon(icon);
        horizontalLayout_7 = new QHBoxLayout(CodingDlg);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(CodingDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(groupBox);
        editName->setObjectName(QStringLiteral("editName"));
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);
        editName->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(editName);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CodingDlg);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_4 = new QHBoxLayout(groupBox_2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableLogicalValues = new LDFCommonTableWidget(groupBox_2);
        if (tableLogicalValues->columnCount() < 2)
            tableLogicalValues->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableLogicalValues->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableLogicalValues->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableLogicalValues->setObjectName(QStringLiteral("tableLogicalValues"));

        verticalLayout->addWidget(tableLogicalValues);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        buttonAddLogical = new QPushButton(groupBox_2);
        buttonAddLogical->setObjectName(QStringLiteral("buttonAddLogical"));

        horizontalLayout_3->addWidget(buttonAddLogical);

        buttonDeleteLogical = new QPushButton(groupBox_2);
        buttonDeleteLogical->setObjectName(QStringLiteral("buttonDeleteLogical"));

        horizontalLayout_3->addWidget(buttonDeleteLogical);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_4->addLayout(verticalLayout);


        verticalLayout_3->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(CodingDlg);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_6 = new QHBoxLayout(groupBox_3);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tablePhysicalValues = new LDFCommonTableWidget(groupBox_3);
        if (tablePhysicalValues->columnCount() < 5)
            tablePhysicalValues->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tablePhysicalValues->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tablePhysicalValues->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tablePhysicalValues->setHorizontalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tablePhysicalValues->setHorizontalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tablePhysicalValues->setHorizontalHeaderItem(4, __qtablewidgetitem6);
        tablePhysicalValues->setObjectName(QStringLiteral("tablePhysicalValues"));

        verticalLayout_2->addWidget(tablePhysicalValues);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        buttonAddPhysical = new QPushButton(groupBox_3);
        buttonAddPhysical->setObjectName(QStringLiteral("buttonAddPhysical"));

        horizontalLayout_5->addWidget(buttonAddPhysical);

        buttonDeletePhysical = new QPushButton(groupBox_3);
        buttonDeletePhysical->setObjectName(QStringLiteral("buttonDeletePhysical"));

        horizontalLayout_5->addWidget(buttonDeletePhysical);


        verticalLayout_2->addLayout(horizontalLayout_5);


        horizontalLayout_6->addLayout(verticalLayout_2);


        verticalLayout_3->addWidget(groupBox_3);

        buttonOption = new QDialogButtonBox(CodingDlg);
        buttonOption->setObjectName(QStringLiteral("buttonOption"));
        buttonOption->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonOption);


        horizontalLayout_7->addLayout(verticalLayout_3);


        retranslateUi(CodingDlg);

        QMetaObject::connectSlotsByName(CodingDlg);
    } // setupUi

    void retranslateUi(QDialog *CodingDlg)
    {
        CodingDlg->setWindowTitle(QApplication::translate("CodingDlg", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("CodingDlg", "General Properties", 0));
        label->setText(QApplication::translate("CodingDlg", "Name*", 0));
        groupBox_2->setTitle(QApplication::translate("CodingDlg", "Logical Values:", 0));
        QTableWidgetItem *___qtablewidgetitem = tableLogicalValues->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CodingDlg", "Raw Value", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableLogicalValues->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CodingDlg", "Description", 0));
        buttonAddLogical->setText(QApplication::translate("CodingDlg", "Add", 0));
        buttonDeleteLogical->setText(QApplication::translate("CodingDlg", "Delete", 0));
        groupBox_3->setTitle(QApplication::translate("CodingDlg", "Physical Values:", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tablePhysicalValues->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("CodingDlg", "Offset", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tablePhysicalValues->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("CodingDlg", "Factor", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tablePhysicalValues->horizontalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("CodingDlg", "Minimum", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tablePhysicalValues->horizontalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("CodingDlg", "Maximum", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tablePhysicalValues->horizontalHeaderItem(4);
        ___qtablewidgetitem6->setText(QApplication::translate("CodingDlg", "Unit", 0));
        buttonAddPhysical->setText(QApplication::translate("CodingDlg", "Add", 0));
        buttonDeletePhysical->setText(QApplication::translate("CodingDlg", "Delete", 0));
    } // retranslateUi

};

namespace Ui {
    class CodingDlg: public Ui_CodingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CODINGDLG_H
