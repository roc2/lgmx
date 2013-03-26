#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QtGui/QPlainTextEdit>
#include <QtCore/QObject>
#include <QColor>

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

	void setHighlighter(int srcType);
	
	//void focusInEvent(QFocusEvent *event);
	
	QFont get_font() const;
	void set_font(QFont &font);
	int get_font_width() const;
	void set_default_font();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);

private:
	QWidget *lineNumberArea;
	QColor line_colour_; /**< current line highlight colour */
	QTextEdit::ExtraSelection selection;
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


