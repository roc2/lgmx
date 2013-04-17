#include <QTextStream>
#include <QPalette>
#include <QMessageBox>
#include <QString>
#include <QFileInfo>
#include <QTextBlock>
#include <QtGui/QApplication>
#include <QScrollBar>
#include <QTextOption>
#include <QtGui/QPainter>

#include <src_file.h>
#include <exception.h>
#include <debug.h>
#include <highlight_manager.h>
#include <src_container.h>
#include <settings.h>
#include <syntax_highlighter.h>
#include <QRect>

/**
 * [throw] Constructor.
 */

src_file::src_file(const QString &file_name, unsigned int id, src_container *parent, highlight_manager *hl_manager)
{
	clone_ = false;
	id_ = id;
	parent_ = parent;
	type_ = file_type::UNKNOWN;
	blink_cursor_ = false;

	this->setContentsMargins(0, 0, 0, 0);
    this->setObjectName(QString::fromUtf8("src_editor"));
    this->installEventFilter(this);
    
    this->setTabStopWidth(parent->get_settings()->get_tab_width() * 8);
    //this->setLineWrapMode(QPlainTextEdit::NoWrap);
    set_line_wrap(parent->get_settings()->get_line_wrap());
    
    //this->setOverwriteMode(true);
    
    /* file properties */
	if (file_name.isEmpty()) {
		file_info_ = new QFileInfo();
	} else {
		file_info_ = new QFileInfo(file_name);
		if (!load_file(file_name)) {      /* reads file from disk */
			delete file_info_;
			lgmx::exception excp("At src_file::src_file(): Unable to read file.");
			throw excp;
		}
	}

	if (hl_manager) {
		highlight_manager_ = hl_manager;
		highlighter_ = highlight_manager_->build_highlighter(this);
	} else {
		highlight_manager_ = NULL;
		highlighter_ = NULL;
	}

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

/**
 * Constructor for clone files. This is not a copy constructor, since some 
 * structures from the original file are shared. The file is created with 
 * the same QTextDocument and QFileInfo from base_file.
 */

src_file::src_file(src_file *base_file, src_container *parent)
{
	clone_ = true;
	id_ = base_file->get_id();
	parent_ = parent;
	type_ = base_file->get_file_type();
	blink_cursor_ = false;
	
	cursor_visible_ = true;
	mb_format_.setBackground(Qt::red);
	
	/* get file info and content from the base file */
	file_info_ = base_file->get_file_info();
	set_content(base_file->get_mutable_content());
	
	this->setContentsMargins(0, 0, 0, 0);
    this->setObjectName(QString::fromUtf8("src_editor"));
    this->setTabStopWidth(parent->get_settings()->get_tab_width() * 8);
    //this->setLineWrapMode(QPlainTextEdit::NoWrap);
    set_line_wrap(parent->get_settings()->get_line_wrap());
    this->installEventFilter(this);

    highlight_manager_ = NULL;
    //highlighter_ = NULL;
	highlighter_ = base_file->get_highlighter();
    
    // set default font
    set_default_font();
    
    // change editor colors
    set_base_color(Qt::white);
    set_text_color(Qt::black);

	//setCursorWidth(get_font_width());
	setCursorWidth(0);
	QApplication::setCursorFlashTime(1000);
	setBlinkingCursorEnabled(false);

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
	
	QScrollBar *scroll = verticalScrollBar();
	QObject *obj = (QObject *) scroll;		// ?????
	QObject::connect(obj, SIGNAL(valueChanged(int)), this, SLOT(highlight(int)));
	
	// when cursor changes highlight
	QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlight()));
	// when cursor changes check for matching braces
	QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(match_braces()));
}

/**
 * Destructor.
 */

src_file::~src_file()
{
	if (!clone_) {
		if (highlighter_)
			delete highlighter_;

		delete file_info_;
	}
}

/**
 * Reads file from disk and loads it to QPlainTextEdit.
 * @param fileName -> complete path of the file to be loaded.
 * @return true, if file loaded ok, false otherwise.
 */

bool src_file::load_file(const QString &fileName)
{
    QFile file(fileName);
    
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle(tr("Open"));
		msgBox.setText(tr("Cannot read file %1:\n%2.") .arg(fileName) .arg(file.errorString()));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();

        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    this->setPlainText(in.readAll());
    file.close();
    QApplication::restoreOverrideCursor();

    return true;
}

