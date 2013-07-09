#include <QFileInfo>
#include <QPainter>
#include <QTextBlock>
#include <QScrollBar>
#include <QApplication>

#include <visual_src_file.h>
#include <src_file.h>
#include <settings.h>
#include <src_container.h>
#include <syntax_highlighter.h>
#include <debug.h>

#include <iostream>
using namespace std;

visual_src_file::visual_src_file(src_file *parent_file, src_container *container) : settings_(parent_file->get_settings())
{
	parent_file_ = parent_file;
	container_ = container;

	// get file info and content from the parent file
	file_info_ = parent_file->file_info_;
	set_content(parent_file->document());

	blink_cursor_ = false;
	setBlinkingCursorEnabled(false);

	cursor_visible_ = true;
	mb_format_.setBackground(Qt::red);

	type_ = parent_file->type_;
	id_ = parent_file->get_id();
	highlighter_ = parent_file->get_highlighter();
	cursor_visible_ = true;
	blink_cursor_ = true;
	set_default_font();
	set_line_wrap(false);
	QApplication::setCursorFlashTime(1000);

	// set colours
	set_base_color(Qt::white);
	set_text_color(Qt::black);

	set_modified(parent_file->is_modified());

	// when cursor changes check for matching braces
	QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(match_braces()));
	// when cursor changes highlight
	QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlight()));
}

visual_src_file& visual_src_file::operator=(const visual_src_file &other)
{
	if (this == &other)
		return *this;

	file_info_ = other.file_info_;
	set_content(other.document());

	id_ = other.id_;
	cursor_visible_ = other.cursor_visible_;
	blink_cursor_ = other.blink_cursor_;

	return *this;
}

