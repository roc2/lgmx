#include "view_manager.h"
#include <iostream>
#include <debug.h>

#include <QDir>
#include <QFileDialog>

using namespace std;

/**
 * Constructor.
 */

view_manager::view_manager(QWidget *parent) : QWidget(parent)
{
	root_view_ = new view(this, 0, true);
	current_view_ = root_view_;
	
	num_splits_ = 0;
	layout_ = new QVBoxLayout(this);
	layout_->addWidget(root_view_);
	layout_->setContentsMargins (0, 0, 0, 0);
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
 * Creates new empty file. A file is always created in the root view, which 
 * in turn propagates the file creation to other views, if they exist.
 */

void view_manager::new_file()
{
	root_view_->new_file("");
}	

/**
 * Slot to handle file close request.
 */

void view_manager::close_file(int index)
{
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
	
	// get pointer to content
	//QTextDocument *content = container->get_src_file(index)->get_mutable_content();
	this->close_file(container->get_src_file(index)->get_mutable_content());
}

/**
 * Creates an open file dialog and opens the selected files.
 */

void view_manager::open_file()
{
	QString path;
    QDir dir;
	int index, size;
    
    index = get_current_src_container()->get_current_tab_index();  /* get current file index */
    
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
		if (root_view_->new_file(file_name) < 0)
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
 * Save file to disk [slot].
 */

bool view_manager::save()
{
    QString file_name;
    int index;
    
    src_container *curr_src_c = get_current_src_container();

    if (!curr_src_c)
		return false;
    
    index = curr_src_c->get_current_tab_index();  /* get current file index */
    
    if (index < 0)      /* no file open */
        return false;
    
    //file_name = _src_container.get_src_tab_full_name(index);
    curr_src_c->get_src_tab_full_name(index, file_name);
    
    if (file_name.isEmpty()) {
        cout << "file name empty" << endl;
        return save_file_as(curr_src_c, index);
    }
    
    return save_file(curr_src_c, file_name, index);
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
        src_c->update_file_info(index);
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

    src_c->set_modified(index, false);
    open_files_.insert(fileName);
    recent_files_->add_file(fileName);

    return true;
}

/**
 * 
 */

void view_manager::close_file(QTextDocument *content)
{
	if (!content)
		return;
	
	src_container *root_container = root_view_->get_src_container();
	int count = root_container->count();
	
	src_file *src_tab;

	for (int i = 0; i < count; i++) {
		if ((src_tab = static_cast<src_file *>(root_container->widget(i))) == 0)
			continue;
		
		// search for the same content address
		if (content == src_tab->get_mutable_content()) {
			cout << "requested close: " << src_tab->get_src_file_full_name().toStdString() << endl;
			break;
		}
	}
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
	if (num_splits_ == 0)
		return root_view_;
	
	list<view *>::const_iterator it;
	int i;
	QWidget *curr_widget = 0;

	for (i = 0, it = view_list_.begin(); it != view_list_.end(); it++, i++) {

		if ((*it)->is_splitted())
			continue;	// aqui quando a view é splitada ela deveria ser retirada dessa lista!!
			
		curr_widget = (*it)->focusWidget();
		if (curr_widget && curr_widget->hasFocus()) {
			cout << "get_current_view " << i << endl;
			return *it;
		}
	}
	
	return 0;
}

/**
 * Returns the root source container.
 * @return Address of the root source container.
 */

src_container* view_manager::get_root_src_container() const
{
	return root_view_->get_src_container();
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
	view *current = get_current_view();
	
	if (current) {
		current->split(Qt::Vertical);
		num_splits_++;
		debug(INFO, VIEW_MANAGER, "splits: " << num_splits_);
	}
}

/**
 * Splits current view vertically.
 */

void view_manager::split_vertically()
{
	view *current = get_current_view();
		
	if (current) {
		current->split(Qt::Horizontal);
		num_splits_++;
		debug(INFO, VIEW_MANAGER, "splits: " << num_splits_);
	}
}

/**
 * Removes current split.
 */

void view_manager::unsplit()
{
	view *current = get_current_view();
	
	if (current) {
		if (current->is_root())
			return;
	
		current_view_ = current->get_parent_view();
		current_view_->unsplit(current);
		num_splits_--;
		debug(INFO, VIEW_MANAGER, "splits: " << num_splits_);
	}
}

/**
 * 
 */

void view_manager::set_recent_files_widget(recent_files *recent_files_widget)
{
	recent_files_ = recent_files_widget;
}















