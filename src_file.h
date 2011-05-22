#ifndef SRC_FILE_H
#define SRC_FILE_H

#include <QtGui/QTabWidget>
#include <QTabBar>
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QFileInfo>
#include <QTextCursor>
#include <QString>

#include "code_editor.h"
#include "highlight/cpp_hl.h"

class src_file : public QWidget {
	
	QWidget *tab;
	CodeEditor *editor;
	QHBoxLayout *horizontalLayout;
	QFileInfo *file_info;
	Highlighter *highlighter;
	QTextCursor *cursor;
	
	public:
	
	src_file(const QString file_name, QString content, bool on_disk);
	bool is_modified();
    bool saved_on_disk();
    bool set_saved_on_disk(bool saved);
    void set_src_file_name(const QString &fileName);
	QString get_content();
	QString get_src_file_name();
	QString get_src_file_full_name();
	bool exists();
	QFont get_font();
	void go_to_line(int line);
    
    bool eventFilter(QObject* pObject, QEvent* pEvent);
};


#endif
