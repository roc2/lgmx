#ifndef SRC_FILE_H
#define SRC_FILE_H

#include <QString>
#include <code_editor.h>
#include <file_type.h>
//#include <srchiliteqt/Qt4SyntaxHighlighter.h>


class QFileInfo;
class src_container;
class highlight_manager;
class QSyntaxHighlighter;

class src_file : public CodeEditor
{
	Q_OBJECT

public:
    src_file(const QString &file_name, unsigned int id, src_container *parent, highlight_manager *hl_manager = NULL);
    src_file(src_file *base_file, src_container *parent);
    ~src_file();
    
    bool load_file(const QString &fileName);
    bool write_file(const QString &fileName);
    
	bool is_modified() const;
    void set_modified(bool modified);

	QString get_content() const;
	QTextDocument *get_mutable_content();
	void set_content(QTextDocument *content);

	unsigned int get_id() const;

	void set_src_file_name(const QString &fileName);
	QString get_src_file_name() const;
    QString get_src_file_path() const;
	QString get_src_file_full_name() const;
	bool get_src_file_full_name(QString &file_path) const;
	QString get_src_file_extension() const;

	bool exists() const;
	
	void focusInEvent(QFocusEvent *event);
	void focusOutEvent(QFocusEvent *event);

	QFont get_font() const;
	void set_font(QFont &font);
	void set_default_font();

    void update_src_file_info();
    
    void set_cursor(const QTextCursor &cursor);
    QTextCursor get_cursor();
    int get_cursor_position();
    
    void set_file_info(QFileInfo *file_info);
    QFileInfo *get_file_info() const;
    
    void set_base_color(const QColor &color);
    void set_text_color(const QColor &color);

	bool set_tab_width(int size);
	int get_tab_width() const;
 
    file_type::type get_file_type() const;
    void set_file_type(file_type::type type);
    
    bool eventFilter(QObject* pObject, QEvent* pEvent);

signals:
	void modificationChanged(bool);

public slots:
    void go_to_line(int line);
    
private:
	QFileInfo *file_info_;
	file_type::type type_;
	src_container *parent_;
	//srchiliteqt::Qt4SyntaxHighlighter *highlighter_;
	
	highlight_manager *highlight_manager_;
	QSyntaxHighlighter *highlighter_;
	bool clone_;
	unsigned int id_;
};


#endif
