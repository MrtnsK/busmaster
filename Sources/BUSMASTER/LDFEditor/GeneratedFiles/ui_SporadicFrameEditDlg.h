/********************************************************************************
** Form generated from reading UI file 'SporadicFrameEditDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPORADICFRAMEEDITDLG_H
#define UI_SPORADICFRAMEEDITDLG_H

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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_SporadicFrameEditDlg
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *editFrameName;
    QLabel *lableName;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    LDFCommonTableWidget *tableFrames;
    QDialogButtonBox *buttonOk;

    void setupUi(QDialog *SporadicFrameEditDlg)
    {
        if (SporadicFrameEditDlg->objectName().isEmpty())
            SporadicFrameEditDlg->setObjectName(QStringLiteral("SporadicFrameEditDlg"));
        SporadicFrameEditDlg->resize(666, 455);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SporadicFrameEditDlg->sizePolicy().hasHeightForWidth());
        SporadicFrameEditDlg->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/Frame.png"), QSize(), QIcon::Normal, QIcon::Off);
        SporadicFrameEditDlg->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(SporadicFrameEditDlg);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(SporadicFrameEditDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        editFrameName = new QLineEdit(groupBox);
        editFrameName->setObjectName(QStringLiteral("editFrameName"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(editFrameName->sizePolicy().hasHeightForWidth());
        editFrameName->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(editFrameName, 0, 1, 1, 1);

        lableName = new QLabel(groupBox);
        lableName->setObjectName(QStringLiteral("lableName"));
        sizePolicy1.setHeightForWidth(lableName->sizePolicy().hasHeightForWidth());
        lableName->setSizePolicy(sizePolicy1);
        lableName->setMinimumSize(QSize(135, 0));

        gridLayout->addWidget(lableName, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(SporadicFrameEditDlg);
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

        buttonOk = new QDialogButtonBox(SporadicFrameEditDlg);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));
        buttonOk->setOrientation(Qt::Horizontal);
        buttonOk->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonOk);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(SporadicFrameEditDlg);

        QMetaObject::connectSlotsByName(SporadicFrameEditDlg);
    } // setupUi

    void retranslateUi(QDialog *SporadicFrameEditDlg)
    {
        SporadicFrameEditDlg->setWindowTitle(QApplication::translate("SporadicFrameEditDlg", "Create Sporadic Frame", 0));
        groupBox->setTitle(QApplication::translate("SporadicFrameEditDlg", "General Properties", 0));
        lableName->setText(QApplication::translate("SporadicFrameEditDlg", "Name*", 0));
        groupBox_2->setTitle(QApplication::translate("SporadicFrameEditDlg", "Associated Frames", 0));
        QTableWidgetItem *___qtablewidgetitem = tableFrames->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("SporadicFrameEditDlg", "Frame Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableFrames->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("SporadicFrameEditDlg", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableFrames->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("SporadicFrameEditDlg", "Length (Byte(s))", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableFrames->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("SporadicFrameEditDlg", "Publishers", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableFrames->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("SporadicFrameEditDlg", "Subscribers", 0));
    } // retranslateUi

};

namespace Ui {
    class SporadicFrameEditDlg: public Ui_SporadicFrameEditDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPORADICFRAMEEDITDLG_H
