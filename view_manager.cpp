#include "view_manager.h"
#include <iostream>
#include <algorithm>
#include <debug.h>

#include <QDir>
#include <QFileDialog>

using namespace std;

/**
 * Constructor.
 */

view_manager::view_manager(QWidget *parent) : QWidget(parent)
{
	root_container_ = new src_container(this);
	root_container_->hide();
	
	view *new_view = new view(this, this);

	current_view_ = new_view;
	m_num_splits = 0;

	layout_ = new QVBoxLayout(this);
	layout_->addWidget(new_view);
	layout_->setContentsMargins (0, 0, 0, 0);
	layout_->setSpacing(0);
    setLayout(layout_);
}

/**
 * Destructor.
 */

view_manager::~view_manager()
{
	delete layout_;
	delete root_view_;
}

/**
 * Adds view to list. Whenever a view is created it must be added to 
 * this list.
 */

void view_manager::add_to_view_list(view *v)
{
	view_list_.insert(view_list_.end(), v);
	cout << "added to view list size: " << view_list_.size() << endl;
}

/**
 * Removes view from list. Whenever a view is destroyed it must be pulled out from 
 * this list.
 */

void view_manager::remove_from_view_list(view *v)
{
	view_list_.remove(v);
	cout << "removed from view list size: " << view_list_.size() << endl;
}

/**
 * Adds splitter to list. Whenever a splitter is created it must be added to 
 * this list.
 */

void view_manager::add_to_splitter_list(QSplitter *s)
{
	if (s)
		view_splitters_.insert(view_splitters_.end(), s);
}

/**
 * Removes splitter from list. Whenever a splitter is destroyed it must be pulled out from 
 * this list.
 */

void view_manager::remove_from_splitter_list(QSplitter *s)
{
	if (s)
		view_splitters_.remove(s);
}

/**
 * Creates new empty file. A file is always created in the root view, which 
 * in turn propagates the file creation to other views, if they exist.
 */

void view_manager::new_file()
{
	int index;

	index = root_container_->new_src_tab("", file_id_.generate_id());

	if (index < 0) {
		return;
	}
	
	src_file *file = root_container_->get_src_file(index);
	
	list<view *>::iterator it;
	
	for (it = view_list_.begin(); it != view_list_.end(); it++) {
		(*it)->clone_file(file, 0);
	}
}

bool view_manager::new_file(const QString &file_name)
{
	int index;

	index = root_container_->new_src_tab(file_name, file_id_.generate_id());

	if (index < 0) {
		return false;
	}
	
	src_file *file = root_container_->get_src_file(index);
	
	list<view *>::iterator it;
	
	for (it = view_list_.begin(); it != view_list_.end(); it++) {
		(*it)->clone_file(file, 0);
	}
	
	return true;
}

/**
 * Slot to handle file close request.
 */

void view_manager::close_file(int index)
{
	int index_r;
	/*
	 * The file to be closed is identified by its QTextDocument address.
	 * When a file is created the root file is created, if there are other views, 
	 * the file is cloned in all other views. The clone files do not have their 
	 * own content (QTextDocument *), they all point to the root file's content instead.
	 * Therefore, if the close action was requested from a child view, we can identify 
	 * the corresponding root file to be closed by the address of QTextDocument, which is 
	 * the same in the root file and in their clones.
	 */
	
	src_container *container = static_cast<src_container *>(sender());

	index_r = this->get_root_src_container_file_index(container->get_src_file(index)->get_mutable_content());
	
	if (index_r < 0) {
		cout << "file not found in root source container" << endl;
		return;
	}
	
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(root_container_->widget(index))) == 0)
		return;	/* index out of range */
	
	cout << "Found file in the root source container!!" << endl;
	
	QString file_name(src_tab->get_src_file_full_name());	// get file name
	
	root_view_->destroy_src_file(src_tab->get_id());
	
	cout << "destroyed!!!" << endl;
	
	set<QString>::iterator it = open_files_.find(file_name);    /* pull out from open files list */
    if (it != open_files_.end())
        open_files_.erase(*it);
	
	//root_container_->destroy_src_tab(index_r);
	// move Ui_MainWindow::close_file to here
}

