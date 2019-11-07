/********************************************************************************
** Form generated from reading UI file 'SignalDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNALDLG_H
#define UI_SIGNALDLG_H

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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SignalDlg
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout1;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *editInitialValue;
    QLabel *label_6;
    QLineEdit *editName;
    QComboBox *comboCoding;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QLineEdit *editLength;
    QLabel *label;
    QComboBox *comboSignalType;
    QLabel *label_2;
    QLabel *labelLength;
    QLabel *labelInitailValueHex;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout_2;
    QComboBox *comboPublisher;
    QLabel *label_5;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer;
    QListWidget *listSubscribers;
    QLabel *labelPublisher;
    QDialogButtonBox *buttonOption;

    void setupUi(QDialog *SignalDlg)
    {
        if (SignalDlg->objectName().isEmpty())
            SignalDlg->setObjectName(QStringLiteral("SignalDlg"));
        SignalDlg->resize(723, 389);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SignalDlg->sizePolicy().hasHeightForWidth());
        SignalDlg->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/Signals.png"), QSize(), QIcon::Normal, QIcon::Off);
        SignalDlg->setWindowIcon(icon);
        horizontalLayout_4 = new QHBoxLayout(SignalDlg);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout1 = new QVBoxLayout();
        verticalLayout1->setObjectName(QStringLiteral("verticalLayout1"));
        verticalLayout1->setSizeConstraint(QLayout::SetFixedSize);
        groupBox = new QGroupBox(SignalDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 0, 6, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(label_8, 1, 3, 1, 1);

        editInitialValue = new QLineEdit(groupBox);
        editInitialValue->setObjectName(QStringLiteral("editInitialValue"));
        editInitialValue->setMinimumSize(QSize(25, 0));

        gridLayout->addWidget(editInitialValue, 1, 5, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        editName = new QLineEdit(groupBox);
        editName->setObjectName(QStringLiteral("editName"));
        sizePolicy.setHeightForWidth(editName->sizePolicy().hasHeightForWidth());
        editName->setSizePolicy(sizePolicy);
        editName->setMinimumSize(QSize(91, 0));

        gridLayout->addWidget(editName, 0, 1, 1, 1);

        comboCoding = new QComboBox(groupBox);
        comboCoding->setObjectName(QStringLiteral("comboCoding"));
        sizePolicy.setHeightForWidth(comboCoding->sizePolicy().hasHeightForWidth());
        comboCoding->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboCoding, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        editLength = new QLineEdit(groupBox);
        editLength->setObjectName(QStringLiteral("editLength"));
        editLength->setMinimumSize(QSize(25, 0));

        gridLayout->addWidget(editLength, 0, 5, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboSignalType = new QComboBox(groupBox);
        comboSignalType->setObjectName(QStringLiteral("comboSignalType"));
        sizePolicy.setHeightForWidth(comboSignalType->sizePolicy().hasHeightForWidth());
        comboSignalType->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboSignalType, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(90, 0));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        labelLength = new QLabel(groupBox);
        labelLength->setObjectName(QStringLiteral("labelLength"));

        gridLayout->addWidget(labelLength, 0, 7, 1, 1);

        labelInitailValueHex = new QLabel(groupBox);
        labelInitailValueHex->setObjectName(QStringLiteral("labelInitailValueHex"));

        gridLayout->addWidget(labelInitailValueHex, 1, 4, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout1->addWidget(groupBox);

        groupBox_3 = new QGroupBox(SignalDlg);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        comboPublisher = new QComboBox(groupBox_3);
        comboPublisher->setObjectName(QStringLiteral("comboPublisher"));
        sizePolicy.setHeightForWidth(comboPublisher->sizePolicy().hasHeightForWidth());
        comboPublisher->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(comboPublisher, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setMinimumSize(QSize(90, 0));

        gridLayout_2->addWidget(label_5, 0, 4, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setMinimumSize(QSize(90, 0));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 3, 1, 1);

        listSubscribers = new QListWidget(groupBox_3);
        listSubscribers->setObjectName(QStringLiteral("listSubscribers"));
        sizePolicy2.setHeightForWidth(listSubscribers->sizePolicy().hasHeightForWidth());
        listSubscribers->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(listSubscribers, 0, 5, 1, 1);

        labelPublisher = new QLabel(groupBox_3);
        labelPublisher->setObjectName(QStringLiteral("labelPublisher"));

        gridLayout_2->addWidget(labelPublisher, 0, 2, 1, 1);


        horizontalLayout_3->addLayout(gridLayout_2);


        verticalLayout1->addWidget(groupBox_3);

        buttonOption = new QDialogButtonBox(SignalDlg);
        buttonOption->setObjectName(QStringLiteral("buttonOption"));
        buttonOption->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout1->addWidget(buttonOption);


        horizontalLayout_4->addLayout(verticalLayout1);

        QWidget::setTabOrder(editName, editLength);
        QWidget::setTabOrder(editLength, comboSignalType);
        QWidget::setTabOrder(comboSignalType, editInitialValue);
        QWidget::setTabOrder(editInitialValue, comboCoding);
        QWidget::setTabOrder(comboCoding, comboPublisher);
        QWidget::setTabOrder(comboPublisher, listSubscribers);
        QWidget::setTabOrder(listSubscribers, buttonOption);

        retranslateUi(SignalDlg);

        QMetaObject::connectSlotsByName(SignalDlg);
    } // setupUi

    void retranslateUi(QDialog *SignalDlg)
    {
        SignalDlg->setWindowTitle(QApplication::translate("SignalDlg", "Edit Signal - Signal1", 0));
        groupBox->setTitle(QApplication::translate("SignalDlg", "General Properties", 0));
        label_7->setText(QApplication::translate("SignalDlg", "bits", 0));
        label_8->setText(QApplication::translate("SignalDlg", "Initial Value", 0));
        label_6->setText(QApplication::translate("SignalDlg", "Select Coding", 0));
        label_3->setText(QApplication::translate("SignalDlg", "Length*", 0));
        label->setText(QApplication::translate("SignalDlg", "Name*", 0));
        comboSignalType->clear();
        comboSignalType->insertItems(0, QStringList()
         << QApplication::translate("SignalDlg", "Scalar", 0)
         << QApplication::translate("SignalDlg", "Byte Array", 0)
        );
        label_2->setText(QApplication::translate("SignalDlg", "Type", 0));
        labelLength->setText(QApplication::translate("SignalDlg", "TextLabel", 0));
        labelInitailValueHex->setText(QApplication::translate("SignalDlg", "0x", 0));
        groupBox_3->setTitle(QApplication::translate("SignalDlg", "Tx\\Rx Relations", 0));
        label_5->setText(QApplication::translate("SignalDlg", "Select\n"
"Subcribers", 0));
        label_4->setText(QApplication::translate("SignalDlg", "Publisher*", 0));
        labelPublisher->setText(QApplication::translate("SignalDlg", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class SignalDlg: public Ui_SignalDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNALDLG_H
