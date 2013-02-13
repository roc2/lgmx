#ifndef CPP_HIGHLIGHT_H
#define CPP_HIGHLIGHT_H

#include <QSyntaxHighlighter>
#include <set>

struct hl_info {
	int begin;
	int offset;
	int token;
};


class C_highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	 C_highlighter(QTextDocument *parent = 0);
	 ~C_highlighter();

protected:
	void highlightBlock(const QString &text);
	void lex(const QString &data, QList<hl_info> &hl_info_list);

private:
	//std::set<QString> keywords_;
	QTextCharFormat keywordFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat integerFormat;
};

#endif

