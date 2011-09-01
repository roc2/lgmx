/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created: Mon Nov 15 23:47:14 2010
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
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QFontDialog>
#include <QList>
#include <QFontInfo>
#include <QSettings>
#include <set>
#include <list>

#include <stack>
#include "code_editor.h"
#include "src_area.h"
#include "go_to_line.h"

#include <iostream>

using namespace std;


#define COMPANY		"lgm."
#define APPLICATION	"lgmx"

#define SAVE_FILE_OK        0
#define SAVE_FILE_DISCARD   -1
#define SAVE_FILE_ERR       -2

QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	QAction *actionSave;
    QAction *actionOpen;
    QAction *actionNew;

	QAction *actionSide_Bar;
	QAction *actionStatus_Bar;
    QAction *actionMenuBar;
    QAction *actionFullScreen;
    QAction *actionSrcTabBar;

	QAction *actionGo_to_line;

    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QList<int> splitter_size;
    
    QTabWidget *symbol_tab_widget;
    QWidget *tab;
    QWidget *tab_2;
    
    src_area src_files;
    
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenu *menu_File;
    QMenu *menuView;
    QMenu *menu_Search;
    //QMenu *menu_View;
    
	go_to_line *gt_ln_dialog;

	QString get_text();
    
    void retranslateUi(QMainWindow *main_window)
    {
        main_window->setWindowTitle(QApplication::translate("main_window", "main_window", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        
        actionSide_Bar->setText(QApplication::translate("main_window", "Side Bar", 0, QApplication::UnicodeUTF8));
        actionSide_Bar->setShortcut(QApplication::translate("main_window", "Alt+X", 0, QApplication::UnicodeUTF8));
        
        actionStatus_Bar->setText(QApplication::translate("main_window", "Status Bar", 0, QApplication::UnicodeUTF8));
        actionStatus_Bar->setShortcut(QApplication::translate("main_window", "Alt+Z", 0, QApplication::UnicodeUTF8));
        
        actionMenuBar->setText(QApplication::translate("main_window", "Menu Bar", 0, QApplication::UnicodeUTF8));
        actionMenuBar->setShortcut(QApplication::translate("main_window", "Alt+C", 0, QApplication::UnicodeUTF8));
        actionMenuBar->setShortcutContext(Qt::ApplicationShortcut);
        
        actionFullScreen->setText(QApplication::translate("main_window", "Full Screen", 0, QApplication::UnicodeUTF8));
        actionFullScreen->setShortcut(QApplication::translate("main_window", "F11", 0, QApplication::UnicodeUTF8));
        actionFullScreen->setShortcutContext(Qt::ApplicationShortcut);
        
        actionSrcTabBar->setText(QApplication::translate("main_window", "Source Tab Bar", 0, QApplication::UnicodeUTF8));
        actionSrcTabBar->setShortcut(QApplication::translate("main_window", "Alt+3", 0, QApplication::UnicodeUTF8));
        actionSrcTabBar->setShortcutContext(Qt::ApplicationShortcut);
        
        actionGo_to_line->setText(QApplication::translate("main_window", "Go to line", 0, QApplication::UnicodeUTF8));
        actionGo_to_line->setShortcut(QApplication::translate("main_window", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        
        symbol_tab_widget->setTabText(symbol_tab_widget->indexOf(tab), QApplication::translate("main_window", "Tab 1", 0, QApplication::UnicodeUTF8));
        symbol_tab_widget->setTabText(symbol_tab_widget->indexOf(tab_2), QApplication::translate("main_window", "Tab 2", 0, QApplication::UnicodeUTF8));

        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("main_window", "&View", 0, QApplication::UnicodeUTF8));
        menu_Search->setTitle(QApplication::translate("main_window", "&Search", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

	bool save_file(bool save_as);
	
	void writeSettings();
	void readSettings();
    
	QString getHomePath();
    
    Ui_MainWindow();    // constructor
    Ui_MainWindow(list<QString> *files);
	//

protected:
     void closeEvent(QCloseEvent *event);

private:
    set<QString> open_files; // current open files
    void createActions();
    bool saveFile(const QString &fileName, int index);
    bool checkUnsavedFiles();
    void build_close_file_msg(int index, QString &msg);
    int get_file_index(const QString &file_name);
    void set_current_index(int index);
    void load_parameter_files(list<QString> *files);

private slots:
    // current in use
	bool save();
    bool saveAs(int index);
    ///////////////////////////
    
    
	
	//void save_file();
	void open_file();
	void new_file();
	void close_file(int index);
	
	void show_side_bar(bool show);
	void show_status_bar(bool show);
    void show_menu_bar(bool show);
    void show_full_screen(bool fullscreen);
    void show_src_tab_bar(bool show);
	
	void set_font();
	void set_tab_width();
	//bool save_as();
	
	void go_to_ln();
};

namespace Ui {
    class main_window: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
