#ifndef CPP_HIGHLIGHT_H
#define CPP_HIGHLIGHT_H

#include <QTextCharFormat>
#include <QSet>

#include <syntax_highlighter.h>

struct hl_info {
	int begin;
	int offset;
	int token;
};

class C_highlighter : public syntax_highlighter
{
	Q_OBJECT

public:
	 C_highlighter(src_file *parent, QSharedPointer<QSet<QString> > keywords);
	 ~C_highlighter();

protected:
	virtual void highlight_block(const QString &text);
	void lex(const QString &data, QList<hl_info> &hl_info_list);

private:
	QTextCharFormat *keywordFormat;
	QTextCharFormat *CommentFormat;
	QTextCharFormat *integerFormat;
	QTextCharFormat *literalFormat;
	QTextCharFormat *pre_processor;

	QSharedPointer<QSet<QString> > keywords_;
};

class dummy_highlighter : public syntax_highlighter
{
	Q_OBJECT

public:
	 dummy_highlighter(src_file *parent);
	 ~dummy_highlighter();
	 
	 void highlight_block(const QString &text);
	 
private:
	QTextCharFormat *keywordFormat;
	//QTextCharFormat CommentFormat;
	//QTextCharFormat integerFormat;
	//QTextCharFormat literalFormat;
};

#endif

