#ifndef CPP_HIGHLIGHT_H
#define CPP_HIGHLIGHT_H

#include <QSyntaxHighlighter>
#include <syntax_highlighter.h>
#include <set>

struct hl_info {
	int begin;
	int offset;
	int token;
};

class C_highlighter : public syntax_highlighter
{
	Q_OBJECT

public:
	 C_highlighter(src_file *parent);
	 ~C_highlighter();

protected:
	virtual void highlight_block(const QString &text);
	void lex(const QString &data, QList<hl_info> &hl_info_list);

private:
	//std::set<QString> keywords_;
	QTextCharFormat keywordFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat integerFormat;
};

#endif

