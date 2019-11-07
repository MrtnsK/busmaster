/********************************************************************************
** Form generated from reading UI file 'SlaveDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLAVEDLG_H
#define UI_SLAVEDLG_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SlaveDlg
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupProperties;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_10;
    QLabel *labelName;
    QLineEdit *editName;
    QSpacerItem *horizontalSpacer_6;
    QLabel *labelProtocol;
    QComboBox *comboProtocol;
    QGroupBox *groupNodeAddress;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelConfigNAD;
    QLineEdit *editConfigNAD;
    QSpacerItem *horizontalSpacer;
    QLabel *labelInitialNAD;
    QLineEdit *editInitialNAD;
    QGroupBox *groupDiagTime;
    QHBoxLayout *horizontalLayout_6;
    QGridLayout *gridLayout;
    QLabel *labelNcr;
    QLineEdit *editP2MIN;
    QLabel *labelP2Min;
    QLineEdit *editNAS;
    QLabel *labelSTMin;
    QLineEdit *editNCR;
    QLabel *labelNAs;
    QLineEdit *editSTMIN;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupProductId;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout_2;
    QLineEdit *editVariant;
    QLabel *labelVariant;
    QLineEdit *editSupplierID;
    QLabel *labelSupplierId;
    QLineEdit *editFunctionId;
    QLabel *labelFunctionId;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *grpRespErr;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_31;
    QLabel *labelRespErr;
    QComboBox *comboRespErr;
    QSpacerItem *horizontalSpacer_7;
    QGroupBox *groupFaultStateSignals;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *editFaultStateSignals;
    QPushButton *buttonFaultSignals;
    QDialogButtonBox *buttonOption;

    void setupUi(QDialog *SlaveDlg)
    {
        if (SlaveDlg->objectName().isEmpty())
            SlaveDlg->setObjectName(QStringLiteral("SlaveDlg"));
        SlaveDlg->setWindowModality(Qt::NonModal);
        SlaveDlg->resize(698, 403);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SlaveDlg->sizePolicy().hasHeightForWidth());
        SlaveDlg->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/Ecu.png"), QSize(), QIcon::Normal, QIcon::Off);
        SlaveDlg->setWindowIcon(icon);
        horizontalLayout_3 = new QHBoxLayout(SlaveDlg);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        groupProperties = new QGroupBox(SlaveDlg);
        groupProperties->setObjectName(QStringLiteral("groupProperties"));
        horizontalLayout_9 = new QHBoxLayout(groupProperties);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        labelName = new QLabel(groupProperties);
        labelName->setObjectName(QStringLiteral("labelName"));
        sizePolicy.setHeightForWidth(labelName->sizePolicy().hasHeightForWidth());
        labelName->setSizePolicy(sizePolicy);
        labelName->setMinimumSize(QSize(133, 0));

        horizontalLayout_10->addWidget(labelName);

        editName = new QLineEdit(groupProperties);
        editName->setObjectName(QStringLiteral("editName"));
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(editName);

        horizontalSpacer_6 = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        labelProtocol = new QLabel(groupProperties);
        labelProtocol->setObjectName(QStringLiteral("labelProtocol"));
        sizePolicy.setHeightForWidth(labelProtocol->sizePolicy().hasHeightForWidth());
        labelProtocol->setSizePolicy(sizePolicy);
        labelProtocol->setMinimumSize(QSize(133, 0));

        horizontalLayout_10->addWidget(labelProtocol);

        comboProtocol = new QComboBox(groupProperties);
        comboProtocol->setObjectName(QStringLiteral("comboProtocol"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboProtocol->sizePolicy().hasHeightForWidth());
        comboProtocol->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(comboProtocol);


        horizontalLayout_9->addLayout(horizontalLayout_10);


        verticalLayout->addWidget(groupProperties);

        groupNodeAddress = new QGroupBox(SlaveDlg);
        groupNodeAddress->setObjectName(QStringLiteral("groupNodeAddress"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupNodeAddress->sizePolicy().hasHeightForWidth());
        groupNodeAddress->setSizePolicy(sizePolicy2);
        horizontalLayout_8 = new QHBoxLayout(groupNodeAddress);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        labelConfigNAD = new QLabel(groupNodeAddress);
        labelConfigNAD->setObjectName(QStringLiteral("labelConfigNAD"));
        sizePolicy.setHeightForWidth(labelConfigNAD->sizePolicy().hasHeightForWidth());
        labelConfigNAD->setSizePolicy(sizePolicy);
        labelConfigNAD->setMinimumSize(QSize(133, 0));

        horizontalLayout_4->addWidget(labelConfigNAD);

        editConfigNAD = new QLineEdit(groupNodeAddress);
        editConfigNAD->setObjectName(QStringLiteral("editConfigNAD"));
        sizePolicy.setHeightForWidth(editConfigNAD->sizePolicy().hasHeightForWidth());
        editConfigNAD->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(editConfigNAD);

        horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        labelInitialNAD = new QLabel(groupNodeAddress);
        labelInitialNAD->setObjectName(QStringLiteral("labelInitialNAD"));
        sizePolicy.setHeightForWidth(labelInitialNAD->sizePolicy().hasHeightForWidth());
        labelInitialNAD->setSizePolicy(sizePolicy);
        labelInitialNAD->setMinimumSize(QSize(133, 0));

        horizontalLayout_4->addWidget(labelInitialNAD);

        editInitialNAD = new QLineEdit(groupNodeAddress);
        editInitialNAD->setObjectName(QStringLiteral("editInitialNAD"));
        sizePolicy.setHeightForWidth(editInitialNAD->sizePolicy().hasHeightForWidth());
        editInitialNAD->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(editInitialNAD);


        horizontalLayout_8->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(groupNodeAddress);

        groupDiagTime = new QGroupBox(SlaveDlg);
        groupDiagTime->setObjectName(QStringLiteral("groupDiagTime"));
        horizontalLayout_6 = new QHBoxLayout(groupDiagTime);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelNcr = new QLabel(groupDiagTime);
        labelNcr->setObjectName(QStringLiteral("labelNcr"));
        sizePolicy1.setHeightForWidth(labelNcr->sizePolicy().hasHeightForWidth());
        labelNcr->setSizePolicy(sizePolicy1);
        labelNcr->setMinimumSize(QSize(133, 0));

        gridLayout->addWidget(labelNcr, 1, 0, 1, 1);

        editP2MIN = new QLineEdit(groupDiagTime);
        editP2MIN->setObjectName(QStringLiteral("editP2MIN"));
        sizePolicy.setHeightForWidth(editP2MIN->sizePolicy().hasHeightForWidth());
        editP2MIN->setSizePolicy(sizePolicy);

        gridLayout->addWidget(editP2MIN, 0, 4, 1, 1);

        labelP2Min = new QLabel(groupDiagTime);
        labelP2Min->setObjectName(QStringLiteral("labelP2Min"));
        sizePolicy1.setHeightForWidth(labelP2Min->sizePolicy().hasHeightForWidth());
        labelP2Min->setSizePolicy(sizePolicy1);
        labelP2Min->setMinimumSize(QSize(133, 0));

        gridLayout->addWidget(labelP2Min, 0, 3, 1, 1);

        editNAS = new QLineEdit(groupDiagTime);
        editNAS->setObjectName(QStringLiteral("editNAS"));
        sizePolicy.setHeightForWidth(editNAS->sizePolicy().hasHeightForWidth());
        editNAS->setSizePolicy(sizePolicy);

        gridLayout->addWidget(editNAS, 0, 1, 1, 1);

        labelSTMin = new QLabel(groupDiagTime);
        labelSTMin->setObjectName(QStringLiteral("labelSTMin"));
        sizePolicy1.setHeightForWidth(labelSTMin->sizePolicy().hasHeightForWidth());
        labelSTMin->setSizePolicy(sizePolicy1);
        labelSTMin->setMinimumSize(QSize(133, 0));

        gridLayout->addWidget(labelSTMin, 1, 3, 1, 1);

        editNCR = new QLineEdit(groupDiagTime);
        editNCR->setObjectName(QStringLiteral("editNCR"));
        sizePolicy.setHeightForWidth(editNCR->sizePolicy().hasHeightForWidth());
        editNCR->setSizePolicy(sizePolicy);

        gridLayout->addWidget(editNCR, 1, 1, 1, 1);

        labelNAs = new QLabel(groupDiagTime);
        labelNAs->setObjectName(QStringLiteral("labelNAs"));
        sizePolicy1.setHeightForWidth(labelNAs->sizePolicy().hasHeightForWidth());
        labelNAs->setSizePolicy(sizePolicy1);
        labelNAs->setMinimumSize(QSize(133, 0));

        gridLayout->addWidget(labelNAs, 0, 0, 1, 1);

        editSTMIN = new QLineEdit(groupDiagTime);
        editSTMIN->setObjectName(QStringLiteral("editSTMIN"));
        sizePolicy.setHeightForWidth(editSTMIN->sizePolicy().hasHeightForWidth());
        editSTMIN->setSizePolicy(sizePolicy);

        gridLayout->addWidget(editSTMIN, 1, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 2, 1, 1);


        horizontalLayout_6->addLayout(gridLayout);


        verticalLayout->addWidget(groupDiagTime);

        groupProductId = new QGroupBox(SlaveDlg);
        groupProductId->setObjectName(QStringLiteral("groupProductId"));
        horizontalLayout_5 = new QHBoxLayout(groupProductId);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        editVariant = new QLineEdit(groupProductId);
        editVariant->setObjectName(QStringLiteral("editVariant"));
        sizePolicy.setHeightForWidth(editVariant->sizePolicy().hasHeightForWidth());
        editVariant->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(editVariant, 1, 1, 1, 1);

        labelVariant = new QLabel(groupProductId);
        labelVariant->setObjectName(QStringLiteral("labelVariant"));
        sizePolicy.setHeightForWidth(labelVariant->sizePolicy().hasHeightForWidth());
        labelVariant->setSizePolicy(sizePolicy);
        labelVariant->setMinimumSize(QSize(133, 0));

        gridLayout_2->addWidget(labelVariant, 1, 0, 1, 1);

        editSupplierID = new QLineEdit(groupProductId);
        editSupplierID->setObjectName(QStringLiteral("editSupplierID"));
        sizePolicy.setHeightForWidth(editSupplierID->sizePolicy().hasHeightForWidth());
        editSupplierID->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(editSupplierID, 0, 4, 1, 1);

        labelSupplierId = new QLabel(groupProductId);
        labelSupplierId->setObjectName(QStringLiteral("labelSupplierId"));
        sizePolicy.setHeightForWidth(labelSupplierId->sizePolicy().hasHeightForWidth());
        labelSupplierId->setSizePolicy(sizePolicy);
        labelSupplierId->setMinimumSize(QSize(133, 0));

        gridLayout_2->addWidget(labelSupplierId, 0, 3, 1, 1);

        editFunctionId = new QLineEdit(groupProductId);
        editFunctionId->setObjectName(QStringLiteral("editFunctionId"));
        sizePolicy.setHeightForWidth(editFunctionId->sizePolicy().hasHeightForWidth());
        editFunctionId->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(editFunctionId, 0, 1, 1, 1);

        labelFunctionId = new QLabel(groupProductId);
        labelFunctionId->setObjectName(QStringLiteral("labelFunctionId"));
        sizePolicy.setHeightForWidth(labelFunctionId->sizePolicy().hasHeightForWidth());
        labelFunctionId->setSizePolicy(sizePolicy);
        labelFunctionId->setMinimumSize(QSize(133, 0));

        gridLayout_2->addWidget(labelFunctionId, 0, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 1, 2, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_2);


        verticalLayout->addWidget(groupProductId);

        grpRespErr = new QGroupBox(SlaveDlg);
        grpRespErr->setObjectName(QStringLiteral("grpRespErr"));
        gridLayout_3 = new QGridLayout(grpRespErr);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        labelRespErr = new QLabel(grpRespErr);
        labelRespErr->setObjectName(QStringLiteral("labelRespErr"));

        horizontalLayout_31->addWidget(labelRespErr);

        comboRespErr = new QComboBox(grpRespErr);
        comboRespErr->setObjectName(QStringLiteral("comboRespErr"));

        horizontalLayout_31->addWidget(comboRespErr);


        gridLayout_3->addLayout(horizontalLayout_31, 0, 0, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_7, 0, 1, 1, 1);


        verticalLayout->addWidget(grpRespErr);

        groupFaultStateSignals = new QGroupBox(SlaveDlg);
        groupFaultStateSignals->setObjectName(QStringLiteral("groupFaultStateSignals"));
        horizontalLayout_2 = new QHBoxLayout(groupFaultStateSignals);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        editFaultStateSignals = new QLineEdit(groupFaultStateSignals);
        editFaultStateSignals->setObjectName(QStringLiteral("editFaultStateSignals"));
        editFaultStateSignals->setEnabled(false);
        editFaultStateSignals->setClearButtonEnabled(false);

        horizontalLayout->addWidget(editFaultStateSignals);

        buttonFaultSignals = new QPushButton(groupFaultStateSignals);
        buttonFaultSignals->setObjectName(QStringLiteral("buttonFaultSignals"));

        horizontalLayout->addWidget(buttonFaultSignals);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(groupFaultStateSignals);

        buttonOption = new QDialogButtonBox(SlaveDlg);
        buttonOption->setObjectName(QStringLiteral("buttonOption"));
        buttonOption->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonOption);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(SlaveDlg);

        QMetaObject::connectSlotsByName(SlaveDlg);
    } // setupUi

    void retranslateUi(QDialog *SlaveDlg)
    {
        SlaveDlg->setWindowTitle(QApplication::translate("SlaveDlg", "Edit ECU - Slave_ECU1", 0));
        groupProperties->setTitle(QApplication::translate("SlaveDlg", "Properties", 0));
        labelName->setText(QApplication::translate("SlaveDlg", "Name*", 0));
        labelProtocol->setText(QApplication::translate("SlaveDlg", "Protocol", 0));
        comboProtocol->clear();
        comboProtocol->insertItems(0, QStringList()
         << QApplication::translate("SlaveDlg", "1.3", 0)
         << QApplication::translate("SlaveDlg", "2.0", 0)
         << QApplication::translate("SlaveDlg", "2.1", 0)
        );
        groupNodeAddress->setTitle(QApplication::translate("SlaveDlg", "Node Address", 0));
        labelConfigNAD->setText(QApplication::translate("SlaveDlg", "Configured NAD*", 0));
        labelInitialNAD->setText(QApplication::translate("SlaveDlg", "Initial NAD", 0));
        groupDiagTime->setTitle(QApplication::translate("SlaveDlg", "Diagnostic Timings", 0));
        labelNcr->setText(QApplication::translate("SlaveDlg", "NCs", 0));
        labelP2Min->setText(QApplication::translate("SlaveDlg", "P2Min", 0));
        labelSTMin->setText(QApplication::translate("SlaveDlg", "STMin", 0));
        labelNAs->setText(QApplication::translate("SlaveDlg", "NAs", 0));
        groupProductId->setTitle(QApplication::translate("SlaveDlg", "Product Identifiers", 0));
        labelVariant->setText(QApplication::translate("SlaveDlg", "Variant", 0));
        labelSupplierId->setText(QApplication::translate("SlaveDlg", "Supplier ID", 0));
        labelFunctionId->setText(QApplication::translate("SlaveDlg", "Function ID", 0));
        grpRespErr->setTitle(QApplication::translate("SlaveDlg", "Response Error", 0));
        labelRespErr->setText(QApplication::translate("SlaveDlg", "Response Error Signal          ", 0));
        groupFaultStateSignals->setTitle(QApplication::translate("SlaveDlg", "Fault State Signals", 0));
        buttonFaultSignals->setText(QApplication::translate("SlaveDlg", "Edit", 0));
    } // retranslateUi

};

namespace Ui {
    class SlaveDlg: public Ui_SlaveDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLAVEDLG_H
