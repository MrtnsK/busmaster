/********************************************************************************
** Form generated from reading UI file 'LDFEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LDFEDITOR_H
#define UI_LDFEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "LDFElementView.h"
#include "LDFNetworkView.h"
#include "ldfadditionalview.h"
#include "ldfpropertyview.h"

QT_BEGIN_NAMESPACE

class Ui_LDFEditor
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionExit;
    QAction *actionNewLIN_1_3;
    QAction *actionNewLIN_2_0;
    QAction *actionNewLIN_2_1;
    QAction *actionHex;
    QAction *actionPreview_LDF_File;
    QAction *action_About;
    QAction *actionNew;
    QAction *actionRecent;
    QAction *action1;
    QAction *actionHelp;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter_main;
    LDFElementView *m_pouLDFElementView;
    QSplitter *splitter;
    LDFPropertyView *m_pouLDFPropertyView;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *labelAdditionalViewHelp;
    LDFAdditionalView *m_pouLDFAdditionalView;
    LDFNetworkView *m_pouLDFNetworkView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menu_New;
    QMenu *menu_Recent_Files;
    QMenu *menuHelp;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockPaneWarning;
    QWidget *dockWidgetContents_3;

    void setupUi(QMainWindow *LDFEditor)
    {
        if (LDFEditor->objectName().isEmpty())
            LDFEditor->setObjectName(QStringLiteral("LDFEditor"));
        LDFEditor->resize(851, 716);
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(11);
        LDFEditor->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/LDFEditorIcons/Resources/icons/32x32/Application.png"), QSize(), QIcon::Normal, QIcon::Off);
        LDFEditor->setWindowIcon(icon);
        LDFEditor->setWindowOpacity(1);
        LDFEditor->setIconSize(QSize(32, 32));
        actionOpen = new QAction(LDFEditor);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QFont font1;
        font1.setFamily(QStringLiteral("Courier New"));
        font1.setPointSize(10);
        actionOpen->setFont(font1);
        actionOpen->setMenuRole(QAction::ApplicationSpecificRole);
        actionSave = new QAction(LDFEditor);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave->setFont(font1);
        actionSave_As = new QAction(LDFEditor);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionSave_As->setFont(font1);
        actionExit = new QAction(LDFEditor);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionExit->setFont(font1);
        actionNewLIN_1_3 = new QAction(LDFEditor);
        actionNewLIN_1_3->setObjectName(QStringLiteral("actionNewLIN_1_3"));
        actionNewLIN_1_3->setFont(font1);
        actionNewLIN_2_0 = new QAction(LDFEditor);
        actionNewLIN_2_0->setObjectName(QStringLiteral("actionNewLIN_2_0"));
        actionNewLIN_2_0->setFont(font1);
        actionNewLIN_2_1 = new QAction(LDFEditor);
        actionNewLIN_2_1->setObjectName(QStringLiteral("actionNewLIN_2_1"));
        actionNewLIN_2_1->setFont(font1);
        actionHex = new QAction(LDFEditor);
        actionHex->setObjectName(QStringLiteral("actionHex"));
        actionPreview_LDF_File = new QAction(LDFEditor);
        actionPreview_LDF_File->setObjectName(QStringLiteral("actionPreview_LDF_File"));
        action_About = new QAction(LDFEditor);
        action_About->setObjectName(QStringLiteral("action_About"));
        actionNew = new QAction(LDFEditor);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionNew->setCheckable(true);
        actionRecent = new QAction(LDFEditor);
        actionRecent->setObjectName(QStringLiteral("actionRecent"));
        action1 = new QAction(LDFEditor);
        action1->setObjectName(QStringLiteral("action1"));
        actionHelp = new QAction(LDFEditor);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        centralWidget = new QWidget(LDFEditor);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        splitter_main = new QSplitter(centralWidget);
        splitter_main->setObjectName(QStringLiteral("splitter_main"));
        splitter_main->setOrientation(Qt::Horizontal);
        m_pouLDFElementView = new LDFElementView(splitter_main);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setBackground(0, QColor(0, 0, 0));
        m_pouLDFElementView->setHeaderItem(__qtreewidgetitem);
        m_pouLDFElementView->setObjectName(QStringLiteral("m_pouLDFElementView"));
        m_pouLDFElementView->setFont(font1);
        splitter_main->addWidget(m_pouLDFElementView);
        splitter = new QSplitter(splitter_main);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        m_pouLDFPropertyView = new LDFPropertyView(splitter);
        if (m_pouLDFPropertyView->columnCount() < 2)
            m_pouLDFPropertyView->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qtablewidgetitem->setFont(font1);
        m_pouLDFPropertyView->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        __qtablewidgetitem1->setFont(font1);
        m_pouLDFPropertyView->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        m_pouLDFPropertyView->setObjectName(QStringLiteral("m_pouLDFPropertyView"));
        m_pouLDFPropertyView->setEditTriggers(QAbstractItemView::EditKeyPressed);
        m_pouLDFPropertyView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_pouLDFPropertyView->setSelectionBehavior(QAbstractItemView::SelectRows);
        splitter->addWidget(m_pouLDFPropertyView);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        labelAdditionalViewHelp = new QLabel(layoutWidget);
        labelAdditionalViewHelp->setObjectName(QStringLiteral("labelAdditionalViewHelp"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelAdditionalViewHelp->sizePolicy().hasHeightForWidth());
        labelAdditionalViewHelp->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(labelAdditionalViewHelp);

        m_pouLDFAdditionalView = new LDFAdditionalView(layoutWidget);
        m_pouLDFAdditionalView->setObjectName(QStringLiteral("m_pouLDFAdditionalView"));
        m_pouLDFAdditionalView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pouLDFAdditionalView->setSelectionMode(QAbstractItemView::SingleSelection);
        m_pouLDFAdditionalView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(m_pouLDFAdditionalView);

        splitter->addWidget(layoutWidget);
        splitter_main->addWidget(splitter);
        m_pouLDFNetworkView = new LDFNetworkView(splitter_main);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("LIN Network View"));
        m_pouLDFNetworkView->setHeaderItem(__qtreewidgetitem1);
        m_pouLDFNetworkView->setObjectName(QStringLiteral("m_pouLDFNetworkView"));
        m_pouLDFNetworkView->setFont(font1);
        splitter_main->addWidget(m_pouLDFNetworkView);

        verticalLayout_2->addWidget(splitter_main);

        LDFEditor->setCentralWidget(centralWidget);
        menubar = new QMenuBar(LDFEditor);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 851, 23));
        QFont font2;
        font2.setPointSize(10);
        menubar->setFont(font2);
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFile->setFont(font1);
        menuFile->setContextMenuPolicy(Qt::DefaultContextMenu);
        menu_New = new QMenu(menuFile);
        menu_New->setObjectName(QStringLiteral("menu_New"));
        menu_Recent_Files = new QMenu(menuFile);
        menu_Recent_Files->setObjectName(QStringLiteral("menu_Recent_Files"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuHelp->setFont(font2);
        menuView = new QMenu(menubar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuView->setFont(font1);
        LDFEditor->setMenuBar(menubar);
        mainToolBar = new QToolBar(LDFEditor);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setFont(font2);
        LDFEditor->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(LDFEditor);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        LDFEditor->setStatusBar(statusBar);
        dockPaneWarning = new QDockWidget(LDFEditor);
        dockPaneWarning->setObjectName(QStringLiteral("dockPaneWarning"));
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        dockPaneWarning->setWidget(dockWidgetContents_3);
        LDFEditor->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockPaneWarning);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(menu_New->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(menu_Recent_Files->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menu_New->addAction(actionNewLIN_1_3);
        menu_New->addAction(actionNewLIN_2_0);
        menu_New->addAction(actionNewLIN_2_1);
        menuHelp->addAction(actionHelp);
        menuHelp->addAction(action_About);
        menuView->addSeparator();
        menuView->addAction(actionHex);
        menuView->addSeparator();
        menuView->addAction(actionPreview_LDF_File);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionSave_As);
        mainToolBar->addSeparator();

        retranslateUi(LDFEditor);

        QMetaObject::connectSlotsByName(LDFEditor);
    } // setupUi

    void retranslateUi(QMainWindow *LDFEditor)
    {
        LDFEditor->setWindowTitle(QApplication::translate("LDFEditor", "BUSMASTER LDF Editor", 0));
        actionOpen->setText(QApplication::translate("LDFEditor", "&Open...", 0));
        actionSave->setText(QApplication::translate("LDFEditor", "&Save", 0));
        actionSave_As->setText(QApplication::translate("LDFEditor", "Save &As...", 0));
        actionExit->setText(QApplication::translate("LDFEditor", "E&xit", 0));
        actionNewLIN_1_3->setText(QApplication::translate("LDFEditor", "LIN 1.3", 0));
        actionNewLIN_2_0->setText(QApplication::translate("LDFEditor", "LIN 2.0", 0));
        actionNewLIN_2_1->setText(QApplication::translate("LDFEditor", "LIN 2.1", 0));
        actionHex->setText(QApplication::translate("LDFEditor", "&Hex", 0));
        actionPreview_LDF_File->setText(QApplication::translate("LDFEditor", "&Preview LDF File", 0));
        action_About->setText(QApplication::translate("LDFEditor", "&About", 0));
        actionNew->setText(QApplication::translate("LDFEditor", "New", 0));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("LDFEditor", "Create New LDF File", 0));
#endif // QT_NO_TOOLTIP
        actionNew->setShortcut(QApplication::translate("LDFEditor", "Ctrl+N", 0));
        actionRecent->setText(QApplication::translate("LDFEditor", "Recent", 0));
        action1->setText(QApplication::translate("LDFEditor", "1", 0));
        actionHelp->setText(QApplication::translate("LDFEditor", "Help", 0));
        QTreeWidgetItem *___qtreewidgetitem = m_pouLDFElementView->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("LDFEditor", "LDF Element View", 0));
        QTableWidgetItem *___qtablewidgetitem = m_pouLDFPropertyView->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("LDFEditor", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = m_pouLDFPropertyView->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("LDFEditor", "Value", 0));
        labelAdditionalViewHelp->setText(QString());
        menuFile->setTitle(QApplication::translate("LDFEditor", "&File", 0));
        menu_New->setTitle(QApplication::translate("LDFEditor", "&New", 0));
        menu_Recent_Files->setTitle(QApplication::translate("LDFEditor", "&Recent Files", 0));
        menuHelp->setTitle(QApplication::translate("LDFEditor", "&Help", 0));
        menuView->setTitle(QApplication::translate("LDFEditor", "&View", 0));
        dockPaneWarning->setWindowTitle(QApplication::translate("LDFEditor", "Warnings Window", 0));
    } // retranslateUi

};

namespace Ui {
    class LDFEditor: public Ui_LDFEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LDFEDITOR_H
