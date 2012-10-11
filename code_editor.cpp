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
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}

/**
 * Destructor.
 */

CodeEditor::~CodeEditor()
{
	delete lineNumberArea;
}

/**
 * Teste
 */

void CodeEditor::print_visible_blocks()
{
	QTextBlock block = firstVisibleBlock();

	while (block.isValid() && block.isVisible()) {
		debug(INFO, CODE_EDITOR, block.text().toStdString());
		block = block.next();
	}
}


void CodeEditor::get_first_visible_block_content()
{
    QTextBlock block;
    QTextCharFormat keywordFormat;
    QString value;
    QTextCursor cursor = this->textCursor();
    
    cursor.movePosition(QTextCursor::Start); 
    cursor.movePosition(QTextCursor::NextCharacter);
    cursor.movePosition(QTextCursor::NextCharacter);
    cursor.movePosition(QTextCursor::NextCharacter);
    
    keywordFormat.setForeground(Qt::red);
    block = firstVisibleBlock();
    
    value = block.text();
    
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setBackground(QColor("yellow"));
    blockFormat.setNonBreakableLines(true);
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    cursor.setBlockFormat(blockFormat);

    for (QTextBlock::iterator it = cursor.block().begin(); !(it.atEnd()); ++it) {
        QTextCharFormat charFormat = it.fragment().charFormat();
        charFormat.setFont(QFont("Times", 15, QFont::Bold));

        QTextCursor tempCursor = cursor;
        tempCursor.setPosition(it.fragment().position());
        tempCursor.setPosition(it.fragment().position() + it.fragment().length(), QTextCursor::KeepAnchor);
        tempCursor.setCharFormat(charFormat);
    }
    
    //mycursor.setBlockFormat(keywordFormat);
    cursor.mergeBlockCharFormat(keywordFormat);
    
    debug(INFO, CODE_EDITOR, "position " << cursor.positionInBlock());
    
    //return block;
    /*
    for (int i = 0; i < 5 ; i++) {
        cout << "num = " << block.blockNumber () << endl;
        cout << block.text().toStdString() << endl;
        block = block.next();

        cout << "num = " << block.blockNumber () << endl;
        cout << block.text().toStdString() << endl;
        block = block.next();
    }
    return block.text();
    */
}

/*
void CodeEditor::focusInEvent(QFocusEvent *event)
{
	cout << "Editor has focus!!" << endl;
	//_parent->set_current_widget(this);
}*/

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
 * Highlights the current line.
 */

void CodeEditor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::green).lighter(170);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

/**
 * Line number area paint event slot.
 */

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
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

