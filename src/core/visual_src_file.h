#ifndef VISUAL_SRC_FILE_H
#define VISUAL_SRC_FILE_H

#include <code_editor.h>
#include <QBasicTimer>
#include <QString>
#include <file_type.h>

class src_file;
class src_container;
class QFileInfo;
class syntax_highlighter;
class Settings;

class visual_src_file : public CodeEditor
{
	Q_OBJECT

private:
	visual_src_file(const visual_src_file&);
	visual_src_file& operator=(const visual_src_file&);

public:
	visual_src_file(src_file *parent, src_container *container);
	~visual_src_file();

	void set_default_font();

	void set_base_color(const QColor &color);
	void set_text_color(const QColor &color);

	bool write_file(const QString &fileName);

	QString get_file_name() const;
	QString get_src_file_full_name() const;
	bool get_src_file_full_name(QString &file_path) const;
	QString get_src_file_path() const;
	QString get_src_file_extension() const;

	void set_src_file_name(const QString &fileName);

	QTextDocument *get_mutable_content();
	bool load_file(const QString &fileName);
	const src_file* get_root_file();

	unsigned int get_id() const;
	bool exists() const;
	bool is_modified() const;
	void update_src_file_info();

	void set_modified(bool modified);
	void set_line_wrap(bool wrap);
	bool get_line_wrap() const;

	bool set_tab_width(int size);
	int get_tab_width() const;
	bool indent_selection(bool forward);

	QString get_word_under_cursor();
	void show_white_spaces(bool show);
	QTextCursor get_cursor();
	int get_cursor_position();
	void set_cursor(const QTextCursor &cursor);
	void setBlinkingCursorEnabled(bool enable);

	int get_first_visible_block();
	void get_visible_blocks_range(int &first, int &last);
	int get_visible_blocks(QTextBlock &first_block);
	void highlight_visible_blocks();
	
	file_type::type get_file_type() const;
	void set_file_type(file_type::type type);

private:
	void set_content(QTextDocument *content);
	void insert_tab(QTextCursor &cursor);
	void paintEvent(QPaintEvent *e);
	void timerEvent(QTimerEvent *);
	void update_cursor();
	QChar get_matching_brace(QChar c, int *direction);
	void highlight_maching_braces(int start_pos, int end_pos);
	void clear_extra_selections();
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *e);
	void focusInEvent(QFocusEvent*);
	void focusOutEvent(QFocusEvent*);

public slots:
	void go_to_line(int line);
	void strip_trailing_spaces();
	void match_braces();
	bool match_braces(bool select);
	void highlight(int);
	void highlight();

private slots:
	void update_highlighter();

private:
	src_file *parent_file_;
	src_container *container_;
	unsigned int id_;
	QFileInfo *file_info_;
	Settings &settings_;
	syntax_highlighter *highlighter_;
	file_type::type type_;
	QBasicTimer cursor_blink_timer_;
	bool cursor_visible_;
	bool blink_cursor_;
	QTextCharFormat mb_format_;		/**< matching braces format */
};

#endif