/**
 * Writes file content to disk.
 * @param fileName -> complete path of the file to be written.
 * @return true, if file written ok, false otherwise.
 */

bool src_file::write_file(const QString &fileName)
{
    QFile file(fileName);

	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, "lgmx",
				  tr("Cannot write file %1:\n%2.")
				  .arg(fileName)
				  .arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << this->toPlainText();
	file.close();
	QApplication::restoreOverrideCursor();
    
    return true;
}

/**
 * Checks whether the file has unsaved modifications or not.
 * @return true, if file has unsaved modifications, false otherwise.
 */

bool src_file::is_modified() const
{
    return this->document()->isModified();
}

/**
 * Sets the file modification state.
 * @param modified -> true, to mark the file as modified, false to mark the file
 * with no modifications.
 */

void src_file::set_modified(bool modified)
{
    this->document()->setModified(modified);
    emit modificationChanged(modified);
}

/**
 * Sets the file_info structure. This method applies for clone files only.
 * @param file_info -> new file info structure.
 */

void src_file::set_file_info(QFileInfo *file_info)
{
	if (file_info && clone_)
		file_info_ = file_info;
}

/**
 * Returns the file info structure.
 */

QFileInfo* src_file::get_file_info() const
{
	return file_info_;
}

/**
 * Sets the source file background color.
 * @param color - the color to be applied.
 */

void src_file::set_base_color(const QColor &color)
{
    QPalette p(this->palette());
    p.setColor(QPalette::Base, color);
    this->setPalette(p);
}

/**
 * Sets the source file text color.
 * @param color - the color to be applied.
 */

void src_file::set_text_color(const QColor &color)
{
    QPalette p(this->palette());
    p.setColor(QPalette::Text, color);
    this->setPalette(p);
}

/**
 * 
 */

bool src_file::set_tab_width(int size)
{
	if (size < 0)
		return false;

	setTabStopWidth(size * 8);
	return true;
}

/**
 * 
 */

int src_file::get_tab_width() const
{
	return tabStopWidth() / 8;
}

/**
 * Sets the line wrap mode.
 * @param on - if true, text wraps at widget's width, otherwise 
 * text does not wrap.
 */

void src_file::set_line_wrap(bool on)
{
	if (on)
		setLineWrapMode(QPlainTextEdit::WidgetWidth);
	else
		setLineWrapMode(QPlainTextEdit::NoWrap);
}

/**
 * 
 */

bool src_file::get_line_wrap() const
{
	if (lineWrapMode() == QPlainTextEdit::WidgetWidth)
		return true;
		
	return false;
}

/**
 * Returns the file type.
 */

file_type::type src_file::get_file_type() const
{
	return type_;
}

/**
 * Sets file type.
 */

void src_file::set_file_type(file_type::type type)
{
	type_ = type;
}

/**
 * Returns the word under cursor. It may be empty.
 */

QString src_file::get_word_under_cursor()
{
	QTextCursor cursor(textCursor());
	cursor.select(QTextCursor::WordUnderCursor);
	return cursor.selectedText();
}

/**
 * Shows/hides whites spaces.
 * @param show - if true, shows white spaces, otherwise hides white spaces.
 */

void src_file::show_white_spaces(bool show)
{
	QTextOption opt(document()->defaultTextOption());

	if (show)
		opt.setFlags(opt.flags() | QTextOption::ShowTabsAndSpaces);
	else
		opt.setFlags(opt.flags() & ~QTextOption::ShowTabsAndSpaces);

    document()->setDefaultTextOption(opt);
}

/**
 * Returns a copy of the file's content.
 */

QString src_file::get_content() const
{
    return this->toPlainText();
}

/**
 * Returns a pointer for the file's content.
 */

QTextDocument *src_file::get_mutable_content()
{
	return this->document();
}

/**
 * Sets the content of the file.
 * @param content - pointer to the new file content.
 */