void visual_src_file::set_content(QTextDocument *content)
{
	setDocument(content);
	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

bool visual_src_file::load_file(const QString &fileName)
{
	return parent_file_->load_file(fileName);
}

/**
 * Sets the source file default font.
 */

void visual_src_file::set_default_font()
{
	QFont def("monospace", 10);
	def.setFixedPitch(true);

	this->setFont(def);
}

/**
 * Sets the source file background color.
 * @param color - the color to be applied.
 */

void visual_src_file::set_base_color(const QColor &color)
{
	QPalette p(this->palette());
	p.setColor(QPalette::Base, color);
	this->setPalette(p);
}

/**
 * Sets the source file text color.
 * @param color - the color to be applied.
 */

void visual_src_file::set_text_color(const QColor &color)
{
	QPalette p(this->palette());
	p.setColor(QPalette::Text, color);
	this->setPalette(p);
}

/**
 * Writes file content to disk.
 * @param fileName -> complete path of the file to be written.
 * @return true, if file written ok, false otherwise.
 */

bool visual_src_file::write_file(const QString &fileName)
{
	return parent_file_->write_file(fileName);
}

/**
 * Returns a pointer for the file's content.
 */

QTextDocument *visual_src_file::get_mutable_content()
{
	return this->document();
}

const src_file* visual_src_file::get_root_file()
{
	return parent_file_;
}

/**
 * Returns the name of the file (without the path).
 * @return file name without the path.
 */

QString visual_src_file::get_file_name() const
{
	return file_info_->fileName();
}

/**
 * Returns the complete file name, path + name.
 */

QString visual_src_file::get_src_file_full_name() const
{
    return file_info_->absoluteFilePath();
}

/**
 * Returns the complete file name (path + name) on the reference parameter.
 */

bool visual_src_file::get_src_file_full_name(QString &file_path) const
{
	file_path = file_info_->absoluteFilePath();
	return true;
}

/**
 * Returns the file path. It does not include the file name
 */

QString visual_src_file::get_src_file_path() const
{
	return file_info_->absolutePath();
}

/**
 * Returns the file name extension. Ex: file.cpp - returns "cpp"
 */

QString visual_src_file::get_src_file_extension() const
{
	return file_info_->suffix().toLower();
}

/**
 * Sets the file name.
 * @param fileName - the new file name.
 */

void visual_src_file::set_src_file_name(const QString &fileName)
{
	parent_file_->set_src_file_name(fileName);
}

/**
 * Sets the file modification state.
 * @param modified -> true, to mark the file as modified, false to mark the file
 * with no modifications.
 */

void visual_src_file::set_modified(bool modified)
{
	document()->setModified(modified);
	emit modificationChanged(modified);
}

/**
 * Returns the word under cursor. It may be empty.
 */

QString visual_src_file::get_word_under_cursor()
{
	QTextCursor cursor(textCursor());
	cursor.select(QTextCursor::WordUnderCursor);
	return cursor.selectedText();
}

/**
 * Shows/hides whites spaces.
 * @param show - if true, shows white spaces, otherwise hides white spaces.
 */

void visual_src_file::show_white_spaces(bool show)
{
	QTextOption opt(document()->defaultTextOption());

	if (show)
		opt.setFlags(opt.flags() | QTextOption::ShowTabsAndSpaces);
	else
		opt.setFlags(opt.flags() & ~QTextOption::ShowTabsAndSpaces);

    document()->setDefaultTextOption(opt);
}

/**
 * Returns the file unique ID.
 */

unsigned int visual_src_file::get_id() const
{
	return id_;
}

/**
 * Checks if the file exists on disk.
 */

bool visual_src_file::exists() const
{
    return file_info_->exists();
}

/**
 * Checks whether the file has unsaved modifications or not.
 * @return true, if file has unsaved modifications, false otherwise.
 */

bool visual_src_file::is_modified() const
{
	return document()->isModified();
}

/**
 * Updates file information
 * @brief Updates file information
 */

void visual_src_file::update_src_file_info()
{
	parent_file_->update_src_file_info();
}

/**
 * Sets the line wrap mode.
 * @param on - if true, text wraps at widget's width, otherwise
 * text does not wrap.
 */

void visual_src_file::set_line_wrap(bool wrap)
{
	if (wrap)
		setLineWrapMode(QPlainTextEdit::WidgetWidth);
	else
		setLineWrapMode(QPlainTextEdit::NoWrap);
}

/**
 *
 */

bool visual_src_file::set_tab_width(int size)
{
	if (size < 0)
		return false;

	setTabStopWidth(size * 8);
	return true;
}

/**
 *
 */

int visual_src_file::get_tab_width() const
{
	return tabStopWidth() / 8;
}

/**
 *
 */

bool visual_src_file::get_line_wrap() const
{
	return (lineWrapMode() == QPlainTextEdit::WidgetWidth);
}

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
 * Focus in event handler.
 */

void visual_src_file::focusInEvent(QFocusEvent*)
{
	// updates the view_manager current view.
	container_->update_current_view();

	if (blink_cursor_)
		setBlinkingCursorEnabled(true);
}

/**
 * Focus out event handler.
 */

void visual_src_file::focusOutEvent(QFocusEvent*)
{
	if (blink_cursor_)
		setBlinkingCursorEnabled(false);
}

/**
 * Paint event. This is a copy of QPlainTextEdit::paintEvent with some
 * customizations (fat cursor, highlight current line).
 */

void visual_src_file::paintEvent(QPaintEvent *e)
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
				//int relativePos = context.cursorPosition - blpos;
				int relativePos = textCursor().position() - blpos;
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
 * [slot] Moves the cursor to the beginning of the specified line.
 * @param line -> line number
 */

void visual_src_file::go_to_line(int line)
{
	if (line > document()->lineCount())
		return;

	QTextCursor cursor(textCursor());
	int pos;

	pos = document()->findBlockByLineNumber(line - 1).position();
	cursor.setPosition(pos, QTextCursor::MoveAnchor);

	setTextCursor(cursor);
	centerCursor();
}

/**
 * [slot] Removes all trailing spaces from the document.
 */

void visual_src_file::strip_trailing_spaces()
{
	QTextDocument *doc = document();
	QTextCursor cursor(textCursor());
	QChar c;

	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

	do {
		cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
		c = doc->characterAt(cursor.position() - 1);

		while (c == ' ' || c == '\t') {
			cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
			c = doc->characterAt(cursor.position() - 1);
		}

		cursor.removeSelectedText();
	} while (cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor));
}

/**
 * Gets a copy of the file cursor.
 */

QTextCursor visual_src_file::get_cursor()
{
	return this->textCursor();
}

/**
 * Returns the current position of the text cursor.
 */

int visual_src_file::get_cursor_position()
{
	return this->textCursor().position();
}

/**
 * Sets the file cursor.
 * @param cursor - the cursor to be set.
 */

void visual_src_file::set_cursor(const QTextCursor &cursor)
{
	this->setTextCursor(cursor);
}
/**
 *
 */

void visual_src_file::setBlinkingCursorEnabled(bool enable)
{
	if (enable)
		cursor_blink_timer_.start(QApplication::cursorFlashTime() >> 1, this);
	else
		cursor_blink_timer_.stop();
}

void visual_src_file::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == cursor_blink_timer_.timerId())
		update_cursor();
}

/**
 * @todo - the cursor should show the char behind it, and should
 * not blink when it's moving.
 */

void visual_src_file::update_cursor()
{
	QRect cursor_rect(cursorRect());

	if (cursor_visible_)
		setCursorWidth(0);
	else
		setCursorWidth(get_font_width());

	cursor_visible_ = !cursor_visible_;
	viewport()->update(cursor_rect);
}

/**
 * Returns the matching brace and the direction within the text.
 * @param direction - return value for the direction that the cursor
 * shall follow to find the matching character. 1 means forward, whereas
 *  -1 means backwards.
 * @return the matching character, or '\0' if unknown.
 */

