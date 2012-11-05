#ifndef CPP_HIGHLIGHT_H
#define CPP_HIGHLIGHT_H

#include <QSyntaxHighlighter>


class C_highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	 C_highlighter(QTextDocument *parent = 0);
	 ~C_highlighter();

protected:
	void highlightBlock(const QString &text);

private:
		
	QTextCharFormat keywordFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat integerFormat;
};

#endif

