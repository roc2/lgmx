#ifndef SRC_AREA_H
#define SRC_AREA_H


#include <QtGui/QTabWidget>
#include <QTabBar>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QFileInfo>
#include <QTextCursor>

#include "code_editor.h"
#include "highlight/cpp_hl.h"
#include "src_file.h"
#include <iostream>

using namespace std;

#define NEW_FILE_NAME	"untitled"


class src_area : public QTabWidget
{
	public:
	
	src_area();
	int new_src_tab(QString &content, QString file_name);
	void destroy_src_tab(int index);

	bool is_modified(int index);
	bool exists(int index);
    bool saved_on_disk(int index);
    bool set_modified(int index, bool modified);
    bool set_file_name(int index, QString &fileName);
	
	string get_src_tab_name(int index);
	QString get_src_tab_full_name(int index);
	bool update_file_info(int index);
	
	bool get_src_tab_content(int index, QString &content);

	int get_current_tab_index();
	bool get_curr_font(QFont &font);
	
    void show_tabs(bool show);
	void go_to_line(int index, int line);
    
private:
    QTabBar *tab_bar;
};



#endif

