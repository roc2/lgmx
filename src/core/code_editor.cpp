#include <QPainter>
#include <QTextBlock>

#include <code_editor.h>
#include <debug.h>

/**
 * Constructor.
 */

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);
	this->setContentsMargins(0, 0, 0, 0);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(updateLineNumberArea(const QRect &, int)));

	updateLineNumberAreaWidth(0);
}

/**
 * Destructor.
 */

CodeEditor::~CodeEditor()
{
	delete lineNumberArea;
}

/**
 * Returns the file font.
 */

QFont CodeEditor::get_font() const
{
    return this->font();
}

/**
 * Sets the file font.
 * @param font -> font type.
 */

void CodeEditor::set_font(QFont &font)
{
    this->setFont(font);
}

/**
 * Returns the document font width in pixels.
 */

int CodeEditor::get_font_width() const
{
	return fontMetrics().width(QLatin1Char('9'));
}

/**
 * Sets the source file default font.
 */

void CodeEditor::set_default_font()
{
	QFont def("monospace", 10);
    def.setFixedPitch(true);
    
    this->setFont(def);
}

/**
 * Returns the width of the line number area based on the number of digits 
 * of the biggest line number.
 */

int CodeEditor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	int space;
	
	/* number of digits of the biggest number */
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	space = 2 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

/**
 * Updates the line number area according to the number of digits
 * of the highest line number.
 */

void CodeEditor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		lineNumberArea->scroll(0, dy);
	else
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(0);
}

/**
 * 
 */

void CodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/**
 * Line number area paint event slot.
 */

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block(firstVisibleBlock());
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                          Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