/**
 * Returns the index in the root source container of the file to be closed.
 * @param content -> pointer to file content.
 * @return file index within the root source container, -1 if not found.
 */

int view_manager::get_root_src_container_file_index(QTextDocument *content)
{
	if (!content)
		return -1;
	
	int count = root_container_->count();
	
	src_file *src_tab;

	for (int i = 0; i < count; i++) {
		if ((src_tab = static_cast<src_file *>(root_container_->widget(i))) == 0)
			continue;
		
		// search for the same content address
		if (content == src_tab->get_mutable_content()) {
			cout << "requested close: " << src_tab->get_src_file_full_name().toStdString() << endl;
			
			return i;
		}
	}
	
	return -1;
}

/**
 * Creates an open file dialog and opens the selected files.
 */

void view_manager::open_file()
{
	QString path;
    QDir dir;
	int index, size;
    src_container* ctr;
    
    ctr = get_current_src_container();
    if (!ctr) {
		debug(ERR, VIEW_MANAGER, "Invalid view");
		return;
    }
    
    index = ctr->get_current_tab_index();  /* get current file index */
    
    /* 
     * "open file" dialog path is the path of the current open file, or "home"
     * if there is no file open
     */
    if (index < 0 || (path = get_current_src_container()->get_src_tab_path(index)) == "")
        path = dir.homePath();
 
	// list of files to be opened
    QStringList files(QFileDialog::getOpenFileNames(this, tr("Open File"), path, tr("All files (*.c *.cpp *.h)")));

    size = files.size();
    for (index = 0; index < size; ++index) {
		open_file(files.at(index));
    }
}

/**
 * Opens a file. Creates a new file and loads the content specified by 
 * file_name.
 * @param file_name -> the complete path of the file to be opened.
 */

void view_manager::open_file(const QString &file_name)
{
	// checks whether this file is already open
	if (open_files_.find(file_name) == open_files_.end()) {
		//if (root_view_->new_file(file_name, file_id_.generate_id()) < 0)
		if (!new_file(file_name))
			return;

		open_files_.insert(file_name);
		//f_watcher.add_path(file_name);
		recent_files_->add_file(file_name);
	} else {
		// file already open, only set it as current file
		current_view_ = get_current_view();

		int index = current_view_->get_src_container()->get_file_index(file_name);
		if (index >= 0)
			set_current_file_index(index);
	}
}

/**
 * 
 */

void view_manager::reload_current_file()
{
	src_file *file = this->get_current_src_file();
	
	if (!file)
		return;

	// check for modifications and ask for confirmation
	// check if file exists on the disk

	file->load_file(file->get_src_file_full_name());
}

/**
 * Save file to disk [slot].
 */

bool view_manager::save()
{
    QString file_name;
    int index;
    bool result;
    
    src_container *curr_src_c = get_current_src_container();

    if (!curr_src_c)
		return false;
    
    index = curr_src_c->get_current_tab_index();  /* get current file index */
    
    if (index < 0)      /* no file open */
        return false;
    
    curr_src_c->get_src_tab_full_name(index, file_name);
    
    if (file_name.isEmpty()) {
        cout << "file name empty" << endl;
        result = save_file_as(curr_src_c, index);
    } else
		result = save_file(curr_src_c, file_name, index);
	
	if (result)
		curr_src_c->set_modified(index, false);
		
	return result;
}

/**
 * Save file as. Saves a file which has not been written to the 
 * disk yet.
 * @param index - file index in the tab widget
 */

bool view_manager::save_file_as(src_container *src_c, int index)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    // mudar para abrir na home do user se o arquivo nao existe:
    //files = QFileDialog::getSaveFileName(this, tr("Save File"), path, tr("All files (*.c *.cpp *.h)"));
    
    if (fileName.isEmpty())
        return false;   /* no file specified */

    if (save_file(src_c, fileName, index)) {
        src_c->set_file_name(index, fileName);
        //open_files_.insert(fileName);
		//recent_files_->add_file(fileName);
        //f_watcher.add_path(fileName);
        return true;
    }
    
    return false;
}

