/********************************************************************************
** Form generated from reading UI file 'AboutLDFEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTLDFEDITOR_H
#define UI_ABOUTLDFEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutLDFEditor
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelIcon;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *Ver10;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *AboutLDFEditor)
    {
        if (AboutLDFEditor->objectName().isEmpty())
            AboutLDFEditor->setObjectName(QStringLiteral("AboutLDFEditor"));
        AboutLDFEditor->resize(396, 155);
        horizontalLayout_2 = new QHBoxLayout(AboutLDFEditor);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        labelIcon = new QLabel(AboutLDFEditor);
        labelIcon->setObjectName(QStringLiteral("labelIcon"));
        labelIcon->setPixmap(QPixmap(QString::fromUtf8(":/LDFEditorIcons/Resources/icons/32x32/Application.png")));
        labelIcon->setScaledContents(true);

        verticalLayout_2->addWidget(labelIcon);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Ver10 = new QLabel(AboutLDFEditor);
        Ver10->setObjectName(QStringLiteral("Ver10"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Ver10->sizePolicy().hasHeightForWidth());
        Ver10->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(10);
        Ver10->setFont(font);

        verticalLayout->addWidget(Ver10);

        label = new QLabel(AboutLDFEditor);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamily(QStringLiteral("Courier New"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        label->setFont(font1);
        label->setTextFormat(Qt::RichText);
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(AboutLDFEditor);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(AboutLDFEditor);

        QMetaObject::connectSlotsByName(AboutLDFEditor);
    } // setupUi

    void retranslateUi(QDialog *AboutLDFEditor)
    {
        AboutLDFEditor->setWindowTitle(QApplication::translate("AboutLDFEditor", "About BUSMASTER LDF Editor", 0));
        labelIcon->setText(QString());
        Ver10->setText(QApplication::translate("AboutLDFEditor", "Ver 1.0", 0));
        label->setText(QApplication::translate("AboutLDFEditor", "<html><head/><body><p><br/>Copyright 2011<br/>ETAS GmbH and Robert Bosch Engineering and Business Solutions Limited</p></body></html>", 0));
        pushButton->setText(QApplication::translate("AboutLDFEditor", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class AboutLDFEditor: public Ui_AboutLDFEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTLDFEDITOR_H
