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
#include <QScrollArea>
#include <QSharedPointer>

#include "code_editor.h"
#include "highlight/cpp_hl.h"

class src_container;

class src_file : public CodeEditor
{
	Q_OBJECT

public:
    
    src_file(const QString &file_name, unsigned int id);
    src_file(src_file *base_file);
    ~src_file();
    
    bool write_file(const QString &fileName);
    bool load_file(const QString &fileName);
    
	bool is_modified() const;
    void set_modified(bool modified);
    
    bool saved_on_disk();
    
    void set_src_file_name(const QString &fileName);
	QString get_content();
	
	QTextDocument *get_mutable_content();
	void set_content(QTextDocument *content);
	
	QString get_src_file_name();
    QString get_src_file_path();
	
	QString get_src_file_full_name();
	bool get_src_file_full_name(QString &file_path);
	bool exists();
	
	void focusInEvent(QFocusEvent *event);
	
	void set_font(QFont &font);
	void set_default_font();
	QFont get_font();

	void go_to_line(int line);
    void update_src_file_info();
    
    void set_cursor(const QTextCursor &cursor);
    QTextCursor get_cursor();
    
    int get_cursor_position();
    
    void set_file_info(QFileInfo *file_info);
    QFileInfo *get_file_info() const;
    
    //void set_clone(src_file *clone);
    void set_child_src_file(src_file *child, int index);
    src_file *get_child_src_file(int index) {return child_file_[index];}

	unsigned int get_id() const;
    
    bool eventFilter(QObject* pObject, QEvent* pEvent);

signals:
	void modificationChanged(bool);

public slots:
    
private:
	QFileInfo *file_info_;
	Highlighter *highlighter;
	//QTextCursor *cursor;
	//src_file *_clone;
	QScrollArea *scroll_area_;
	bool clone_;
	src_file *child_file_[2];
	unsigned int id_;
	
	hilight_thread *ht;
};


#endif
