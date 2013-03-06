#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>

#include <src_container.h>
#include <debug.h>
#include <exception.h>
#include <view_manager.h>
#include <settings.h>

/**
 * Constructor.
 * @param parent -> parent widget.
 */

src_container::src_container(view_manager *manager, Settings *settings, QWidget *parent) : QTabWidget(parent), settings_(settings)
{
	manager_ = manager;
	
	this->installEventFilter(this);
    setObjectName(QString::fromUtf8("src_tab_widget"));
	setTabsClosable(true);
	setMovable(true);
	setContentsMargins(0, 0, 0, 0);
	//setTabShape(QTabWidget::Triangular);

    tab_bar = tabBar();
    tab_bar->setContentsMargins(0, 0, 0, 0);
    
    /* Update current tab focus */
	QObject::connect(this, SIGNAL(currentChanged(int)), this, SLOT(set_focus_to_current_tab(int)));
    
    //this->setFocusPolicy(Qt::StrongFocus);
    tab_bar->setStyleSheet("border-width: 0px;");
    
    //setStyleSheet("border-width: 0px;");
    setStyleSheet("QTabWidget::pane {border-top: 0px;} QTabBar::tab { height: 25px; }");
    //setStyleSheet("QTabBar::tab { height: 25px; }");
    //setStyleSheet("QTabBar::tab { background-color: rgb(60, 60, 60); }");
}

/**
 * Destructor.
 */

src_container::~src_container()
{
	int count = this->count();
	
	for (int i = 0; i < count; i++)
		destroy_src_tab(i);
}

/**
 * Focus in event handler.
 */

void src_container::focusInEvent(QFocusEvent *event)
{
	debug(DEBUG, SRC_CONTAINER, "Tab widget has focus!!");
}

/**
 * Updates the current view. If the focus widget has changed the 
 * view_manager current view needs to be updated.
 */

void src_container::update_current_view()
{
	manager_->set_current_view(static_cast<view *>(this->parent()));
}

Settings* src_container::get_settings()
{
	return settings_;
}

/**
 * Create new source tab.
 * @param file_name -> the new file name.
 * @param file_id -> the file unique ID.
 * @return file index within the container, or -1 in case of error.
 */

int src_container::new_src_tab(const QString &file_name, unsigned int file_id)
{
	int index;
	src_file *src_tab;
	QString show_name;

	try {
		src_tab = new src_file(file_name, file_id, this, manager_->get_highlight_manager());
		index = addTab(src_tab, "");
		debug(INFO, SRC_CONTAINER, "New file created at index " << index);
	} catch(lgmx::exception &excp) {
		debug(ERR, SRC_CONTAINER, excp.get_message());
		return -1;
	}

    if (file_name.isEmpty())
        show_name = NEW_FILE_NAME;
    else
        show_name = src_tab->get_src_file_name();

    setTabText(index, QApplication::translate("main_window", show_name.toStdString().c_str(), 0, QApplication::UnicodeUTF8));
    
    // slot to add an asterisk to the end of file name in case of unsaved modifications
	QObject::connect(src_tab, SIGNAL(modificationChanged(bool)), this, SLOT(file_changed(bool)));

	return index;
}

/**
 * [throw] Create new source tab with a clone file.
 * @param base_file -> the file to be cloned.
 * @return address of the new file.
 */

src_file* src_container::new_clone_tab(src_file *base_file)
{
	int index;
	src_file *src_tab;
	QString show_name;

	if (!base_file) {
		lgmx::exception excp("At src_container::new_clone_tab: Invalid base file.");
		throw excp;
	}

	try {
		src_tab = new src_file(base_file, this);
		index = addTab(src_tab, "");
	} catch(lgmx::exception &excp) {
		lgmx::exception excp("At src_container::new_clone_tab: Unable to create file.");
		throw excp;
	}

    setTabText(index, QApplication::translate("main_window", base_file->get_src_file_name().toStdString().c_str(), 0, QApplication::UnicodeUTF8));
    
    // slot to add an asterisk to the end of file name in case of unsaved modifications
	QObject::connect(src_tab, SIGNAL(modificationChanged(bool)), this, SLOT(file_changed(bool)));
    
	return src_tab;
}

/**
 * This is just a wrapper, currentIndex() may be used directly.
 */

int src_container::get_current_tab_index()
{
	return currentIndex();
}

/**
 * Returns the file index within the container.
 * @param src_tab - file we want the index of.
 * @return file index, or -1 if the file is not found.
 */

int src_container::index_of(src_file *src_tab) const
{
	if (!src_tab)
		return -1;
		
	return indexOf(static_cast<QWidget *>(src_tab));
}

/**
 * Returns the source_file's index in the container.
 * @param src_tab - file we want the index of.
 */

int src_container::get_index_of(src_file *src_tab)
{
	return indexOf(static_cast<QWidget *>(src_tab));
}

/**
 * Returns the current source file.
 * @return pointer to the current source file, NULL pointer if there 
 * is no current file.
 */

src_file *src_container::get_current_src_file()
{
	return static_cast<src_file *>(currentWidget());
}

/**
 * Sets the current file on the container, i.e. the file to be shown.
 * @param id -> the file unique ID.
 */

void src_container::set_current_src_file(unsigned int id)
{
	src_file *file = get_src_file(id);
	
	if (file)
		setCurrentIndex(indexOf(file));
}

/**
 * 
 * @param id -> the file unique ID.
 */

void src_container::highlight_current_src_file()
{
	src_file *file = get_current_src_file();
	
	if (file)
		file->highlight_visible_blocks();
}

/**
 * Sets the next file as the current file.
 */

void src_container::set_next_src_file_as_current()
{
	int curr = currentIndex();
	
	if (curr < 0)
		return;
	
	if (++curr >= count())
		curr = 0;
		
	setCurrentIndex(curr);
}


/**
 * Returns the source file through its index.
 * @param index - source file index within the container.
 * @return pointer to the file, NULL pointer if the index is invalid.
 */

src_file *src_container::get_src_file(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return NULL;	/* index out of range */
		
	return src_tab;
}

/**
 * Returns the source file through its ID.
 * @param id - source file unique ID.
 * @return pointer to the file, NULL pointer if the ID is invalid.
 */

src_file *src_container::get_src_file(unsigned int id)
{
	src_file *file;
	int count = this->count();

    for (int i = 0; i < count; i++) {
		file = this->get_src_file(i);
		if (file->get_id() == id)
			return file;
	}

	return NULL;
}

/**
 * [throw] Returns the file unique ID.
 * @param index - file index within the container. If the index is invalid 
 * this function throws an exception.
 * @return file unique ID.
 */

unsigned int src_container::get_src_file_id(int index)
{
	src_file *src_tab = get_src_file(index);
	
	if (src_tab)
		return src_tab->get_id();

	lgmx::exception excp("At src_container::get_src_file_id: Invalid index.");
	throw excp;
}

/**
 * Removes the specified tab from the tab widget.
 * @param index -> tab index
 */

void src_container::destroy_src_tab(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return;	/* index out of range */
	
	removeTab(index);	// remove from container
	delete src_tab;	// destroy the file
	src_tab = NULL;

	debug(INFO, SRC_CONTAINER, "destroyed tab at index " << index);
}

/**
 * Removes the specified tab from the tab widget.
 * @param file -> pointer to file.
 */

void src_container::destroy_src_tab(src_file *file)
{
	int index = indexOf(file);
	
	if (index < 0) {
		debug(ERR, SRC_CONTAINER, "file not found!!");
		return;
	}

	removeTab(index);	// remove from container
	delete file;		// destroy the file
	file = NULL;

	debug(INFO, SRC_CONTAINER, "destroyed tab at index " << index);
}

/**
 * Removes the specified tab from the tab widget.
 * @param id - source file unique ID.
 */

void src_container::destroy_src_tab(unsigned int id)
{
	src_file *file = NULL;
	int count = this->count();
	
	for (int i = 0; i < count; i++) {
		file = this->get_src_file(i);
		
		if (file && file->get_id() == id) {
			removeTab(i);
			delete file;
			debug(INFO, SRC_CONTAINER, "destroyed tab at index " << i);
			break;
		}
	}
}

/**
 * Retrieves the complete file name, path + name.
 * @param index - file index.
 * @param file_name - string to hold the result.
 * @return true, if success, false otherwise.
 */

bool src_container::get_src_tab_full_name(int index, QString &file_name)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	src_tab->get_src_file_full_name(file_name);
	return true;
}

/**
 * Returns the name of the file (without the path).
 * @return file name without the path.
 */

QString src_container::get_src_tab_short_name(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return "";	/* index out of range */

	return src_tab->get_src_file_name();
}

/**
 * Returns the file path. It does not include the file name
 */

QString src_container::get_src_tab_path(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return "";	/* index out of range */

	return src_tab->get_src_file_path();
}

/**
 * Retrieves the content of the entire file.
 * @param index - file index.
 * @param content - string to hold the result.
 */

bool src_container::get_src_tab_content(int index, QString &content)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	content = src_tab->get_content();

	return true;
}

/**
 * Writes file content to disk.
 * @param fileName -> complete path of the file to be written.
 * @return true, if file written ok, false otherwise.
 */

bool src_container::src_tab_write_file(int index, const QString &fileName)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	return src_tab->write_file(fileName);
}

/**
 * Check if file content was modified.
 * @brief Check if file content was modified
 * @param index -> tab index
 * @return true -> file modified, false -> not modified or invalid index
 */