void src_file::set_content(QTextDocument *content)
{
	this->setDocument(content);
	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

/**
 * Returns the file unique ID.
 */

unsigned int src_file::get_id() const
{
	return id_;
}

/**
 * Gets a copy of the file cursor.
 */

QTextCursor src_file::get_cursor()
{
	return this->textCursor();
}

/**
 * Returns the current position of the text cursor.
 */

int src_file::get_cursor_position()
{
	return this->textCursor().position();
}

/**
 * Sets the file cursor.
 * @param cursor - the cursor to be set.
 */

void src_file::set_cursor(const QTextCursor &cursor)
{
	this->setTextCursor(cursor);
}

/**
 * Sets the file name.
 * @param fileName - the new file name.
 */

void src_file::set_src_file_name(const QString &fileName)
{
    file_info_->setFile(fileName);
    //update_src_file_info();
    emit modificationChanged(this->is_modified());
}

/**
 * Returns the name of the file (without the path).
 * @return file name without the path.
 */

QString src_file::get_src_file_name() const
{
    return file_info_->fileName();
}

/**
 * Returns the file path. It does not include the file name
 */

QString src_file::get_src_file_path() const
{
    return file_info_->absolutePath();
}

/**
 * Returns the complete file name, path + name.
 */

QString src_file::get_src_file_full_name() const
{
    return file_info_->absoluteFilePath();
}

/**
 * Returns the complete file name (path + name) on the reference parameter.
 */

bool src_file::get_src_file_full_name(QString &file_path) const
{
    file_path = file_info_->absoluteFilePath();
    return true;
}

/**
 * Returns the file name extension. Ex: file.cpp - returns "cpp"
 */

QString src_file::get_src_file_extension() const
{
	return file_info_->suffix().toLower();
}

/**
 * Updates file information
 * @brief Updates file information
 */

void src_file::update_src_file_info()
{
    file_info_->refresh();
}

/**
 * Checks if the file exists on disk.
 */

bool src_file::exists() const
{
    return file_info_->exists();
}

/**
 * [slot] Moves the cursor to the beginning of the specified line.
 * @param line -> line number
 */

void src_file::go_to_line(int line)
{
    if (line > this->document()->lineCount())
        return;
    
    QTextCursor cursor(textCursor());
    int pos;
    
    pos = this->document()->findBlockByLineNumber(line - 1).position();
    cursor.setPosition(pos, QTextCursor::MoveAnchor);
    
    this->setTextCursor(cursor);
    this->centerCursor();
}

void src_file::highlight(int)
{
	highlight_visible_blocks();
}

void src_file::highlight()
{
	highlight_visible_blocks();
}

/**
 * Returns the matching brace and the direction within the text.
 * @param direction - return value for the direction that the cursor 
 * shall follow to find the matching character. 1 means forward, whereas
 *  -1 means backwards.
 * @return the matching character, or '\0' if unknown.
 */

QChar src_file::get_matching_brace(QChar c, int *direction)
{
	switch (c.unicode()) {
	case '{':
		*direction = 1;
		return '}';
	case '(':
		*direction = 1;
		return ')';
	case '[':
		*direction = 1;
		return ']';
	case '}':
		*direction = -1;
		return '{';
	case ')':
		*direction = -1;
		return '(';
	case ']':
		*direction = -1;
		return '[';
	default:
		break;
	}
	
	return '\0';
}

/**
 * [slot] Matches corresponding '{}', '()', '[]' and '<>'.
 */

void src_file::src_file::match_braces()
{
	match_braces(false);
}

/**
 * Matches corresponding '{}', '()' and '[]'.
 * @param select - If select is true, everything in between the matching
 * characters is selected, otherwise only the matching characters are 
 * highlighted.
 */

bool src_file::match_braces(bool select)
{
	QTextDocument *doc = document();
	QTextCursor cursor(textCursor());
	int start_pos = cursor.position();
	int direction = 1;
	bool prev_ch = false;

	QChar start_c = doc->characterAt(start_pos);
	QChar end_c = get_matching_brace(start_c, &direction);

	clear_extra_selections();

	if (end_c.isNull()) {
		// no match for the current char, let's try the previous one
		start_c = doc->characterAt(--start_pos);
		end_c = get_matching_brace(start_c, &direction);
		prev_ch = true;
		
		if (end_c.isNull())
			return false;
	}

	int end_pos = start_pos + direction;
	QChar curr;
	int depth = 0;
	bool match = false;

	while (!(curr = doc->characterAt(end_pos)).isNull()) {
		if (curr == start_c) {
			depth++;
		} else if (curr == end_c) {
			if (depth == 0) {
				match = true;
				break;
			} else {
				depth--;
			}
		}
		end_pos += direction;
	}
	
	if (match) {
		if (select) {
			if (direction == 1) {
				end_pos++;
				if (prev_ch)
					cursor.setPosition(start_pos, QTextCursor::MoveAnchor);
			} else if (!prev_ch) {
				cursor.setPosition(start_pos + 1, QTextCursor::MoveAnchor);
			}

			cursor.setPosition(end_pos, QTextCursor::KeepAnchor);
			setTextCursor(cursor);
		} else {
			highlight_maching_braces(start_pos, end_pos);
		}
	}
	
	return true;
}

/**
 * Marks the current matching braces.
 * @param start_pos - Cursor start position.
 * @param end_pos - Cursor end position.
 */

void src_file::highlight_maching_braces(int start_pos, int end_pos)
{
	QTextEdit::ExtraSelection sel;
	QList<QTextEdit::ExtraSelection> extra_selections;
	
	sel.format = mb_format_;
	sel.cursor = textCursor();
	sel.cursor.setPosition(start_pos);
	sel.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	extra_selections.append(sel);
	
	sel.cursor.setPosition(end_pos);
	sel.cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	extra_selections.append(sel);
	
	setExtraSelections(extra_selections);
}

/**
 * Clears all extra selections.
 */

void src_file::clear_extra_selections()
{
	QList<QTextEdit::ExtraSelection> extra_selections;
	QTextEdit::ExtraSelection sel;

	sel.cursor = textCursor();
	sel.cursor.clearSelection();
	extra_selections.append(sel);
	setExtraSelections(extra_selections);
}

/**
 * Focus in event handler.
 */

void src_file::focusInEvent(QFocusEvent*)
{
	// updates the view_manager current view.
	parent_->update_current_view();
	
	if (blink_cursor_)
		setBlinkingCursorEnabled(true);
}

/**
 * Focus out event handler.
 */

void src_file::focusOutEvent(QFocusEvent*)
{
	if (blink_cursor_)
		setBlinkingCursorEnabled(false);
}

/**
 * Returns the number of the first visible line.
 */

int src_file::get_first_visible_block()
{
	return firstVisibleBlock().blockNumber();
}

/**
 * Returns the first and last visible lines.
 * @param first - return value for the first visible line.
 * @param last - return value for the last visible line.
 */

void src_file::get_visible_blocks_range(int &first, int &last)
{
	QTextBlock block(firstVisibleBlock());
	first = block.blockNumber();
	
	int block_height = (int) blockBoundingRect(block).height();
	int content_height = (int) contentsRect().height();
	
	last = first + content_height / block_height + 1;
}

/**
 * Returns the first visible block and the number of visible blocks.
 * @param first_block - return value for the first visible block.
 * @return The number of visible blocks. 
 */

int src_file::get_visible_blocks(QTextBlock &first_block)
{
	first_block = firstVisibleBlock();
	int first_num = first_block.blockNumber();
	
	int block_height = (int) blockBoundingRect(first_block).height();
	int content_height = contentsRect().height();
	
	int last_num = first_num + content_height / block_height + 1;
	
	return last_num - first_num;
}

void src_file::highlight_visible_blocks()
{
	if (highlighter_) {
		QTextBlock first_block;
		int count = this->get_visible_blocks(first_block);
		highlighter_->highlight_blocks(first_block, count);
	}
}

syntax_highlighter *src_file::get_highlighter()
{
	return highlighter_;
}

/**
 * 
 */

QTextBlock src_file::get_text_block(int block)
{
	return document()->findBlockByNumber(block);
}

/**
 * 
 */

void src_file::setBlinkingCursorEnabled(bool enable)
{
    if (enable)
        cursor_blink_timer_.start(QApplication::cursorFlashTime() >> 1, this);
    else
        cursor_blink_timer_.stop();
}

void src_file::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == cursor_blink_timer_.timerId())
        update_cursor();
}

