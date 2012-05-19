#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QtGui/QPlainTextEdit>
#include <QtCore/QObject>
#include <QTextBlock>
#include <QTextCursor>
#include "highlight/cpp_hl.h"

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;


class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget *parent = 0);
	~CodeEditor();

	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();

	//QString get_text_block_content();
	void setHighlighter(int srcType);
	void get_first_visible_block_content();
	
	//void focusInEvent(QFocusEvent *event);
	void print_visible_blocks();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);

private:
	QWidget *lineNumberArea;
	//syntaxHighlighter *highlighter;
};



class LineNumberArea : public QWidget
{
public:
	LineNumberArea(CodeEditor *editor) : QWidget(editor) {
		codeEditor = editor;
	}

	QSize sizeHint() const {
		return QSize(codeEditor->lineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) {
		codeEditor->lineNumberAreaPaintEvent(event);
	}

private:
	CodeEditor *codeEditor;
};

#endif


