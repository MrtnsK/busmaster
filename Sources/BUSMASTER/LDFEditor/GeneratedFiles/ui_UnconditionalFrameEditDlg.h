/********************************************************************************
** Form generated from reading UI file 'UnconditionalFrameEditDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNCONDITIONALFRAMEEDITDLG_H
#define UI_UNCONDITIONALFRAMEEDITDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_UnConditionalFrameEditDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_6;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLabel *label;
    QComboBox *comboFrameLength;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboFrameId;
    QLineEdit *editFrameName;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QCheckBox *chkDynFrame;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QComboBox *comboPublisher;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_5;
    QListWidget *listSubscribers;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout_4;
    QPushButton *btnMapSignal;
    QSpacerItem *horizontalSpacer_6;
    LDFCommonTableWidget *tableSignals;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_4;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UnConditionalFrameEditDlg)
    {
        if (UnConditionalFrameEditDlg->objectName().isEmpty())
            UnConditionalFrameEditDlg->setObjectName(QStringLiteral("UnConditionalFrameEditDlg"));
        UnConditionalFrameEditDlg->resize(643, 569);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/Frame.png"), QSize(), QIcon::Normal, QIcon::Off);
        UnConditionalFrameEditDlg->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(UnConditionalFrameEditDlg);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(UnConditionalFrameEditDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_6 = new QHBoxLayout(groupBox);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(130, 0));

        gridLayout_2->addWidget(label_2, 0, 4, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(130, 0));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        comboFrameLength = new QComboBox(groupBox);
        comboFrameLength->setObjectName(QStringLiteral("comboFrameLength"));
        sizePolicy.setHeightForWidth(comboFrameLength->sizePolicy().hasHeightForWidth());
        comboFrameLength->setSizePolicy(sizePolicy);
        comboFrameLength->setMinimumSize(QSize(133, 0));
        comboFrameLength->setMaxVisibleItems(8);
        comboFrameLength->setMaxCount(8);

        gridLayout_2->addWidget(comboFrameLength, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(130, 0));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);

        comboFrameId = new QComboBox(groupBox);
        comboFrameId->setObjectName(QStringLiteral("comboFrameId"));
        sizePolicy.setHeightForWidth(comboFrameId->sizePolicy().hasHeightForWidth());
        comboFrameId->setSizePolicy(sizePolicy);
        comboFrameId->setMaxVisibleItems(10);
        comboFrameId->setMaxCount(61);

        gridLayout_2->addWidget(comboFrameId, 0, 5, 1, 1);

        editFrameName = new QLineEdit(groupBox);
        editFrameName->setObjectName(QStringLiteral("editFrameName"));
        sizePolicy.setHeightForWidth(editFrameName->sizePolicy().hasHeightForWidth());
        editFrameName->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(editFrameName, 0, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        gridLayout_2->addLayout(horizontalLayout, 0, 3, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        gridLayout_2->addLayout(horizontalLayout_3, 1, 3, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 1, 2, 1, 1);

        chkDynFrame = new QCheckBox(groupBox);
        chkDynFrame->setObjectName(QStringLiteral("chkDynFrame"));
        sizePolicy.setHeightForWidth(chkDynFrame->sizePolicy().hasHeightForWidth());
        chkDynFrame->setSizePolicy(sizePolicy);
        chkDynFrame->setMinimumSize(QSize(130, 0));

        gridLayout_2->addWidget(chkDynFrame, 1, 4, 1, 1);


        horizontalLayout_6->addLayout(gridLayout_2);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(UnConditionalFrameEditDlg);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(130, 0));

        horizontalLayout_2->addWidget(label_4);

        comboPublisher = new QComboBox(groupBox_3);
        comboPublisher->setObjectName(QStringLiteral("comboPublisher"));
        sizePolicy.setHeightForWidth(comboPublisher->sizePolicy().hasHeightForWidth());
        comboPublisher->setSizePolicy(sizePolicy);
        comboPublisher->setMinimumSize(QSize(133, 0));

        horizontalLayout_2->addWidget(comboPublisher);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setMinimumSize(QSize(60, 0));

        horizontalLayout_2->addWidget(label_5);

        listSubscribers = new QListWidget(groupBox_3);
        listSubscribers->setObjectName(QStringLiteral("listSubscribers"));
        sizePolicy1.setHeightForWidth(listSubscribers->sizePolicy().hasHeightForWidth());
        listSubscribers->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(listSubscribers);


        horizontalLayout_4->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(UnConditionalFrameEditDlg);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_2);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        btnMapSignal = new QPushButton(groupBox_2);
        btnMapSignal->setObjectName(QStringLiteral("btnMapSignal"));
        sizePolicy.setHeightForWidth(btnMapSignal->sizePolicy().hasHeightForWidth());
        btnMapSignal->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(btnMapSignal, 1, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_6, 1, 0, 1, 1);

        tableSignals = new LDFCommonTableWidget(groupBox_2);
        if (tableSignals->columnCount() < 6)
            tableSignals->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableSignals->setObjectName(QStringLiteral("tableSignals"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tableSignals->sizePolicy().hasHeightForWidth());
        tableSignals->setSizePolicy(sizePolicy3);
        tableSignals->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_4->addWidget(tableSignals, 0, 0, 1, 2);


        horizontalLayout_5->addLayout(gridLayout_4);


        verticalLayout->addWidget(groupBox_2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(UnConditionalFrameEditDlg);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        sizePolicy.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        groupBox_2->raise();
        groupBox->raise();
        groupBox_3->raise();

        retranslateUi(UnConditionalFrameEditDlg);

        QMetaObject::connectSlotsByName(UnConditionalFrameEditDlg);
    } // setupUi

    void retranslateUi(QDialog *UnConditionalFrameEditDlg)
    {
        UnConditionalFrameEditDlg->setWindowTitle(QApplication::translate("UnConditionalFrameEditDlg", "Edit Unconditional Frame - Frame1", 0));
        groupBox->setTitle(QApplication::translate("UnConditionalFrameEditDlg", "General Properties", 0));
        label_2->setText(QApplication::translate("UnConditionalFrameEditDlg", "ID*", 0));
        label->setText(QApplication::translate("UnConditionalFrameEditDlg", "Name*", 0));
        label_3->setText(QApplication::translate("UnConditionalFrameEditDlg", "Length*", 0));
        label_6->setText(QApplication::translate("UnConditionalFrameEditDlg", "Byte(s)", 0));
        chkDynFrame->setText(QApplication::translate("UnConditionalFrameEditDlg", "Dynamic Frame", 0));
        groupBox_3->setTitle(QApplication::translate("UnConditionalFrameEditDlg", "Tx\\Rx Relations", 0));
        label_4->setText(QApplication::translate("UnConditionalFrameEditDlg", "Publisher*", 0));
        label_5->setText(QApplication::translate("UnConditionalFrameEditDlg", "Subcribers", 0));
        groupBox_2->setTitle(QApplication::translate("UnConditionalFrameEditDlg", "Signal Map", 0));
        btnMapSignal->setText(QApplication::translate("UnConditionalFrameEditDlg", "Map Signal", 0));
        QTableWidgetItem *___qtablewidgetitem = tableSignals->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("UnConditionalFrameEditDlg", "Signal Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableSignals->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("UnConditionalFrameEditDlg", "Start Bit", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableSignals->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("UnConditionalFrameEditDlg", "Subscriber (s)", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableSignals->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("UnConditionalFrameEditDlg", "Length (bits)", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableSignals->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("UnConditionalFrameEditDlg", "Init Value", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableSignals->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("UnConditionalFrameEditDlg", "Coding", 0));
    } // retranslateUi

};

namespace Ui {
    class UnConditionalFrameEditDlg: public Ui_UnConditionalFrameEditDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNCONDITIONALFRAMEEDITDLG_H
