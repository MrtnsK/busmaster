/********************************************************************************
** Form generated from reading UI file 'EventFrameEditDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTFRAMEEDITDLG_H
#define UI_EVENTFRAMEEDITDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_EventFrameEditDlg
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *lableName;
    QLineEdit *editName;
    QComboBox *comboFrameId;
    QLabel *label_4;
    QComboBox *comboCollisionTable;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    LDFCommonTableWidget *tableFrames;
    QDialogButtonBox *buttonOption;

    void setupUi(QDialog *EventFrameEditDlg)
    {
        if (EventFrameEditDlg->objectName().isEmpty())
            EventFrameEditDlg->setObjectName(QStringLiteral("EventFrameEditDlg"));
        EventFrameEditDlg->resize(661, 459);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/Frame.png"), QSize(), QIcon::Normal, QIcon::Off);
        EventFrameEditDlg->setWindowIcon(icon);
        horizontalLayout_3 = new QHBoxLayout(EventFrameEditDlg);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(EventFrameEditDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lableName = new QLabel(groupBox);
        lableName->setObjectName(QStringLiteral("lableName"));
        lableName->setMinimumSize(QSize(135, 0));

        gridLayout->addWidget(lableName, 0, 0, 1, 1);

        editName = new QLineEdit(groupBox);
        editName->setObjectName(QStringLiteral("editName"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);

        gridLayout->addWidget(editName, 0, 1, 1, 1);

        comboFrameId = new QComboBox(groupBox);
        comboFrameId->setObjectName(QStringLiteral("comboFrameId"));
        sizePolicy.setHeightForWidth(comboFrameId->sizePolicy().hasHeightForWidth());
        comboFrameId->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboFrameId, 0, 4, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(135, 0));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        comboCollisionTable = new QComboBox(groupBox);
        comboCollisionTable->setObjectName(QStringLiteral("comboCollisionTable"));

        gridLayout->addWidget(comboCollisionTable, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(135, 0));

        gridLayout->addWidget(label_2, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(120, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(EventFrameEditDlg);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tableFrames = new LDFCommonTableWidget(groupBox_2);
        if (tableFrames->columnCount() < 5)
            tableFrames->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableFrames->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableFrames->setObjectName(QStringLiteral("tableFrames"));

        horizontalLayout_2->addWidget(tableFrames);


        verticalLayout->addWidget(groupBox_2);

        buttonOption = new QDialogButtonBox(EventFrameEditDlg);
        buttonOption->setObjectName(QStringLiteral("buttonOption"));
        buttonOption->setOrientation(Qt::Horizontal);
        buttonOption->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonOption);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(EventFrameEditDlg);

        QMetaObject::connectSlotsByName(EventFrameEditDlg);
    } // setupUi

    void retranslateUi(QDialog *EventFrameEditDlg)
    {
        EventFrameEditDlg->setWindowTitle(QApplication::translate("EventFrameEditDlg", "Create Event Triggered Frame", 0));
        groupBox->setTitle(QApplication::translate("EventFrameEditDlg", "General Properties", 0));
        lableName->setText(QApplication::translate("EventFrameEditDlg", "Name*", 0));
        label_4->setText(QApplication::translate("EventFrameEditDlg", "Collision Resolution Table", 0));
        label_2->setText(QApplication::translate("EventFrameEditDlg", "ID", 0));
        groupBox_2->setTitle(QApplication::translate("EventFrameEditDlg", "Associated Frames", 0));
        QTableWidgetItem *___qtablewidgetitem = tableFrames->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("EventFrameEditDlg", "Frame Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableFrames->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("EventFrameEditDlg", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableFrames->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("EventFrameEditDlg", "Length (Byte(s))", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableFrames->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("EventFrameEditDlg", "Publishers", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableFrames->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("EventFrameEditDlg", "Subscribers", 0));
    } // retranslateUi

};

namespace Ui {
    class EventFrameEditDlg: public Ui_EventFrameEditDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTFRAMEEDITDLG_H
