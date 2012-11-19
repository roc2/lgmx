#include <QShortcut>
#include <QTextBlock>

#include <text_manip.h>
#include <src_file.h>

#include <iostream>

using namespace std;

/**
 * Constructor.
 */

text_manip::text_manip(view_manager &manager, QWidget *parent) : manager_(manager)
{
	dup_up = new QShortcut(Qt::CTRL+ Qt::ALT + Qt::Key_Up, parent);
	connect(dup_up, SIGNAL(activated()), this, SLOT(duplicate_up()));
	
	dup_down = new QShortcut(Qt::CTRL+ Qt::ALT + Qt::Key_Down, parent);
	connect(dup_down, SIGNAL(activated()), this, SLOT(duplicate_down()));
}

/**
 * Destructor.
 */

text_manip::~text_manip()
{
	delete dup_up;
	delete dup_down;
}

/**
 * Duplicates the text and inserts the new text above the original one.
 * The selected text is duplicated, or the current line if there is no selection.
 */

void text_manip::duplicate_up()
{
	src_file *curr_file;
	QTextCursor cursor;
	QString buff;
	
	if (!(curr_file = manager_.get_current_src_file()))
		return;

	cursor = curr_file->get_cursor();

	if (cursor.hasSelection()) {	// copy current selected text
		buff = cursor.selectedText() + '\n';
		int pos = cursor.selectionStart();
		cursor.setPosition(pos, QTextCursor::MoveAnchor);
	} else {	// copy current line up
		buff = cursor.block().text() + '\n';
		cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
	}
	
	cursor.insertText(buff);
	cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::MoveAnchor);
	cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
	curr_file->set_cursor(cursor);
}

/**
 * Duplicates the text and inserts the new text below the original one.
 * The selected text is duplicated, or the current line if there is no selection.
 */

void text_manip::duplicate_down()
{
	src_file *curr_file;
	QTextCursor cursor;
	QString buff;
	
	if (!(curr_file = manager_.get_current_src_file()))
		return;

	cursor = curr_file->get_cursor();

	if (cursor.hasSelection()) {	// copy current selected text
		buff = '\n' + cursor.selectedText();
		int pos = cursor.selectionEnd();
		cursor.setPosition(pos, QTextCursor::MoveAnchor);
	} else {	// copy current line down
		buff = '\n' + cursor.block().text();
		cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
	}
	
	cursor.insertText(buff);
	curr_file->set_cursor(cursor);
}