bool src_container::is_modified(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	return src_tab->is_modified();
}

/**
 * 
 */

bool src_container::set_modified(int index, bool modified)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	src_tab->set_modified(modified);
	return true;
}

bool src_container::set_file_name(int index, const QString &fileName)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	src_tab->set_src_file_name(fileName);
    
    return true;
}

/**
 * 
 */

//void src_container::setFont(QFont &font)
//{
    //this->setFont(font);
//}

/**
 * Updates the information from a file already saved on disk
 * @brief Updates the information from a file already saved on disk
 * @param index -> tab index
 * @return true -> info updated ok, false -> invalid index
 */

bool src_container::update_file_info(int index)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	src_tab->update_src_file_info();    /* refresh file info */
    
    /* set tab text with the short file name */
    setTabText(index, src_tab->get_src_file_name());
    
    return true;
}

bool src_container::get_curr_font(int index, QFont &font)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	font = src_tab->get_font();
	return true;
}

/**
 * Moves cursor to specific file line.
 * @param index -> file index within the container.
 * @param line -> line to go to.
 */

void src_container::go_to_line(int index, int line)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return;		/* index out of range */

	src_tab->go_to_line(line);
}

/**
 * Show or hide the source files tab bar.
 * @param show -> true, show tabs; false, hide tabs
 */

void src_container::show_tabs(bool show)
{
	if (show)
        tab_bar->show();
	else
        tab_bar->hide();
}

/**
 * Checks whether the tab bar is visible or not.
 * @return true, if tab bar is visible, false otherwise.
 */

bool src_container::tabs_visible()
{
	return !tab_bar->isHidden();
}

/**
 * Returns the file index within the container.
 * @param file_name - complete file name.
 * @return file index, or -1 if the file was not found.
 */

int src_container::get_file_index(const QString &file_name)
{
	int index;
    int count = this->count();
    QString file;
    
    for (index = 0; index < count; index++) {
        this->get_src_tab_full_name(index, file);
        if (file_name == file)
            return index;
    }
    
    return -1;
}

/**
 * Sets line wrap mode for the files within the container.
 * @param wrap -> true, line wraps according to the widget's width, 
 * false, no wrap.
 */
/*
void src_container::set_line_wrap(bool wrap)
{
	int c = this->count();
	
	for (int i = 0; i < c; i++) {
		if (wrap)
			static_cast<src_file *>(widget(i))->setLineWrapMode(QPlainTextEdit::WidgetWidth);
		else
			static_cast<src_file *>(widget(i))->setLineWrapMode(QPlainTextEdit::NoWrap);
	}
}*/

/**
 * Returns the line wrap mode for the files within the container.
 * @return true, if the line wrap is on, false otherwise.
 */

bool src_container::get_line_wrap() const
{
	if (count() > 0 && static_cast<src_file *>(widget(0))->lineWrapMode() == QPlainTextEdit::WidgetWidth)
		return true;

	return false;
}

/**
 * Sets the tab width for all files in this container.
 * @param size - number of spaces for the tab width.
 */

void src_container::set_tab_width(int size)
{
	src_file *src_tab;
	int c = count();
	
	for (int i = 0; i < c; i++) {
		if ((src_tab = static_cast<src_file *>(widget(i))) == 0)
			continue;	/* index out of range */
			
		src_tab->set_tab_width(size);
	}
}

int src_container::get_tab_width() const
{
	return 0;
}

/**
 * 
 */
 
void src_container::set_line_wrap(bool wrap)
{
	src_file *src_tab;
	int c = count();
	
	for (int i = 0; i < c; i++) {
		if ((src_tab = static_cast<src_file *>(widget(i))) == 0)
			continue;	/* index out of range */
			
		src_tab->set_line_wrap(wrap);
	}
}

/**
 * [slot] Adds asterisk to end of file name if there are any unsaved modifications.
 * This slot is triggered by src_file::modificationChanged(bool).
 * @param changed -> true, if there are unsaved modifications, false otherwise.
 */

void src_container::file_changed(bool changed)
{
	src_file *file = static_cast<src_file *>(sender());
	
	if (!file) {
		debug(ERR, SRC_CONTAINER, "Unable to find signal sender");
		return;
	}
	
	int index =	this->indexOf(file);
	QString file_name(file->get_src_file_name());
	
	if (file_name.isEmpty())
		file_name = NEW_FILE_NAME;

	if (changed)
		file_name += "*";
		
	setTabText(index, QApplication::translate("main_window", file_name.toStdString().c_str(), 0, QApplication::UnicodeUTF8));
}

/**
 * [slot] Updates the current tab focus. Whenever the current tab changes 
 * the focus is set to it.
 */

void src_container::set_focus_to_current_tab(int index)
{
	QWidget *curr =	widget(index);
	
	if (curr)
		curr->setFocus();
}

