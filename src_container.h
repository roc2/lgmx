#ifndef SRC_AREA_H
#define SRC_AREA_H


#include <QtGui/QTabWidget>
#include <QTabBar>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QFileInfo>
#include <QTextCursor>
#include <QFont>
#include <QFileSystemWatcher>

#include "code_editor.h"
#include "highlight/cpp_hl.h"
#include "src_file.h"
#include "status_line.h"
#include <iostream>

using namespace std;

#define NEW_FILE_NAME	"untitled"


class src_container : public QTabWidget
{
	Q_OBJECT
	
	public:
	
	src_container(QWidget *parent = 0);
	src_container(const src_container &copy);
	~src_container();
	
	int new_src_tab(const QString &file_name);
	int new_clone_tab(src_file *base_file);
	void destroy_src_tab(int index);

	bool is_modified(int index);
	bool exists(int index);
    bool saved_on_disk(int index);
    bool set_modified(int index, bool modified);
    bool set_file_name(int index, const QString &fileName);
	
	QWidget* get_parent() const;
	
	//string get_src_tab_name(int index);
    QString get_src_tab_path(int index);
	//QString get_src_tab_full_name(int index);
	bool get_src_tab_full_name(int index, QString &file_name);
    QString get_src_tab_short_name(int index);
	
    bool update_file_info(int index);
	
    bool src_tab_write_file(int index, const QString &fileName);
    bool src_tab_load_file(const QString &fileName);
    
	bool get_src_tab_content(int index, QString &content);

	int get_current_tab_index();
	src_file *get_current_src_file();
	src_file *get_src_file(int index);
	
	bool get_curr_font(int index, QFont &font);
	
    void show_tabs(bool show);
	void go_to_line(int index, int line);
    
    int get_file_index(const QString &file_name);
    //void setFont(QFont &font);
    
    void focusInEvent(QFocusEvent *event);

public slots:
	void file_changed(bool changed);


private:
	QWidget *_parent;
    QTabBar *tab_bar;
    status_line *status_line_;
    QVBoxLayout *main_layout_;
    QFont font;
};



#endif

