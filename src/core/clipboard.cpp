#include <clipboard.h>
#include <sstream>

/**
 * Constructor.
 */

clipboard::clipboard(view_manager &manager, QWidget *parent) : manager_(manager), copy_signal_map(this), 
					 cut_signal_map(this), paste_signal_map(this)
{
	int i;

	// set copy shortcuts
	for (i = 0; i < NUM_SHORTCUTS; i++) {
		
		std::ostringstream stream;
		stream << "Ctrl+" << i << ", Ctrl+C";

		copy_s[i] = new QShortcut(tr(stream.str().c_str()), parent);
		
		copy_signal_map.setMapping(copy_s[i], i);
		connect(copy_s[i], SIGNAL(activated()), &copy_signal_map, SLOT(map()));
	}
	
	// set cut shortcuts
	for (i = 0; i < NUM_SHORTCUTS; i++) {
		
		std::ostringstream stream;
		stream << "Ctrl+" << i << ", Ctrl+X";

		cut_s[i] = new QShortcut(tr(stream.str().c_str()), parent);
		
		cut_signal_map.setMapping(cut_s[i], i);
		connect(cut_s[i], SIGNAL(activated()), &cut_signal_map, SLOT(map()));
	}
	
	// set paste shortcuts
	for (i = 0; i < NUM_SHORTCUTS; i++) {
		
		std::ostringstream stream;
		stream << "Ctrl+" << i << ", Ctrl+V";

		paste_s[i] = new QShortcut(tr(stream.str().c_str()), parent);
		
		paste_signal_map.setMapping(paste_s[i], i);
		connect(paste_s[i], SIGNAL(activated()), &paste_signal_map, SLOT(map()));
	}

	connect(&copy_signal_map, SIGNAL(mapped(int)), this, SLOT(copy_buff(int)));
	connect(&cut_signal_map, SIGNAL(mapped(int)), this, SLOT(cut_buff(int)));
	connect(&paste_signal_map, SIGNAL(mapped(int)), this, SLOT(paste_buff(int)));
}

/**
 * Destructor.
 */

clipboard::~clipboard()
{
	for (int i = 0; i < NUM_SHORTCUTS; i++) {
		delete copy_s[i];
		delete cut_s[i];
		delete paste_s[i];
	}
}

/**
 * Copy selected text to buffer.
 */

void clipboard::copy_buff(int index)
{
	src_file *curr_file;
	QTextCursor cursor;
	
	if (!(curr_file = manager_.get_current_src_file()))
		return;

	if ((cursor = curr_file->get_cursor()).hasSelection())
		buffers[index] = cursor.selectedText();
}

/**
 * Cut selected text to buffer.
 */

void clipboard::cut_buff(int index)
{
	src_file *curr_file;
	QTextCursor cursor;
	
	if (!(curr_file = manager_.get_current_src_file()))
		return;

	if ((cursor = curr_file->get_cursor()).hasSelection()) {
		buffers[index] = cursor.selectedText();
		cursor.deleteChar();
	}
}

/**
 * Paste selected text.
 */

void clipboard::paste_buff(int index)
{
	src_file *curr_file;
	
	if (!(curr_file = manager_.get_current_src_file()))
		return;

	curr_file->get_cursor().insertText(buffers[index]);
}

