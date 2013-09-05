#ifndef SRC_FILE_H
#define SRC_FILE_H

#include <QString>
#include <QFileInfo>
#include <code_editor.h>
#include <file_type.h>


class root_file_container;
class highlight_manager;
class syntax_highlighter;
class file_settings;
class visual_src_file;

class src_file : public CodeEditor
{
	Q_OBJECT

	friend class visual_src_file;

private:
	src_file(const src_file&);
	src_file& operator=(const src_file&);

public:
	src_file(const QString &file_name, unsigned int id, root_file_container *parent, file_settings &settings, highlight_manager *hl_manager, file_type &type_manager);
	~src_file();

	bool load_file(const QString &fileName);
	bool write_file(const QString &fileName);

	bool is_modified() const;
	void set_modified(bool modified);

	QString get_content() const;
	QTextDocument *get_mutable_content();

	unsigned int get_id() const;

	void set_src_file_name(const QString &fileName);
	QString get_src_file_name() const;
	QString get_src_file_path() const;
	QString get_src_file_full_name() const;
	bool get_src_file_full_name(QString &file_path) const;
	QString get_src_file_extension() const;

	bool exists() const;
	file_settings& get_settings();

	int get_first_visible_block();
	QTextBlock get_text_block(int block);
	syntax_highlighter *get_highlighter();

	void update_src_file_info();

	void set_cursor(const QTextCursor &cursor);
	QTextCursor get_cursor();
	int get_cursor_position();

	QFileInfo *get_file_info();

	file_type::type get_file_type() const;
	void set_file_type(file_type::type type);

signals:
	void modificationChanged(bool);
	void fileTypeChanged();

private:
	QFileInfo file_info_;
	file_type::type type_;
	root_file_container *parent_;
	file_settings &settings_;
	const file_type &type_manager_;

	highlight_manager *highlight_manager_;
	syntax_highlighter *highlighter_;
	unsigned int id_;
};

#endif