/**
 * @todo - the cursor should show the char behind it, and should 
 * not blink when it's moving.
 */

void src_file::update_cursor()
{
	QRect cursor_rect(cursorRect());
	
	//if (cursor_visible_)
		//setCursorWidth(0);
	//else
		//setCursorWidth(get_font_width());

	cursor_visible_ = !cursor_visible_;
	viewport()->update(cursor_rect);
}

/**
 * 
 */

bool src_file::indent_selection(bool forward)
{
	return false;	// not used for now
	
	QTextCursor cursor = textCursor();
    //cursor.beginEditBlock();

    if (!cursor.hasSelection())
		return false;

	int pos = cursor.position();
	int anchor = cursor.anchor();
	int start_pos = qMin(anchor, pos);
	int end_pos = qMax(anchor, pos);
	
	QTextDocument *doc = document();
	QTextBlock start_block = doc->findBlock(start_pos);
	QTextBlock end_block = doc->findBlock(end_pos - 1).next();

	if (start_block.next() == end_block && (start_pos > start_block.position() || end_pos < end_block.position() - 1)) {
		// Only one line partially selected.
		cursor.removeSelectedText();
	} else {
		for (QTextBlock block = start_block; block != end_block; block = block.next()) {
			QString text(block.text());
			
			//int indentPosition = tabSettings.lineIndentPosition(text);
			//if (!doIndent && !indentPosition)
				//indentPosition = tabSettings.firstNonSpace(text);
			//int targetColumn = tabSettings.indentedColumn(tabSettings.columnAt(text, indentPosition), doIndent);
			
			cursor.setPosition(block.position() + 4);
			//cursor.insertText(tabSettings.indentationString(0, targetColumn, block));
			cursor.insertText(text);
			cursor.setPosition(block.position());
			cursor.setPosition(block.position() + 4, QTextCursor::KeepAnchor);
			cursor.removeSelectedText();
		}
	}
	
	return true;
}


