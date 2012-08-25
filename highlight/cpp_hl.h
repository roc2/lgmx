#ifndef CPP_HIGHLIGHT_H
#define CPP_HIGHLIGHT_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QtGui/QPlainTextEdit>

#define NONE            0
#define C_HIGHLIGHT     1
#define CPP_HIGHLIGHT   2
#define JAVA_HIGHLIGHT  3
#define XML_HIGHLIGHT   4

#include <QThread>

class hilight_thread : public QThread
{
public:
	hilight_thread(QTextDocument *doc) : doc_(doc) {}
	void run();
	
private:
	QTextDocument *doc_;
	
};


class Highlighter : public QSyntaxHighlighter
{
	 Q_OBJECT

 public:
     Highlighter(QTextDocument *parent = 0);
     ~Highlighter();
     
     void set_keyword_fg_color(const QBrush &brush);
     void set_keyword_bg_color(const QBrush &brush);

 protected:
     void highlightBlock(const QString &text);

 private:
     
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;

     QRegExp commentStartExpression;
     QRegExp commentEndExpression;

     QTextCharFormat keywordFormat;
     QTextCharFormat classFormat;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
     QTextCharFormat quotationFormat;
     QTextCharFormat functionFormat;
     QTextCharFormat preProcessorFormat;
};
/*
class syntaxHighlighter
{
    
    public:
    syntaxHighlighter(QPlainTextEdit *content);

};

class C_highlighter : public syntaxHighlighter
{
    public:// essa classe reimplementa o metodo virtual "set_format"
    C_highlighter(QPlainTextEdit *content){ };
    void setFormat();

};
*/
#endif



















