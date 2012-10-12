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
#include <QMenu>
#include <set>
#include <list>

#include <stack>
#include "code_editor.h"
#include "src_container.h"
#include "go_to_line.h"
#include "file_watcher.h"
#include "recent_files.h"
#include "search.h"
#include "view_manager.h"

#include <iostream>

using namespace std;


#define COMPANY		"lgm."
#define APPLICATION	"lgmx"


class QShortcut;
class clipboard;

QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	QAction *actionSave;
    QAction *actionOpen;
    QAction *action_reload;
    QAction *actionNew;
	QAction *actionQuit;

	QAction *actionSide_Bar;
	QAction *actionStatus_Bar;
    QAction *actionMenuBar;
    QAction *actionFullScreen;
    QAction *actionSrcTabBar;
    QAction *action_split_horizontally;
    QAction *action_split_vertically;
    QAction *action_unsplit;

	QAction *actionGo_to_line;
	QAction *action_find;

    QSplitter *splitter;
    QList<int> splitter_size;
    
    QTabWidget *symbol_tab_widget;
    QWidget *tab;
    QWidget *tab_2;
    
    src_container _src_container;
    
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QMenu *menu_File;
    QMenu *menuView;
    QMenu *menu_Search;
    recent_files *menu_recent_files;
    //QMenu *menu_View;
    
	go_to_line *gt_ln_dialog;
	
	clipboard *c_board;

	QString get_text();
    
    void retranslateUi(QMainWindow *main_window);

	bool save_file(bool save_as);
	
	void writeSettings();
	void readSettings();
	
	bool is_active_window();
    
	QString getHomePath();
    
    Ui_MainWindow(list<QString> *files = NULL);
    ~Ui_MainWindow();


protected:
     virtual void closeEvent(QCloseEvent *event);
     virtual void changeEvent(QEvent *e);

private:

	src_container *_src_container_ptr;	// current active set of source files
	src_container *_root_src_container;
	view_manager view_manager_;

    set<QString> open_files; /**< current open files */
    
    void createActions();
    void destroy_actions();
    void create_menus();
    void destroy_menus();

    bool saveFile(const QString &fileName, int index);
    bool checkUnsavedFiles();
    int get_file_index(const QString &file_name);
    void set_current_index(int index);
    void load_parameter_files(list<QString> *files);

	src_container* get_current_src_container();

	file_watcher f_watcher;
	lgmx::search *search_dialog;

private slots:
    // current in use
    bool saveAs(int index);
    ///////////////////////////
	
	//void save_file();
	void quit();
	
	void show_side_bar(bool show);
	void show_status_bar(bool show);
    void show_menu_bar(bool show);
    void show_full_screen(bool fullscreen);
	
	void set_font();
	void set_tab_width();
	//bool save_as();
	
	void go_to_ln();
	
	void print_msg();	// remove

public slots:

	void reload_file(const QString path);

signals:
    void windowActivated();
	
private:
	void create_connections();
	void create_shortcuts();
};

namespace Ui {
    class main_window: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