/**
 * Saves file to disk.
 * @param fileName -> complete file path.
 * @param index -> index in the source tab
 * @return true -> file saved successfully, false -> error
 */

bool view_manager::save_file(src_container *src_c, const QString &fileName, int index)
{
    QString error;
    
    cout << "file name -> " << fileName.toStdString() << endl;

    if (!src_c->src_tab_write_file(index, fileName))
        return false;

    open_files_.insert(fileName);
    recent_files_->add_file(fileName);

    return true;
}

/**
 * 
 */

void view_manager::set_current_file_index(int index)
{
	current_view_ = get_current_view();

	if (!current_view_)
		return;
	
	src_container *container = current_view_->get_src_container();
	
    if (index >= container->count())
        return; /* index out of range */
    
    container->setCurrentIndex(index);
}

/**
 * 
 */

int view_manager::get_current_file_index(const QString &file_name)
{
	current_view_ = get_current_view();

	if (!current_view_)
		return -1;
	
	return current_view_->get_src_container()->get_file_index(file_name);
}

/**
 * Returns the current source file.
 * @return pointer to the current source file, NULL pointer if there 
 * is no current file.
 */

src_file* view_manager::get_current_src_file() const
{
	src_container *curr_ctr = this->get_current_src_container();
	
	if (!curr_ctr)
		return NULL;

	return curr_ctr->get_current_src_file();
}

unsigned int view_manager::generate_view_id()
{
	return view_id_.generate_id();	
}

void view_manager::release_view_id(unsigned int id)
{
	view_id_.release_id(id);
}

/**
 * Returns the root view.
 * @return Address of the root view.
 */

view* view_manager::get_root_view() const
{
	return root_view_;
}

/**
 * 
 */

view* view_manager::get_current_view() const
{
	if (m_num_splits == 0)
		return *(view_list_.begin());
	
	list<view *>::const_iterator it;
	int i;
	QWidget *curr_widget = 0;

	for (i = 0, it = view_list_.begin(); it != view_list_.end(); it++, i++) {
		curr_widget = (*it)->focusWidget();
		if (curr_widget && curr_widget->hasFocus()) {
			cout << "get_current_view " << i << endl;
			return *it;
		}
	}
	
	return *(view_list_.begin());
}

/**
 * Returns the root source container.
 * @return Address of the root source container.
 */

src_container* view_manager::get_root_src_container() const
{
	return root_container_;
}

/**
 * Returns the current source container.
 * @return Address of the current source container, or NULL in case
 * there is no source container.
 */

src_container* view_manager::get_current_src_container() const
{
	view *current = get_current_view();
	
	if (current)
		return current_view_->get_src_container();

	return 0;
}

/**
 * Set the current view pointer. The current view pointer should always 
 * reference a valid view.
 * @param curr_view -> new current view.
 */

void view_manager::set_current_view(view* curr_view)
{
	if (!curr_view) {
		debug(ERR, VIEW_MANAGER, "Invalid view");
		return;
	}

	current_view_ = curr_view;
}

/**
 * Splits current view horizontally. 
 */

void view_manager::split_horizontally()
{
	this->split(Qt::Vertical);
}

/**
 * Splits current view vertically.
 */

void view_manager::split_vertically()
{	
	this->split(Qt::Horizontal);
}

/**
 * 
 */

