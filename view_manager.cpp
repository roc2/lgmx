#include <QVBoxLayout>
#include <QSplitter>
#include "view_manager.h"
#include <iostream>
#include <algorithm>
#include <debug.h>

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <highlight_manager.h>

using namespace std;

/**
 * Constructor.
 */

view_manager::view_manager(QWidget *parent, file_type *type_manager) : QWidget(parent)
{
	root_container_ = new src_container(this);
	root_container_->hide();
	
	type_manager_ = type_manager;

	current_view_ = create_view(this);
	m_num_splits = 0;

	highlight_manager_ = new highlight_manager(type_manager_);

	layout_ = new QVBoxLayout(this);
	layout_->addWidget(current_view_);
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
	delete highlight_manager_;
	clear_view_list();
	delete root_container_;
}

/**
 * Creates a new view.
 * @param parent - the parent widget.
 * @return pointer to the new view.
 */

view* view_manager::create_view(QWidget *parent)
{
	view *new_view = new view(this, parent);
	add_to_view_list(new_view);
	
	return new_view;
}

/**
 * Destroys the view.
 * @param v - the address of the view to be destroyed.
 */

void view_manager::destroy_view(view* v)
{
	remove_from_view_list(v);
	delete v;
}

/**
 * Destroys all views. This method should be called only when the application 
 * finishes.
 */

void view_manager::clear_view_list()
{
	std::list<view *>::iterator it(view_list_.begin());

	for (; it != view_list_.end(); it++)
		delete *it;
		
	view_list_.clear();
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
	unsigned int id = file_id_.generate_id();

	index = root_container_->new_src_tab("", id);

	if (index < 0) {
		return;
	}
	
	src_file *file = root_container_->get_src_file(index);

	list<view *>::iterator it;
	view *curr_view = get_current_view();
	
	for (it = view_list_.begin(); it != view_list_.end(); it++) {
		(*it)->clone_file(file);
		
		if (*it == curr_view)
			curr_view->get_src_container()->set_current_src_file(id);
	}
}

bool view_manager::new_file(const QString &file_name)
{
	int index;
	unsigned int id = file_id_.generate_id();

	index = root_container_->new_src_tab(file_name, id);

	if (index < 0) {
		return false;
	}

	src_file *file = root_container_->get_src_file(index);

	// set file type
	file_type::type ft;
	ft = type_manager_->get_file_type(file->get_src_file_extension());
	debug(DEBUG, VIEW_MANAGER, "File type: " << ft);
	
	list<view *>::iterator it;
	view *curr_view = get_current_view();
	
	for (it = view_list_.begin(); it != view_list_.end(); it++) {
		(*it)->clone_file(file);
		
		if (*it == curr_view)
			curr_view->get_src_container()->set_current_src_file(id);
	}
	
	//plugin_manager_.load_plugins(ft);
	
	return true;
}

/**
 * [Slot] Closes a file.
 * @param index - file index in the container.
 */

void view_manager::close_file(int index)
{
	/*
	 * The file to be closed is identified by its ID.
	 * The original file is the root file, if there are other views, 
	 * the root file is cloned in all other views. The clone files do not have their 
	 * own content (QTextDocument *), they all point to the root file's content instead.
	 * Therefore, if the close action was requested from a child view, we can identify 
	 * the corresponding root file to be closed by the address of QTextDocument, which is 
	 * the same in the root file and in their clones.
	 */
	
	unsigned int id;
	src_file *src_tab;

	// get container
	src_container *container = static_cast<src_container *>(sender());

	// get sender file
	src_tab = container->get_src_file(index);
	if (!src_tab) {
		debug(ERR, VIEW_MANAGER, "File not found");
		return;
	}
	
	QString file_name(src_tab->get_src_file_full_name());
	id = src_tab->get_id();

	// check if file needs to be saved
	if (src_tab->is_modified()) {
		QString msg;
		bool exists = src_tab->exists();

		// build close file message
		if (!exists) {
			msg = tr("The file 'untitled' has been modified.\nDo you want to save your changes?");
		} else {
			msg = tr("The file '") + src_tab->get_src_file_name() + 
				  tr("' has been modified.\nDo you want to save your changes?");
		}
		
		container->setCurrentIndex(index);

        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "lgmx", msg,
			  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	
		if (ret == QMessageBox::Save) { /* save file */
            
            if (!exists) {
                if (save_file_as(container, index))
					src_tab->get_src_file_full_name(file_name);	// saved ok, update file name
                else
                    return; /* could not save, just return */
            } else {
                if (!save_file(container, file_name, index))
                    return; /* could not save, just return */
            }
            
        } else if (ret == QMessageBox::Cancel)
			return;     /* if dialog is canceled, do nothing */
	}

	list<view *>::iterator v_it;
	
	// remove the file from all views
	for (v_it = view_list_.begin(); v_it != view_list_.end(); v_it++) {
		src_tab = (*v_it)->get_src_container()->get_src_file(id);
		
		if (src_tab)
			delete src_tab;
	}

	// remove the file from root container
	src_tab = root_container_->get_src_file(id);
	if (!src_tab) {
		debug(ERR, VIEW_MANAGER, "File not found in root container");
		return;
	}

	delete src_tab;
	
	// remove the file from open files list
	set<QString>::iterator it(open_files_.find(file_name));
    if (it != open_files_.end())
        open_files_.erase(*it);
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

		if (!new_file(file_name))
			return;

		open_files_.insert(file_name);
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
 * @param src_c - the container that holds the file.
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
        
        // set file type
        src_file *file = src_c->get_src_file(index);
		
		if (file) {
			file_type::type ft;
			ft = type_manager_->get_file_type(file->get_src_file_extension());
			file->set_file_type(ft);
			debug(DEBUG, VIEW_MANAGER, "File type: " << ft);
		}

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
    if (!src_c->src_tab_write_file(index, fileName))
        return false;

    open_files_.insert(fileName);
    recent_files_->add_file(fileName);
    debug(INFO, VIEW_MANAGER, "File saved: " << fileName.toStdString());

    return true;
}

