#ifndef SYNTAX_HIGHLIGHT_H
#define SYNTAX_HIGHLIGHT_H

#include <QTextBlock>
#include <QObject>

class src_file;
class QTextCursor;
class QTextCharFormat;

struct hl_info {
	int begin;
	int offset;
	int token;
};

class syntax_highlighter : public QObject
{
	Q_OBJECT

public:
	syntax_highlighter(src_file *file);
	virtual ~syntax_highlighter();

	virtual void highlight_block(const QString &text) = 0;
	void highlight_blocks(QTextBlock &first, int count);
	void set_format(int start, int count, const QTextCharFormat &format);
	void applyFormatChanges();
	
	int get_current_block_state() const;
	void set_current_block_state(int state);
	int get_previous_block_state() const;

protected:
	QTextCursor *cursor_;
	QTextBlock curr_block_;
	QTextBlock prev_block_;
	src_file *file_;
};


#endif