void view_manager::split(Qt::Orientation orientation)
{
	QSplitter *new_splitter;
	view *new_view;
	int size;
	view *curr_view = get_current_view();
	
	if (!curr_view) {
		debug(ERR, VIEW_MANAGER, "No current view");
		return;
	}
	
	if (m_num_splits == 0) {
		new_splitter = new QSplitter(orientation, this);
		new_splitter->setHandleWidth(1);
		new_splitter->setChildrenCollapsible(false);
		new_splitter->setProperty("minisplitter", true);
		
		// create new view
		new_view = new view(this, new_splitter);
		new_view->clone_src_container(curr_view->get_src_container(), 0);
		
		if (orientation == Qt::Vertical)
			size = curr_view->height() / 2;
		else
			size = curr_view->width() / 2;
		
		new_splitter->addWidget(curr_view);
		new_splitter->addWidget(new_view);
		layout_->addWidget(new_splitter);

		QList<int> sizes;
		sizes.push_back(size);
		sizes.push_back(size);
		new_splitter->setSizes(sizes);
			
		view_splitters_.push_back(new_splitter);
		
	} else {
		int index = -1;

		if (orientation == Qt::Vertical) {
			size = curr_view->height() / 2;
		} else {
			size = curr_view->width() / 2;
		}

		QList<int> sizes;
		sizes.push_back(size);
		sizes.push_back(size);

		// get parent splitter
		QSplitter *parent = static_cast<QSplitter*>(curr_view->parentWidget());
		
		// get current view index in the splitter
		index = parent->indexOf(curr_view);
		
		// create new splitter
		new_splitter = new QSplitter(orientation, parent);
		new_splitter->setHandleWidth(1);
		new_splitter->setChildrenCollapsible(false);
		new_splitter->setProperty("minisplitter", true);
		
		// create new view
		new_view = new view(this, new_splitter);
		new_view->clone_src_container(curr_view->get_src_container(), 0);
		
		parent->insertWidget(index, new_splitter);
		
		new_splitter->addWidget(curr_view);
		new_splitter->addWidget(new_view);
		new_splitter->setSizes(sizes);
		
		debug(INFO, VIEW_MANAGER, "splitter count -> " << parent->count());
		debug(INFO, VIEW_MANAGER, "new splitter count -> " << new_splitter->count());
		
		view_splitters_.push_back(new_splitter);
	}
	
	m_num_splits++;
}

/**
 * Removes current split.
 */

void view_manager::unsplit()
{
	if (m_num_splits == 0)
		return;

	view *curr_view = get_current_view();

	if (!curr_view) {
		debug(ERR, VIEW_MANAGER, "No current view");
		return;
	}

	QSplitter *grand_parent;
	list<QSplitter *>::iterator s_it;
	list<view *>::iterator v_it;

	// get parent splitter
	QSplitter *parent = static_cast<QSplitter*>(curr_view->parentWidget());
	
	if (parent->parentWidget() == this) {	// grandparent is view_manager
		// view index in the parent splitter
		int view_index = parent->indexOf(curr_view);

		if (view_index == 0)
			layout_->addWidget(parent->widget(1));
		else
			layout_->addWidget(parent->widget(0));
	
		remove_from_splitter_list(parent);
		
		delete curr_view;
		delete parent;

	} else if ((grand_parent = dynamic_cast<QSplitter*>(parent->parentWidget()))) {
		
		debug(DEBUG, VIEW_MANAGER, "grand parent is QSplitter");
		
		// parent index in the grandparent splitter
		int index = grand_parent->indexOf(parent);
		
		// view index in the parent splitter
		int view_index = parent->indexOf(curr_view);

		if (view_index == 0)
			grand_parent->insertWidget(index, parent->widget(1));
		else
			grand_parent->insertWidget(index, parent->widget(0));
		
		remove_from_splitter_list(parent);

		// destroy current
		debug(DEBUG, VIEW_MANAGER, "splitter count: " << parent->count());
		
		delete curr_view;
		delete parent;
	} else {
		debug(CRIT, VIEW_MANAGER, "Unknown parent splitter");
		return;
	}
	
	m_num_splits--;
}

/**
 * 
 */

void view_manager::set_recent_files_widget(recent_files *recent_files_widget)
{
	recent_files_ = recent_files_widget;
}



void view_manager::show_src_tab_bar(bool show)
{
	list<view *>::iterator it;
	
	for (it = view_list_.begin(); it != view_list_.end(); it++)
		(*it)->show_src_tab_bar(show);
}









