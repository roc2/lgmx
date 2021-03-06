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
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QWidget>
#include <QFontDialog>
#include <QFontInfo>
#include <QSettings>
#include <QMenu>
#include <list>

#include <code_editor.h>
#include <src_container.h>
#include <go_to_line.h>
#include <recent_files.h>
#include <search.h>
#include <view_manager.h>
#include <cli.h>


#define COMPANY		"lgm."
#define APPLICATION	"lgmx"


class QShortcut;
class clipboard;
class text_manip;
class CLI_args;

QT_BEGIN_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	MainWindow(const MainWindow&);
	MainWindow& operator=(const MainWindow&);

public:
	MainWindow();
    ~MainWindow();

	void init(CLI_args &args);

private:
	void createActions();
    void destroy_actions();
    void create_menus();
    void destroy_menus();
	void create_connections();
	void create_shortcuts();
	void destroy_shortcuts();
	void load_plugins();

    void retranslateUi(QMainWindow *main_window);

	void writeSettings();
	void readSettings();
	bool is_active_window();
	QString getHomePath();
	void load_parameter_files(std::list<QString> *files);

	virtual void closeEvent(QCloseEvent *event);
	virtual void changeEvent(QEvent *e);

public slots:

private slots:
	void quit();
    void show_menu_bar(bool show);
    void show_full_screen(bool);
	void go_to_ln();
	void show_cli();

signals:
    void windowActivated();

private:
	file_type type_manager;
	//Settings *settings_;
	view_manager *view_manager_;

	QAction *actionSave;
    QAction *actionOpen;
    QAction *action_close_;
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
    QAction *action_remove_all_splits;
	QAction *actionGo_to_line;
	QAction *action_find;

    lgmx::cli *cli_;
    QVBoxLayout *main_layout_;
    QWidget *widget_;

    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QMenu *menu_File;
    QMenu *menuView;
    QMenu *menu_Search;
    recent_files *menu_recent_files;
    //QMenu *menu_View;

	go_to_line *gt_ln_dialog;
	lgmx::search *search_dialog;

	QShortcut *next_file_;
	QShortcut *go_to_tag_;
	QShortcut *show_cli_;

	clipboard *c_board;
	text_manip *text_manip_;
};

QT_END_NAMESPACE

#endif
