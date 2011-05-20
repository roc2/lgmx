#ifndef CPP_HIGHLIGHT_H
#define CPP_HIGHLIGHT_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter
{
	 Q_OBJECT

 public:
     Highlighter(QTextDocument *parent = 0);
     
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

#endif

