#ifndef SRC_FILE_H
#define SRC_FILE_H

#include <QString>
#include <code_editor.h>
#include <file_type.h>


class QFileInfo;
class src_container;
class root_file_container;
class highlight_manager;
class syntax_highlighter;
class Settings;
class visual_src_file;

class src_file : public CodeEditor
{
	Q_OBJECT

	friend class visual_src_file;

private:
	src_file();
	src_file(const src_file&);
	src_file& operator=(const src_file&);

public:
	//src_file(const QString &file_name, unsigned int id, src_container *parent, Settings &settings, highlight_manager *hl_manager = NULL);
	src_file(const QString &file_name, unsigned int id, root_file_container *parent, Settings &settings, highlight_manager *hl_manager);
	//src_file(src_file *base_file, src_container *parent);
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
	Settings& get_settings();

	int get_first_visible_block();
	QTextBlock get_text_block(int block);
	syntax_highlighter *get_highlighter();

	void update_src_file_info();

	void set_cursor(const QTextCursor &cursor);
	QTextCursor get_cursor();
	int get_cursor_position();

	void set_file_info(QFileInfo *file_info);
	QFileInfo *get_file_info() const;

	file_type::type get_file_type() const;
	void set_file_type(file_type::type type);

private:
	//void paintEvent(QPaintEvent *e);

signals:
	void modificationChanged(bool);

private:
	QFileInfo *file_info_;
	file_type::type type_;
	src_container *parent_;
	root_file_container *rparent_;
	Settings &settings_;

	highlight_manager *highlight_manager_;
	syntax_highlighter *highlighter_;
	unsigned int id_;

	bool cursor_visible_;
	bool blink_cursor_;
	QTextCharFormat mb_format_;		/**< matching braces format */
};


#endif
