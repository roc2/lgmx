#include <syntax_highlighter.h>
#include <src_file.h>
#include <QTextCursor>
#include <QVector>

QVector<QTextCharFormat> formatChanges;

syntax_highlighter::syntax_highlighter(src_file *file) : file_(file)
{
	cursor_ = new QTextCursor(file->document());
	// connect(this, scroll(), this, highlight_visible_blocks);
	// connect(text->document, contentsChange(int position, int charsRemoved, int charsAdded), this, highlight_block(int, int, int));
}


syntax_highlighter::~syntax_highlighter()
{
	delete cursor_;
}

void syntax_highlighter::highlight_blocks(QTextBlock &first, int count)
{
	curr_block_ = first;
	prev_block_ = curr_block_.previous();

	if (curr_block_.isValid()) {
		highlight_block(curr_block_.text());
		applyFormatChanges();
		formatChanges.clear();
	}
		
	for (int i = 0; i < count; i++) {
		prev_block_ = curr_block_;
		curr_block_ = curr_block_.next();
		
		if (!curr_block_.isValid())
			continue;
	
		highlight_block(curr_block_.text());
		applyFormatChanges();
		formatChanges.clear();
	}
}

/**
 * Applies the specified format to text.
 * @param start - position of the first character in the block 
 * to receive format.
 * @param count - number of characters to receive format.
 * @param format - format to be applied.
 */

void syntax_highlighter::set_format(int start, int count, const QTextCharFormat &format)
{
	if (start < 0 || start >= formatChanges.count())
		return;

	const int end = qMin(start + count, formatChanges.count());
	for (int i = start; i < end; ++i)
		formatChanges[i] = format;
}

/**
 * 
 */

void syntax_highlighter::applyFormatChanges()
{
	bool formatsChanged = false;

	QTextLayout *layout = curr_block_.layout();

	QList<QTextLayout::FormatRange> ranges = layout->additionalFormats();

	const int preeditAreaStart = layout->preeditAreaPosition();
	const int preeditAreaLength = layout->preeditAreaText().length();

	if (preeditAreaLength != 0) {
		QList<QTextLayout::FormatRange>::Iterator it = ranges.begin();
		while (it != ranges.end()) {
			if (it->start >= preeditAreaStart
				&& it->start + it->length <= preeditAreaStart + preeditAreaLength) {
				++it;
			} else {
				it = ranges.erase(it);
				formatsChanged = true;
			}
		}
	} else if (!ranges.isEmpty()) {
		ranges.clear();
		formatsChanged = true;
	}

	QTextCharFormat emptyFormat;

	QTextLayout::FormatRange r;
	r.start = -1;

	int i = 0;
	while (i < formatChanges.count()) {

		while (i < formatChanges.count() && formatChanges.at(i) == emptyFormat)
			++i;

		if (i >= formatChanges.count())
			break;

		r.start = i;
		r.format = formatChanges.at(i);

		while (i < formatChanges.count() && formatChanges.at(i) == r.format)
			++i;

		if (i >= formatChanges.count())
			break;

		r.length = i - r.start;

		if (preeditAreaLength != 0) {
			if (r.start >= preeditAreaStart)
				r.start += preeditAreaLength;
			else if (r.start + r.length >= preeditAreaStart)
				r.length += preeditAreaLength;
		}

		ranges << r;
		formatsChanged = true;
		r.start = -1;
	}

	if (r.start != -1) {
		r.length = formatChanges.count() - r.start;

		if (preeditAreaLength != 0) {
			if (r.start >= preeditAreaStart)
				r.start += preeditAreaLength;
			else if (r.start + r.length >= preeditAreaStart)
				r.length += preeditAreaLength;
		}

		ranges << r;
		formatsChanged = true;
	}

	if (formatsChanged) {
		layout->setAdditionalFormats(ranges);
		file_->document()->markContentsDirty(curr_block_.position(), curr_block_.length());
	}
}

/**
 * 
 */

int syntax_highlighter::get_current_block_state() const
{
	return curr_block_.userState();
}

/**
 * 
 */
 
void syntax_highlighter::set_current_block_state(int state)
{
	curr_block_.setUserState(state);
}

/**
 * 
 */

int syntax_highlighter::get_previous_block_state() const
{
	// if this is the first block, we assume the previous block is highlighted
	if (curr_block_.blockNumber() == 0)
		return 0;
	
	return curr_block_.previous().userState();
}