QChar visual_src_file::get_matching_brace(QChar c, int *direction)
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

void visual_src_file::match_braces()
{
	match_braces(false);
	//this->repaint();	// repaints the widget
}

/**
 * Matches corresponding '{}', '()' and '[]'.
 * @param select - If select is true, everything in between the matching
 * characters is selected, otherwise only the matching characters are
 * highlighted.
 */

bool visual_src_file::match_braces(bool select)
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

void visual_src_file::highlight_maching_braces(int start_pos, int end_pos)
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

void visual_src_file::clear_extra_selections()
{
	QList<QTextEdit::ExtraSelection> extra_selections;
	QTextEdit::ExtraSelection sel;

	sel.cursor = textCursor();
	sel.cursor.clearSelection();
	extra_selections.append(sel);
	setExtraSelections(extra_selections);
}

void visual_src_file::insert_tab(QTextCursor &cursor)
{
	if (settings_.get_tab_settings().get_tab_policy() == tab_settings::tabs_only) {
		cursor.insertText("\t");
	} else {
		int num_spaces = settings_.get_tab_settings().get_tab_width();
		QString text(num_spaces, ' ');
		cursor.insertText(text);
		debug(DEBUG, V_SRC_FILE, "inserted spaces");
	}
}

/**
 *
 */

bool visual_src_file::indent_selection(bool forward)
{
	QTextCursor cursor = textCursor();

	if (!cursor.hasSelection()) {
		insert_tab(cursor);
		return true;
	}

	int pos = cursor.position();
	int anchor = cursor.anchor();
	int start_pos = qMin(anchor, pos);
	int end_pos = qMax(anchor, pos);

	QTextDocument *doc = document();
	QTextBlock start_block = doc->findBlock(start_pos);
	QTextBlock end_block = doc->findBlock(end_pos - 1).next();

	cursor.beginEditBlock();

	if (start_block.next() == end_block && (start_pos > start_block.position() || end_pos < end_block.position() - 1)) {
		// Only one line partially selected.
		cursor.removeSelectedText();
		insert_tab(cursor);
	} else {
		for (QTextBlock block = start_block; block != end_block; block = block.next()) {
			QString text(block.text());

			cursor.setPosition(block.position());
			insert_tab(cursor);
			//int indentPosition = tabSettings.lineIndentPosition(text);
			//if (!doIndent && !indentPosition)
			//indentPosition = tabSettings.firstNonSpace(text);
			//int targetColumn = tabSettings.indentedColumn(tabSettings.columnAt(text, indentPosition), doIndent);
			//cursor.insertText(tabSettings.indentationString(0, targetColumn, block));
			//cursor.setPosition(block.position());
			//cursor.setPosition(block.position() + 4, QTextCursor::KeepAnchor);
			//cursor.removeSelectedText();
		}
	}

	cursor.endEditBlock();
	return true;
}

void visual_src_file::highlight(int)
{
	highlight_visible_blocks();
}

void visual_src_file::highlight()
{
	highlight_visible_blocks();
}

/**
 * Returns the number of the first visible line.
 */

int visual_src_file::get_first_visible_block()
{
	return firstVisibleBlock().blockNumber();
}

/**
 * Returns the first and last visible lines.
 * @param first - return value for the first visible line.
 * @param last - return value for the last visible line.
 */

void visual_src_file::get_visible_blocks_range(int &first, int &last)
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

int visual_src_file::get_visible_blocks(QTextBlock &first_block)
{
	first_block = firstVisibleBlock();
	int first_num = first_block.blockNumber();

	int block_height = (int) blockBoundingRect(first_block).height();
	int content_height = contentsRect().height();

	int last_num = first_num + content_height / block_height + 1;

	return last_num - first_num;
}

void visual_src_file::highlight_visible_blocks()
{
	if (highlighter_) {
		QTextBlock first_block;
		int count = this->get_visible_blocks(first_block);
		highlighter_->highlight_blocks(first_block, count);
	}
}

/**
 * Event filter to handle double mouse clicks.
 */

void visual_src_file::mouseDoubleClickEvent(QMouseEvent *event)
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
 * Event filter to handle single mouse clicks.
 */

void visual_src_file::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		debug(DEBUG, V_SRC_FILE, "right click");
	} else if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier) {
		debug(DEBUG, SRC_FILE, "Ctrl + left_click");
	} else {
		QPlainTextEdit::mousePressEvent(event);
	}
}

/**
 *
 */

void visual_src_file::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {

	//case Qt::Key_Backtab:
	case Qt::Key_Tab:
		if (indent_selection(e->key() == Qt::Key_Tab)) {
			e->accept();
			return;
		}
		break;
	}

	QPlainTextEdit::keyPressEvent(e);
}

