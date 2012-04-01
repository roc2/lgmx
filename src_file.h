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
	
	QWidget tab;
	CodeEditor *editor;
	QHBoxLayout *horizontalLayout;
	QFileInfo *file_info;
	Highlighter *highlighter;
	QTextCursor *cursor;
	
	public:
    
    src_file(const QString &file_name);
    ~src_file();
    
    bool write_file(const QString &fileName);
    bool load_file(const QString &fileName);
    
	bool is_modified();
    bool saved_on_disk();
    bool set_src_file_modified(bool modified);
    void set_src_file_name(const QString &fileName);
	QString get_content();
	QTextDocument *get_mutable_content();
	
	QString get_src_file_name();
    QString get_src_file_path();
	QString get_src_file_full_name();
	bool get_src_file_full_name(QString &file_path);
	bool exists();
	
	void set_font(QFont &font);
	QFont get_font();

	void go_to_line(int line);
    void update_src_file_info();
    
    void set_cursor(const QTextCursor &cursor);
    QTextCursor get_cursor();
    
    int get_cursor_position();
    
    bool eventFilter(QObject* pObject, QEvent* pEvent);
};


#endif
