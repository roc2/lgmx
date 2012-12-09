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
	
	delete_line = new QShortcut(Qt::CTRL+ Qt::Key_K, parent);
	connect(delete_line, SIGNAL(activated()), this, SLOT(delete_current_line()));
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

/**
 * Removes the line where the cursor is positioned in.
 */
 
void text_manip::delete_current_line()
{
	src_file *curr_file;
	QTextCursor cursor;
	
	if (!(curr_file = manager_.get_current_src_file()))
		return;

	cursor = curr_file->get_cursor();
	cursor.select(QTextCursor::BlockUnderCursor);
	
	if (cursor.hasSelection()) {
		cursor.removeSelectedText();
		cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
		curr_file->set_cursor(cursor);
	} else {
		cursor.deleteChar();
	}
}

/*
void FakeVimPluginPrivate::moveToMatchingParenthesis(bool *moved, bool *forward,
        QTextCursor *cursor)
{
    *moved = false;

    bool undoFakeEOL = false;
    if (cursor->atBlockEnd() && cursor->block().length() > 1) {
        cursor->movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
        undoFakeEOL = true;
    }
    TextBlockUserData::MatchType match
        = TextBlockUserData::matchCursorForward(cursor);
    if (match == TextBlockUserData::Match) {
        *moved = true;
        *forward = true;
    } else {
        if (undoFakeEOL)
            cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
        if (match == TextBlockUserData::NoMatch) {
            // Backward matching is according to the character before the cursor.
            bool undoMove = false;
            if (!cursor->atBlockEnd()) {
                cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                undoMove = true;
            }
            match = TextBlockUserData::matchCursorBackward(cursor);
            if (match == TextBlockUserData::Match) {
                *moved = true;
                *forward = false;
            } else if (undoMove) {
                cursor->movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
            }
        }
    }
}


void FakeVimHandler::Private::updateCursorShape()
{
    bool thinCursor = m_mode == ExMode
            || m_subsubmode == SearchSubSubMode
            || m_mode == InsertMode
            || isVisualMode()
            || cursor().hasSelection();
    EDITOR(setOverwriteMode(!thinCursor));
}
*/
