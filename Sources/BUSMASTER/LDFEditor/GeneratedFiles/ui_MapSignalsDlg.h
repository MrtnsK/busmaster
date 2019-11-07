/********************************************************************************
** Form generated from reading UI file 'MapSignalsDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPSIGNALSDLG_H
#define UI_MAPSIGNALSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include "ldfcommontablewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MapSignalsDlg
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *Signals;
    QGridLayout *gridLayout_2;
    LDFCommonTableWidget *tableSignals;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MapSignalsDlg)
    {
        if (MapSignalsDlg->objectName().isEmpty())
            MapSignalsDlg->setObjectName(QStringLiteral("MapSignalsDlg"));
        MapSignalsDlg->resize(719, 461);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/Signals.png"), QSize(), QIcon::Normal, QIcon::Off);
        MapSignalsDlg->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(MapSignalsDlg);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Signals = new QGroupBox(MapSignalsDlg);
        Signals->setObjectName(QStringLiteral("Signals"));
        gridLayout_2 = new QGridLayout(Signals);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tableSignals = new LDFCommonTableWidget(Signals);
        if (tableSignals->columnCount() < 3)
            tableSignals->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableSignals->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableSignals->setObjectName(QStringLiteral("tableSignals"));
        tableSignals->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_2->addWidget(tableSignals, 0, 0, 1, 1);


        verticalLayout->addWidget(Signals);

        buttonBox = new QDialogButtonBox(MapSignalsDlg);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(MapSignalsDlg);

        QMetaObject::connectSlotsByName(MapSignalsDlg);
    } // setupUi

    void retranslateUi(QDialog *MapSignalsDlg)
    {
        MapSignalsDlg->setWindowTitle(QApplication::translate("MapSignalsDlg", "Signal Mapping", 0));
        Signals->setTitle(QApplication::translate("MapSignalsDlg", "Signal Selection", 0));
        QTableWidgetItem *___qtablewidgetitem = tableSignals->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MapSignalsDlg", "Name", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableSignals->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MapSignalsDlg", "Start Bit", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableSignals->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MapSignalsDlg", "Length", 0));
    } // retranslateUi

};

namespace Ui {
    class MapSignalsDlg: public Ui_MapSignalsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPSIGNALSDLG_H
