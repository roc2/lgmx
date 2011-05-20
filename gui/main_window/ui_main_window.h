/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created: Tue Jan 18 00:01:51 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main_window
{
public:
    QAction *actionSide_Bar;
    QAction *actionStatus_Bar;
    QAction *actionGo_to_line;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTabWidget *symbol_tab_widget;
    QWidget *tab;
    QWidget *tab_2;
    QTabWidget *src_tab_widget;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_2;
    QPlainTextEdit *plainTextEdit;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_3;
    QPlainTextEdit *plainTextEdit_2;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menu_Search;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *main_window)
    {
        if (main_window->objectName().isEmpty())
            main_window->setObjectName(QString::fromUtf8("main_window"));
        main_window->resize(831, 557);
        actionSide_Bar = new QAction(main_window);
        actionSide_Bar->setObjectName(QString::fromUtf8("actionSide_Bar"));
        actionSide_Bar->setCheckable(true);
        actionStatus_Bar = new QAction(main_window);
        actionStatus_Bar->setObjectName(QString::fromUtf8("actionStatus_Bar"));
        actionStatus_Bar->setCheckable(true);
        actionGo_to_line = new QAction(main_window);
        actionGo_to_line->setObjectName(QString::fromUtf8("actionGo_to_line"));
        centralWidget = new QWidget(main_window);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        symbol_tab_widget = new QTabWidget(splitter);
        symbol_tab_widget->setObjectName(QString::fromUtf8("symbol_tab_widget"));
        symbol_tab_widget->setTabsClosable(true);
        symbol_tab_widget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        symbol_tab_widget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        symbol_tab_widget->addTab(tab_2, QString());
        splitter->addWidget(symbol_tab_widget);
        src_tab_widget = new QTabWidget(splitter);
        src_tab_widget->setObjectName(QString::fromUtf8("src_tab_widget"));
        src_tab_widget->setTabsClosable(true);
        src_tab_widget->setMovable(true);
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_2 = new QHBoxLayout(tab_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        plainTextEdit = new QPlainTextEdit(tab_3);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        horizontalLayout_2->addWidget(plainTextEdit);

        src_tab_widget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_3 = new QHBoxLayout(tab_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        plainTextEdit_2 = new QPlainTextEdit(tab_4);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));

        horizontalLayout_3->addWidget(plainTextEdit_2);

        src_tab_widget->addTab(tab_4, QString());
        splitter->addWidget(src_tab_widget);

        horizontalLayout->addWidget(splitter);

        main_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(main_window);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 831, 25));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menu_Search = new QMenu(menuBar);
        menu_Search->setObjectName(QString::fromUtf8("menu_Search"));
        main_window->setMenuBar(menuBar);
        mainToolBar = new QToolBar(main_window);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        main_window->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(main_window);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        main_window->setStatusBar(statusBar);

        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menu_Search->menuAction());
        menuView->addSeparator();
        menuView->addAction(actionSide_Bar);
        menuView->addAction(actionStatus_Bar);
        menu_Search->addSeparator();
        menu_Search->addAction(actionGo_to_line);

        retranslateUi(main_window);

        src_tab_widget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(main_window);
    } // setupUi

    void retranslateUi(QMainWindow *main_window)
    {
        main_window->setWindowTitle(QApplication::translate("main_window", "main_window", 0, QApplication::UnicodeUTF8));
        actionSide_Bar->setText(QApplication::translate("main_window", "Side Bar", 0, QApplication::UnicodeUTF8));
        actionSide_Bar->setShortcut(QApplication::translate("main_window", "Alt+X", 0, QApplication::UnicodeUTF8));
        actionStatus_Bar->setText(QApplication::translate("main_window", "Status Bar", 0, QApplication::UnicodeUTF8));
        actionStatus_Bar->setShortcut(QApplication::translate("main_window", "Alt+Z", 0, QApplication::UnicodeUTF8));
        actionGo_to_line->setText(QApplication::translate("main_window", "Go to line", 0, QApplication::UnicodeUTF8));
        actionGo_to_line->setShortcut(QApplication::translate("main_window", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        symbol_tab_widget->setTabText(symbol_tab_widget->indexOf(tab), QApplication::translate("main_window", "Tab 1", 0, QApplication::UnicodeUTF8));
        symbol_tab_widget->setTabText(symbol_tab_widget->indexOf(tab_2), QApplication::translate("main_window", "Tab 2", 0, QApplication::UnicodeUTF8));
        src_tab_widget->setTabText(src_tab_widget->indexOf(tab_3), QApplication::translate("main_window", "Tab 1", 0, QApplication::UnicodeUTF8));
        src_tab_widget->setTabText(src_tab_widget->indexOf(tab_4), QApplication::translate("main_window", "Tab 2", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("main_window", "&View", 0, QApplication::UnicodeUTF8));
        menu_Search->setTitle(QApplication::translate("main_window", "&Search", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class main_window: public Ui_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