/**
 * Checks if there are unsaved files before closing the application. The 
 * files are either saved or discarded, according to the user's request.
 * @brief Checks if there are unsaved files before closing the application.
 * @return true -> files saved or discarded, Ok to close the application. 
 * false -> The close operation was canceled.
 */

bool view_manager::check_unsaved_files()
{
	int index, tabs;
    QMessageBox::StandardButton ret;
    QString msg;
    src_container *curr = get_current_src_container();
    
    tabs = curr->count();
    
    /* check for unsaved files and save them if requested */
    for (index = 0; index < tabs; index++) {

		src_file *src_tab = curr->get_src_file(index);
		if (!src_tab) {
			debug(ERR, VIEW_MANAGER, "File not found");
			continue;
		}

        if (src_tab->is_modified()) {
            curr->setCurrentIndex(index);
            bool exists = src_tab->exists();

            // build close file message
			if (!exists) {
				msg = tr("The file 'untitled' has been modified.\nDo you want to save your changes?");
			} else {
				msg = tr("The file '") + src_tab->get_src_file_name() + 
					  tr("' has been modified.\nDo you want to save your changes?");
			}
            
            ret = QMessageBox::warning(this, "lgmx", msg,
			      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            
            if (ret == QMessageBox::Save) { /* save file */
            
				if (!exists) {
                    if (!save_file_as(curr, index))
                        return false; /* could not save, just return */
                    
                } else {
					QString file_name(src_tab->get_src_file_full_name());
					
                    if (!save_file(curr, file_name, index))
                        return false; /* could not save, just return */
                    
                }
            } else if (ret == QMessageBox::Cancel)
                return false;     /* if dialog is canceled, do nothing */
        }
    }
    
    return true;
}

/**
 * 
 */

highlight_manager* view_manager::get_highlight_manager()
{
	return highlight_manager_;
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
 * @todo - review this method
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
 * Returns the current source container. This method always return a valid 
 * source container.
 * @return Address of the current source container.
 */

src_container* view_manager::get_current_src_container() const
{
	return get_current_view()->get_src_container();
	/*
	view *current = get_current_view();
	
	if (current)
		return current_view_->get_src_container();

	return NULL;*/
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
 * Splits current view horizontally (up/down).
 */

void view_manager::split_horizontally()
{
	this->split(Qt::Vertical);
}

/**
 * Splits current view vertically (left/right).
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
		//new_view = new view(this, new_splitter);
		new_view = create_view(new_splitter);
		new_view->clone_src_container(curr_view->get_src_container());
		set_view_properties(*curr_view, *new_view);
		
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
		QList<int> sizes, p_split_sizes;

		// get parent splitter
		QSplitter *parent = static_cast<QSplitter*>(curr_view->parentWidget());
		
		// get current view index in the splitter
		index = parent->indexOf(curr_view);

		// get new splitter sizes
		if (orientation == Qt::Vertical) {
			size = curr_view->height() / 2;
		} else {
			size = curr_view->width() / 2;
		}

		sizes.push_back(size);
		sizes.push_back(size);

		// get parent splitter sizes
		QWidget *widget = parent->widget(index == 1 ? 0 : 1);

		if (parent->orientation() == Qt::Vertical) {
			p_split_sizes.push_back(widget->height());
			p_split_sizes.push_back(curr_view->height());
		} else {
			p_split_sizes.push_back(widget->width());
			p_split_sizes.push_back(curr_view->width());
		}

		// create new splitter
		new_splitter = new QSplitter(orientation, parent);
		new_splitter->setHandleWidth(1);
		new_splitter->setChildrenCollapsible(false);
		new_splitter->setProperty("minisplitter", true);
		
		// create new view
		new_view = create_view(new_splitter);
		new_view->clone_src_container(curr_view->get_src_container());
		set_view_properties(*curr_view, *new_view);
		
		// add new splitter to parent splitter
		parent->insertWidget(index, new_splitter);
		
		// add views to new splitter
		new_splitter->addWidget(curr_view);
		new_splitter->addWidget(new_view);

		// set splitter sizes
		new_splitter->setSizes(sizes);
		parent->setSizes(p_split_sizes);
		
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
		
		destroy_view(curr_view);
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
		
		destroy_view(curr_view);
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

void view_manager::set_view_properties(view &old_view, view &new_view)
{
	new_view.show_src_tab_bar(old_view.src_tab_bar_visible());
	new_view.show_status_bar(old_view.status_bar_visible());
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

/**
 * 
 */

void view_manager::show_status_bar(bool show)
{
	list<view *>::iterator it;
	
	for (it = view_list_.begin(); it != view_list_.end(); it++)
		(*it)->show_status_bar(show);
}







