#include <QtGui>
#include "cpp_hl.h"
#include <iostream>

using namespace std;

/**
 * Constructor
 * @param parent -> QTextDocument where the highlight will be applied to
 */

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

    /* o parametro aqui eh QColor, "green" eh por ex um QColor pre definido do qt */
	keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setBackground(Qt::transparent);
	keywordFormat.setFontWeight(QFont::Bold);
	
	QStringList keywordPatterns;
	
    /* language reserved words */
	keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                     << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b" << "\\bfloat\\b"
                     << "\\bfriend\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b" << "\\bint\\b"
                     << "\\blong\\b" << "\\bnamespace\\b" << "\\bNULL\\b" << "\\boperator\\b"
                     << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\breturn\\b"
                     << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                     << "\\bslots\\b" << "\\bstatic\\b" << "\\bstring\\b" << "\\bstruct\\b"
                     << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                     << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                     << "\\bvoid\\b" << "\\bvolatile\\b";
	
	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(Qt::red);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::red);

	quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	functionFormat.setFontItalic(true);
	functionFormat.setForeground(Qt::blue);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

/*
passar somente o textblock visível para a funçao
*/



void Highlighter::highlightBlock(const QString &text)
{
	//QString::iterator it;
	bool in_comment = false;
	
	//for (QString::const_iterator it = text.begin(); it != text.end(); it++) {
/*	
	QString::const_iterator it = text.begin()
	
	if (*it == '/' && *(it + 1) == '*') {
		in_comment = true;
		while (*it != '*' && *(it + 1) != '/')
			it++;
		
		in_comment = false;
	}
	
	return;
*/	
    foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }
     setCurrentBlockState(0);

     int startIndex = 0;
     if (previousBlockState() != 1)
         startIndex = commentStartExpression.indexIn(text);

     while (startIndex >= 0) {
         int endIndex = commentEndExpression.indexIn(text, startIndex);
         int commentLength;
         if (endIndex == -1) {
             setCurrentBlockState(1);
             commentLength = text.length() - startIndex;
         } else {
             commentLength = endIndex - startIndex
                             + commentEndExpression.matchedLength();
         }
         setFormat(startIndex, commentLength, multiLineCommentFormat);
         startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
     }

}

/**
 * Sets keywords foreground color
 */

void Highlighter::set_keyword_fg_color(const QBrush &brush)
{
    keywordFormat.setForeground(brush);
}

/**
 * Sets keywords background color
 */

void Highlighter::set_keyword_bg_color(const QBrush &brush)
{
    keywordFormat.setBackground(brush);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
syntaxHighlighter::syntaxHighlighter(QPlainTextEdit *content)
{
    QTextBlock block;
    
 //   block = content->firstVisibleBlock();
   
    for (int i = 0; i < 5 ; i++) {
        cout << "num = " << block.blockNumber () << endl;
        cout << block.text().toStdString() << endl;
        block = block.next();
    }
*/