/*
void src_file::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
		debug(DEBUG, SRC_FILE, "Ctrl + left_click");
	} else 
		CodeEditor::mousePressEvent(event);
}
*/

/**
 * Event filter to handle double mouse clicks.
 */

void src_file::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
        if (match_braces(true)) {
			event->accept();
			return;
		}
	}
	
	QPlainTextEdit::mouseDoubleClickEvent(event);
}

/**
 * 
 */

void src_file::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {

	case Qt::Key_Tab:
    case Qt::Key_Backtab:
		debug(DEBUG, SRC_FILE, "tab");
		if (indent_selection(e->key() == Qt::Key_Tab)) {
			e->accept();
			return;
		}
		break;
	}
	
	QPlainTextEdit::keyPressEvent(e);
}

/**
 * 
 */

static void fillBackground(QPainter *p, const QRectF &rect, QBrush brush, QRectF gradientRect = QRectF())
{
	p->save();
	if (brush.style() >= Qt::LinearGradientPattern && brush.style() <= Qt::ConicalGradientPattern) {
		if (!gradientRect.isNull()) {
			QTransform m = QTransform::fromTranslate(gradientRect.left(), gradientRect.top());
			m.scale(gradientRect.width(), gradientRect.height());
			brush.setTransform(m);
			const_cast<QGradient *>(brush.gradient())->setCoordinateMode(QGradient::LogicalMode);
		}
	} else {
		p->setBrushOrigin(rect.topLeft());
	}
	p->fillRect(rect, brush);
	p->restore();
}

/**
 * Paint event. This is a copy of QPlainTextEdit::paintEvent with some 
 * customizations (fat cursor, highlight current line).
 */

