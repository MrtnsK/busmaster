/********************************************************************************
** Form generated from reading UI file 'NodeConfiguration.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NODECONFIGURATION_H
#define UI_NODECONFIGURATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
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

class Ui_NodeConfiguration
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    LDFCommonTableWidget *tableWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NodeConfiguration)
    {
        if (NodeConfiguration->objectName().isEmpty())
            NodeConfiguration->setObjectName(QStringLiteral("NodeConfiguration"));
        NodeConfiguration->resize(571, 364);
        gridLayout_2 = new QGridLayout(NodeConfiguration);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(NodeConfiguration);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(NodeConfiguration);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        groupBox = new QGroupBox(NodeConfiguration);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        btnAdd = new QPushButton(groupBox);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));

        horizontalLayout_2->addWidget(btnAdd);

        btnDelete = new QPushButton(groupBox);
        btnDelete->setObjectName(QStringLiteral("btnDelete"));

        horizontalLayout_2->addWidget(btnDelete);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        tableWidget = new LDFCommonTableWidget(groupBox);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(NodeConfiguration);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(NodeConfiguration);

        QMetaObject::connectSlotsByName(NodeConfiguration);
    } // setupUi

    void retranslateUi(QDialog *NodeConfiguration)
    {
        NodeConfiguration->setWindowTitle(QApplication::translate("NodeConfiguration", "Node Configuration", 0));
        label->setText(QApplication::translate("NodeConfiguration", "Name*", 0));
        groupBox->setTitle(QApplication::translate("NodeConfiguration", "Composite Nodes", 0));
        btnAdd->setText(QApplication::translate("NodeConfiguration", "Add", 0));
        btnDelete->setText(QApplication::translate("NodeConfiguration", "Delete", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("NodeConfiguration", "Node Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("NodeConfiguration", "Logical Nodes", 0));
    } // retranslateUi

};

namespace Ui {
    class NodeConfiguration: public Ui_NodeConfiguration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NODECONFIGURATION_H
