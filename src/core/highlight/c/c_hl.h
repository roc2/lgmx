#ifndef C_HIGHLIGHT_H
#define C_HIGHLIGHT_H

#include <QTextCharFormat>
#include <QSet>

#include <syntax_highlighter.h>


class C_highlighter : public syntax_highlighter
{
	Q_OBJECT

public:
	static const int SIZE = 5;
	// the following constats must be sequential (no gaps)
	static const int COMMENT_IDX = 0;
	static const int KEYWORD_IDX = 1;
	static const int NUMBER_IDX = 2;
	static const int LITERAL_IDX = 3;
	static const int PRE_PROC_IDX = 4;

public:
	 C_highlighter(src_file *parent, QSharedPointer<QSet<QString> > keywords, 
		           QSharedPointer<std::vector<QTextCharFormat> > formats);
	 ~C_highlighter();

protected:
	virtual void highlight_block(const QString &text);
	void lex(const QString &data, QList<hl_info> &hl_info_list);

private:
	QSharedPointer<QSet<QString> > keywords_;
	QSharedPointer<std::vector<QTextCharFormat> > formats_;
};


#endif

