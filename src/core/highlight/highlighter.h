#ifndef HL_MANAGER_H
#define HL_MANAGER_H

#include <QSyntaxHighlighter>

class highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	highlighter(QTextDocument *parent = 0);
	virtual ~highlighter() = 0;

protected:
	virtual void highlightBlock(const QString &text) = 0;
};

/*
class cpp_highlighter : public highlighter
{
	Q_OBJECT

public:
	cpp_highlighter(QTextDocument *parent = 0);
	~cpp_highlighter(){}

protected:
	void highlightBlock(const QString &text){}
};
*/



#endif