void src_file::paintEvent(QPaintEvent *e)
{
	QPainter painter(viewport());
	Q_ASSERT(qobject_cast<QPlainTextDocumentLayout*>(document()->documentLayout()));

	QPointF offset(contentOffset());

	QRect er = e->rect();
	QRect viewportRect = viewport()->rect();

	bool editable = !isReadOnly();

	QTextBlock cursor_block = textCursor().block();
	QTextBlock block = firstVisibleBlock();
	qreal maximumWidth = document()->documentLayout()->documentSize().width();

	// keep right margin clean from full-width selection
	int maxX = offset.x() + qMax((qreal)viewportRect.width(), maximumWidth)
			   - document()->documentMargin();
	er.setRight(qMin(er.right(), maxX));
	painter.setClipRect(er);


	QAbstractTextDocumentLayout::PaintContext context = getPaintContext();

	while (block.isValid()) {

		QRectF r = blockBoundingRect(block).translated(offset);
		QTextLayout *layout = block.layout();

		if (!block.isVisible()) {
			offset.ry() += r.height();
			block = block.next();
			continue;
		}

		if (r.bottom() >= er.top() && r.top() <= er.bottom()) {

			QTextBlockFormat blockFormat = block.blockFormat();

			QBrush bg = blockFormat.background();
			if (bg != Qt::NoBrush) {
				QRectF contentsRect = r;
				contentsRect.setWidth(qMax(r.width(), maximumWidth));
				fillBackground(&painter, contentsRect, bg);
			}


			QVector<QTextLayout::FormatRange> selections;
			int blpos = block.position();
			int bllen = block.length();
			for (int i = 0; i < context.selections.size(); ++i) {
				const QAbstractTextDocumentLayout::Selection &range = context.selections.at(i);
				const int selStart = range.cursor.selectionStart() - blpos;
				const int selEnd = range.cursor.selectionEnd() - blpos;
				if (selStart < bllen && selEnd > 0
					&& selEnd > selStart) {
					QTextLayout::FormatRange o;
					o.start = selStart;
					o.length = selEnd - selStart;
					o.format = range.format;
					selections.append(o);
				}
#if 0
				 else if (!range.cursor.hasSelection() && range.format.hasProperty(QTextFormat::FullWidthSelection)
						   && block.contains(range.cursor.position())) {
					// for full width selections we don't require an actual selection, just
					// a position to specify the line. that's more convenience in usage.
					QTextLayout::FormatRange o;
					QTextLine l = layout->lineForTextPosition(range.cursor.position() - blpos);
					o.start = l.textStart();
					o.length = l.textLength();
					if (o.start + o.length == bllen - 1)
						++o.length; // include newline
					o.format = range.format;
					selections.append(o);
				}
#endif
            }

			bool drawCursor = false;

			// highlight current line
			if (block == cursor_block) {
				QRectF rr = layout->lineForTextPosition(textCursor().positionInBlock()).rect();
				rr.moveTop(rr.top() + r.top());
				rr.setLeft(0);
				rr.setRight(viewportRect.width() - offset.x());
				QColor color(200, 200, 200);
				// set alpha, otherwise we cannot see block highlighting and find scope underneath
				color.setAlpha(128);
				painter.fillRect(rr, color);
				drawCursor = true;
			}

			bool drawCursorAsBlock = drawCursor;

			if (drawCursorAsBlock) {
				int relativePos = context.cursorPosition - blpos;
				QTextLine line = layout->lineForTextPosition(relativePos);
				qreal x = line.cursorToX(relativePos);
				qreal w = get_font_width();

				QRectF rr = line.rect();
				rr.moveTop(rr.top() + r.top());
				rr.moveLeft(r.left() + x);
				rr.setWidth(w);
				painter.fillRect(rr, palette().text());

				QTextLayout::FormatRange o;
				o.start = relativePos;
				o.length = 1;
				o.format.setForeground(palette().base());
				selections.append(o);
			}

			layout->draw(&painter, offset, selections, er);
			if ((drawCursor && !drawCursorAsBlock)
				|| (editable && context.cursorPosition < -1
					&& !layout->preeditAreaText().isEmpty())) {
				int cpos = context.cursorPosition;
				if (cpos < -1)
					cpos = layout->preeditAreaPosition() - (cpos + 2);
				else
					cpos -= blpos;
				layout->drawCursor(&painter, offset, cpos, cursorWidth());
			}
		}

		offset.ry() += r.height();
		if (offset.y() > viewportRect.height())
			break;
		block = block.next();
	}

	if (backgroundVisible() && !block.isValid() && offset.y() <= er.bottom()
		&& (centerOnScroll() || verticalScrollBar()->maximum() == verticalScrollBar()->minimum())) {
		painter.fillRect(QRect(QPoint((int)er.left(), (int)offset.y()), er.bottomRight()), palette().background());
	}
}

/**
 * Events filter. For VI mode and shortcuts
 * 
 */

bool vi_mode = 0;

bool src_file::eventFilter(QObject* pObject, QEvent* pEvent)
{

    if (pEvent->type() == QEvent::KeyPress) {

        QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
        int PressedKey = pKeyEvent->key();

        if (PressedKey == Qt::Key_Escape) {
            
            vi_mode = true;
            debug(DEBUG, SRC_FILE, "VI mode on!!!");

            return true;
        } else if (vi_mode && PressedKey == Qt::Key_I) {
            
            vi_mode = false;
            debug(DEBUG, SRC_FILE, "Insert mode on!!!");
            
            return true;
        }
    }

    // standard event processing
    return QObject::eventFilter(pObject, pEvent);
}

