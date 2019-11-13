/********************************************************************************
** Form generated from reading UI file 'ScheduleTableDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCHEDULETABLEDLG_H
#define UI_SCHEDULETABLEDLG_H

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

class Ui_ScheduleTableDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editScheduleName;
    QSpacerItem *horizontalSpacer;
    LDFCommonTableWidget *tableFrames;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    LDFCommonTableWidget *tableProperties;
    QDialogButtonBox *buttonOk;

    void setupUi(QDialog *ScheduleTableDlg)
    {
        if (ScheduleTableDlg->objectName().isEmpty())
            ScheduleTableDlg->setObjectName(QStringLiteral("ScheduleTableDlg"));
        ScheduleTableDlg->resize(752, 570);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/ScheduleTable.png"), QSize(), QIcon::Normal, QIcon::Off);
        ScheduleTableDlg->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(ScheduleTableDlg);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        groupBox = new QGroupBox(ScheduleTableDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        editScheduleName = new QLineEdit(groupBox);
        editScheduleName->setObjectName(QStringLiteral("editScheduleName"));

        horizontalLayout->addWidget(editScheduleName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        tableFrames = new LDFCommonTableWidget(groupBox);
        if (tableFrames->columnCount() < 4)
            tableFrames->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableFrames->setObjectName(QStringLiteral("tableFrames"));
        tableFrames->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableFrames->setTabKeyNavigation(true);
        tableFrames->setColumnCount(4);

        verticalLayout->addWidget(tableFrames);

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


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(ScheduleTableDlg);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tableProperties = new LDFCommonTableWidget(groupBox_2);
        if (tableProperties->columnCount() < 2)
            tableProperties->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableProperties->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableProperties->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        tableProperties->setObjectName(QStringLiteral("tableProperties"));
        tableProperties->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableProperties->setColumnCount(2);

        gridLayout_2->addWidget(tableProperties, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 1);

        buttonOk = new QDialogButtonBox(ScheduleTableDlg);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));
        buttonOk->setOrientation(Qt::Horizontal);
        buttonOk->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_3->addWidget(buttonOk, 2, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_3);


        retranslateUi(ScheduleTableDlg);

        QMetaObject::connectSlotsByName(ScheduleTableDlg);
    } // setupUi

    void retranslateUi(QDialog *ScheduleTableDlg)
    {
        ScheduleTableDlg->setWindowTitle(QApplication::translate("ScheduleTableDlg", "Create Event Triggered Frame", 0));
        groupBox->setTitle(QApplication::translate("ScheduleTableDlg", "Schedule Table(s)", 0));
        label->setText(QApplication::translate("ScheduleTableDlg", "Name*", 0));
        QTableWidgetItem *___qtablewidgetitem = tableFrames->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ScheduleTableDlg", "Frame Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableFrames->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ScheduleTableDlg", "Frame Type", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableFrames->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ScheduleTableDlg", "Length", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableFrames->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ScheduleTableDlg", "Delay [ms]*", 0));
        btnAdd->setText(QApplication::translate("ScheduleTableDlg", "Add", 0));
        btnDelete->setText(QApplication::translate("ScheduleTableDlg", "Delete", 0));
        groupBox_2->setTitle(QApplication::translate("ScheduleTableDlg", "Properties", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("ScheduleTableDlg", "Name", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("ScheduleTableDlg", "Value", 0));
    } // retranslateUi

};

namespace Ui {
    class ScheduleTableDlg: public Ui_ScheduleTableDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCHEDULETABLEDLG_H
