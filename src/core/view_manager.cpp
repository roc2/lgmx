#include <QVBoxLayout>
#include <QSplitter>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <view_manager.h>
#include <view.h>
#include <src_container.h>
#include <src_file.h>
#include <visual_src_file.h>
#include <highlight_manager.h>
#include <exception.h>
#include <global.h>
#include <debug.h>


/**
 * Constructor.
 */

view_manager::view_manager(QWidget *parent, file_type *type_manager) : QWidget(parent), root_container_(*this), f_watcher_(this)
{
	type_manager_ = type_manager;

	curr_view_ = create_view(this);
	num_splits_ = 0;
	tag_ = NULL;

	highlight_manager_ = new highlight_manager(type_manager_);

	layout_ = new QVBoxLayout(this);
	layout_->addWidget(curr_view_);
	layout_->setContentsMargins (0, 0, 0, 0);
	layout_->setSpacing(0);
	setLayout(layout_);
}

/**
 * Destructor.
 */

view_manager::~view_manager()
{
	file_settings_.save();
	
	delete layout_;
	delete highlight_manager_;
	clear_view_list();
	clear_splitter_list();
	delete tag_;
}

/**
 * Creates a new view.
 * @param parent - the parent widget.
 * @return pointer to the new view.
 */

/**
 * @todo make operator new private in the class view, and this function
 * friend of view class. The same with delete.
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
 * Destroys all view splitters.
 */

void view_manager::clear_splitter_list()
{
	std::list<QSplitter *>::iterator it;

	// It is possible that a splitter is the parent of another splitter, and
	// a splitter automatically deletes its children on destruction. Therefore
	// we need to reparent all splitters before destroying them, in order to
	// avoid double deletion.

	for (it = view_splitters_.begin(); it != view_splitters_.end(); it++)
		(*it)->setParent(NULL);

	for (it = view_splitters_.begin(); it != view_splitters_.end(); it++)
		delete *it;

	view_splitters_.clear();
}

/**
 * Adds view to list. Whenever a view is created it must be added to
 * this list.
 * @param v - the address of the view to be added.
 */

void view_manager::add_to_view_list(view *v)
{
	if (v)
		view_list_.insert(view_list_.end(), v);

	debug(DEBUG, VIEW_MANAGER, "view list size: " << view_list_.size());
}

/**
 * Removes view from list. Whenever a view is destroyed it must be pulled out from
 * this list.
 * @param v - the address of the view to be removed.
 */

void view_manager::remove_from_view_list(view *v)
{
	if (v)
		view_list_.remove(v);

	debug(DEBUG, VIEW_MANAGER, "view list size: " << view_list_.size());
}

/**
 * Adds splitter to list. Whenever a splitter is created it must be added to
 * this list.
 * @param s - the address of the splitter to be added.
 */

void view_manager::add_to_splitter_list(QSplitter *s)
{
	if (s)
		view_splitters_.insert(view_splitters_.end(), s);
}

/**
 * Removes splitter from list. Whenever a splitter is destroyed it must be pulled out from
 * this list.
 * @param s - the address of the splitter to be removed.
 */

void view_manager::remove_from_splitter_list(QSplitter *s)
{
	if (s)
		view_splitters_.remove(s);
}

/**
 * Creates a new empty file. A file is always created in the root view, which
 * in turn propagates the file creation to other views, if they exist.
 */

void view_manager::new_file()
{
	unsigned int id = file_id_.generate_id();	// new file unique ID

	if (!root_container_.new_file("", id)) {
		file_id_.release_id(id);
		debug(ERR, VIEW_MANAGER, "Could not create a new file");
		return;
	}

	debug(DEBUG, VIEW_MANAGER, "New file ID: " << id);
	src_file *file = root_container_.get_file(id);

	if (!file) {
		debug(ERR, VIEW_MANAGER, "File not found");
		return;
	}

	std::list<view *>::iterator it;
	view *curr_view = get_current_view();

	// clone the new file in all views
	for (it = view_list_.begin(); it != view_list_.end(); it++) {
		//(*it)->clone_file(file);
		(*it)->new_visual_file(file);

		if (*it == curr_view) {
			curr_view->get_src_container()->set_current_src_file(id);
			//curr_view->get_src_container()->highlight_current_src_file();
		}
	}
}

/**
 * Creates a new file in memory from a file saved on the hard disk.
 * @param file_name - complete file name.
 * @return true, if file created successfully, false otherwise.
 */

bool view_manager::new_file(const QString &file_name)
{
	unsigned int id = file_id_.generate_id();

	if (!root_container_.new_file(file_name, id)) {
		file_id_.release_id(id);
		debug(ERR, VIEW_MANAGER, "Could not create a new file");
		return false;
	}

	debug(DEBUG, VIEW_MANAGER, "New file created - ID: " << id);
	src_file *file = root_container_.get_file(id);

	std::list<view *>::iterator it;
	view *curr_view = get_current_view();

	for (it = view_list_.begin(); it != view_list_.end(); it++) {
		(*it)->new_visual_file(file);

		if (*it == curr_view) {
			curr_view->get_src_container()->set_current_src_file(id);
			//curr_view->get_src_container()->highlight_current_src_file();
		}
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
	visual_src_file *src_tab;

	// get container
	src_container *container = static_cast<src_container *>(sender());

	// get sender file
	src_tab = container->get_src_file(index);
	if (!src_tab) {
		debug(ERR, VIEW_MANAGER, "File not found");
		return;
	}

	close_file(container, src_tab, index);
}

/**
 * [Slot] Closes current file.
 */

void view_manager::close_file()
{
	src_container *container = get_current_src_container();
	visual_src_file *src_tab = container->get_current_src_file();

	if (!src_tab) {
		debug(ERR, VIEW_MANAGER, "File not found");
		return;
	}

	close_file(container, src_tab, container->get_current_tab_index());
}

/**
 * Destroys a file removing it from all views.
 * @param container - container in which the action started.
 * @param src_tab - file to be closed.
 * @param index - file index within the container.
 */

void view_manager::close_file(src_container *container, visual_src_file *src_tab, int index)
{
	/*
	 * The file to be closed is identified by its unique ID.
	 * The original file is the root file, which is not shown to the user,
	 * what the user sees are visual_src_files, which are visual representations
	 * of the root file. There is one visual_src_file for each view.
	 * The visual_src_file does not have its own content (QTextDocument *), they
	 * all point to the root file's content instead.
	 * Therefore, if the close action was requested from a child view, we can identify
	 * the corresponding root file to be closed by its ID, which is
	 * the same in the root file and its visual representations.
	 */

	unsigned int id;
	QString file_name(src_tab->get_src_file_full_name());
	id = src_tab->get_id();
	bool exists = src_tab->exists();

	// check if file needs to be saved
	if (src_tab->is_modified()) {
		QString msg;

		// build close file message
		if (!exists) {
			msg = tr("The file 'untitled' has been modified.");
		} else {
			msg = tr("The file '") + src_tab->get_file_name() +
				  tr("' has been modified.");
		}

		container->setCurrentIndex(index);

		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Close"));
		msgBox.setText(msg);
		msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		msgBox.setIcon(QMessageBox::Question);
		int ret = msgBox.exec();

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

	// remove the file from all views
	std::list<view *>::iterator v_it;
	for (v_it = view_list_.begin(); v_it != view_list_.end(); v_it++)
		(*v_it)->destroy_src_file(id);

	// get the root file
	src_file *file = root_container_.get_file(id);
	if (!file) {
		debug(CRIT, VIEW_MANAGER, "File not found in root container");
		return;
	}

	if (file->exists()) {
		// canonical name (no symbolic links, "." or "..")
		QDir path(file_name);
		QString can_name(path.canonicalPath());

		f_watcher_.remove_file(can_name);	// remove from file watcher

		// remove the file from open files list
		std::set<QString>::iterator it(open_files_.find(can_name));
		if (it != open_files_.end())
			open_files_.erase(*it);
	}

	// remove the file from root container
	root_container_.destroy_file(id);
	file_id_.release_id(id);
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
	index = ctr->get_current_tab_index();  /* get current file index */

	/*
	 * "open file" dialog path is the path of the current open file, or "home"
	 * if there is no file open
	 */
	if (index < 0 || (path = ctr->get_src_tab_path(index)) == "")
		path = dir.homePath();

	// list of files to be opened
	QFileDialog dialog(this);
	QStringList files(dialog.getOpenFileNames(this, tr("Open File"), path));

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

bool view_manager::open_file(const QString &file_name)
{
	// canonical name (no symbolic links, "." or "..")
	QDir path(file_name);
	QString can_name(path.canonicalPath());

	if (can_name.isEmpty()) {	// file does not exist
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Open"));
		msgBox.setText(tr("Cannot read file ") + file_name);
		msgBox.setInformativeText(tr("File does not exist."));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();

		return false;
	}

	debug(DEBUG, VIEW_MANAGER, can_name.toStdString());

	// checks whether this file is already open
	if (open_files_.find(can_name) == open_files_.end()) {

		if (!new_file(can_name))
			return false;

		open_files_.insert(can_name);
		recent_files_->add_file(can_name);
		f_watcher_.add_file(can_name);
	} else {
		// file already open, only set it as current file
		int index = get_current_src_container()->get_file_index(can_name);
		if (index >= 0)
			set_current_file_index(index);
		else
			return false;
	}

	return true;
}

/**
 * Reloads current file from disk.
 */

void view_manager::reload_current_file()
{
	src_container *container = get_current_src_container();
	visual_src_file *file = container->get_current_src_file();

	if (!file || !file->exists())
		return;

	int index = container->index_of(file);
	if (index < 0) {
		debug(ERR, VIEW_MANAGER, "File not found");
		return;
	}

	QString file_name(file->get_src_file_full_name());

	// check if file needs to be saved
	if (file->is_modified()) {
		// display message box
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Reload"));
		msgBox.setText(tr("The file '") + file->get_file_name() + tr("' has been modified."));
		msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.addButton(tr("Discard Changes"), QMessageBox::ActionRole);
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		msgBox.setIcon(QMessageBox::Question);
		int ret = msgBox.exec();

		if (ret == QMessageBox::Save) { // save file
			if (!save_file(container, file_name, index))
				return; // could not save, just return

        } else if (ret == QMessageBox::Cancel)
			return;     // if dialog is canceled, do nothing
	}

	int pos = file->get_cursor_position();
	file->load_file(file_name);
	// restore cursor position
	QTextCursor cursor(file->get_cursor());
	cursor.setPosition(pos);
	file->set_cursor(cursor);
	file->centerCursor();
	debug(DEBUG, VIEW_MANAGER, "File reloaded");
}

/**
 * [slot] Save file to disk.
 */

bool view_manager::save()
{
	QString file_name;
	int index;
	bool result;

	src_container *curr_src_c = get_current_src_container();

	index = curr_src_c->get_current_tab_index();  /* get current file index */

	if (index < 0)      /* no file open */
		return false;

	curr_src_c->get_src_tab_full_name(index, file_name);

	if (file_name.isEmpty()) {
		result = save_file_as(curr_src_c, index);
	} else if (curr_src_c->is_modified(index)) {
		result = save_file(curr_src_c, file_name, index);
	} else {
		debug(INFO, VIEW_MANAGER, "File has no modifications");
		return false;
	}

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
    QDir dir;
    QString path(dir.homePath());

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), path);

    if (fileName.isEmpty())
        return false;   /* no file specified */

    if (save_file(src_c, fileName, index)) {
		unsigned int id;

		try {
			id = src_c->get_src_file_id(index);
		} catch (lgmx::exception &excp) {
			debug(ERR, VIEW_MANAGER, excp.get_message());
		}

		// canonical name (no symbolic links, "." or "..")
		dir.setPath(fileName);
		QString can_name(dir.canonicalPath());

		open_files_.insert(can_name);
		recent_files_->add_file(fileName);
		f_watcher_.add_file(can_name);
        src_c->set_file_name(index, fileName);
        update_status_bar(fileName, id);

        // set file type
        visual_src_file *file = src_c->get_src_file(index);

		if (file) {
			file_type::type ft;
			ft = type_manager_->get_file_type(file->get_src_file_extension());
			file->set_file_type(ft);
			debug(DEBUG, VIEW_MANAGER, "File type: " << file_type::to_string(ft).toStdString());
		} else {
			debug(ERR, VIEW_MANAGER, "File not found " << fileName.toStdString());
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

    debug(INFO, VIEW_MANAGER, "File saved: " << fileName.toStdString());
    // warns the file watcher the modification is internal
    f_watcher_.notify_internal_modification(fileName);

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
	QString msg;
	src_container *curr = get_current_src_container();

	tabs = curr->count();

	/* check for unsaved files and save them if requested */
	for (index = 0; index < tabs; index++) {

		visual_src_file *src_tab = curr->get_src_file(index);
		if (!src_tab) {
			debug(ERR, VIEW_MANAGER, "File not found");
			continue;
		}

		if (src_tab->is_modified()) {
			curr->setCurrentIndex(index);
			bool exists = src_tab->exists();

			// build close file message
			if (!exists) {
				msg = tr("The file 'untitled' has been modified.");
			} else {
				msg = tr("The file '") + src_tab->get_file_name() +
					  tr("' has been modified.");
			}

			QMessageBox msgBox(this);
			msgBox.setWindowTitle(tr("Quit"));
			msgBox.setText(msg);
			msgBox.setInformativeText("Do you want to save your changes?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			msgBox.setIcon(QMessageBox::Question);
			int ret = msgBox.exec();

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
 * Returns a pointer to the highlight manager.
 */

highlight_manager* view_manager::get_highlight_manager()
{
	return highlight_manager_;
}

/**
 * Returns a pointer to the settings object.
 */

file_settings* view_manager::get_settings()
{
	return &file_settings_;
}

file_type& view_manager::get_type_manager()
{
	return *type_manager_;
}

/**
 * Sets the file specified by index as the current file.
 * @param index - file index in the container.
 */

void view_manager::set_current_file_index(int index)
{
	src_container *container = get_current_src_container();

	if (index >= container->count())
		return; /* index out of range */

	container->setCurrentIndex(index);
}

/**
 * [slot] Sets the next file as the current file.
 */

void view_manager::set_next_file_as_current()
{
	get_current_src_container()->set_next_src_file_as_current();
}

tag * view_manager::get_tags()
{
	if (!tag_) {
		tag_ = new tag(this);
		debug(DEBUG, VIEW_MANAGER, "Created tags");
	}

	return tag_;
}

/**
 * [slot] Jumps to the tag under cursor.
 */

void view_manager::go_to_tag()
{
	if (!tag_) {
		tag_ = new tag(this);
		debug(DEBUG, VIEW_MANAGER, "Created tags");
	}

	visual_src_file *file = get_current_src_file();

	if (!file) {
		debug(INFO, VIEW_MANAGER, "No current file");
		return;
	}

	QString tag_name(file->get_word_under_cursor());

	if (tag_name.isEmpty())
		return;

	debug(INFO, VIEW_MANAGER, tag_name.toStdString());

	tag_->find_tag(tag_name);
}

/**
 * [slot] Jumps to specific location. If the specified file is not open
 * the application will try to open it.
 * @param file_name - file to jump to.
 * @param line - line to jump to.
 */

void view_manager::jump_to(const QString &file_name, int line)
{
	if (!open_file(file_name))
		return;

	visual_src_file* file = get_current_src_file();
	if (file)
		file->go_to_line(line);
}

/**
 * Returns the file index in the container.
 * @param file_name - complete file name.
 * @return file index, or -1 if the file is not found.
 */

int view_manager::get_current_file_index(const QString &file_name)
{
	return get_current_src_container()->get_file_index(file_name);
}

/**
 * Returns the current source file.
 * @return pointer to the current source file, NULL pointer if there
 * is no current file.
 */

visual_src_file* view_manager::get_current_src_file() const
{
	return get_current_src_container()->get_current_src_file();
}

/**
 * Sets the tab width for all files.
 * @param size - number of spaces for the tab width.
 */

void view_manager::set_tab_width(int size)
{
	std::list<view *>::iterator it(view_list_.begin());

	for (; it != view_list_.end(); it++)
		(*it)->get_src_container()->set_tab_width(size);

	//settings_->set_tab_width(size);
}

/**
 * @todo implement this method.
 */

int view_manager::get_tab_width() const
{
	return 0;
}

/**
 * Sets the line wrap mode.
 * @param wrap - true, line wrap on, false for line wrap disabled.
 */

void view_manager::set_line_wrap(bool wrap)
{
	std::list<view *>::iterator it(view_list_.begin());

	for (; it != view_list_.end(); it++)
		(*it)->get_src_container()->set_line_wrap(wrap);

	//settings_->set_line_wrap(wrap);
}

/**
 * @todo implement this method.
 */

bool view_manager::get_line_wrap() const
{
	return false;
}

/**
 * Returns the editor version number.
 * @return version number string.
 */

QString view_manager::get_version_number()
{
	return STR_VER_NUM;
}

/**
 * Creates a unique view ID.
 * @return new unique ID.
 */

unsigned int view_manager::generate_view_id()
{
	return view_id_.generate_id();
}

/**
 * Releases a no longer used view ID. Every destroyed view must
 * release its ID.
 * @param id - The ID to be released.
 */

void view_manager::release_view_id(unsigned int id)
{
	view_id_.release_id(id);
}

/**
 * Returns the current view. This method always returns a valid view.
 * @return Address of the current view.
 */

view* view_manager::get_current_view() const
{
	if (num_splits_ == 0)
		return *(view_list_.begin());

	if (curr_view_) {
		debug(DEBUG, VIEW_MANAGER, "QPointer Ok!!");
		return curr_view_.data();
	} else {
		debug(WARNING, VIEW_MANAGER, "Invalid QPointer!");
	}

	std::list<view *>::const_iterator it;
	int i;
	QWidget *curr_widget = 0;

	for (i = 0, it = view_list_.begin(); it != view_list_.end(); it++, i++) {
		curr_widget = (*it)->focusWidget();
		if (curr_widget && curr_widget->hasFocus()) {
			debug(DEBUG, VIEW_MANAGER, "get_current_view - number: " << i);
			return *it;
		}
	}

	return *(view_list_.begin());
}

/**
 * Returns the current source container. This method always returns a valid
 * source container.
 * @return Address of the current source container.
 */

src_container* view_manager::get_current_src_container() const
{
	return get_current_view()->get_src_container();
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

	curr_view_ = curr_view;
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
 * Splits the screen creating a new view.
 * @param orientation - split Qt::orientation. Vertical (up/down) or
 * horizontal (left/right).
 */

void view_manager::split(Qt::Orientation orientation)
{
	QSplitter *new_splitter;
	view *new_view;
	int size;
	view *curr_view = get_current_view();

	if (num_splits_ == 0) {
		new_splitter = new QSplitter(orientation, this);
		new_splitter->setHandleWidth(1);
		new_splitter->setChildrenCollapsible(false);
		new_splitter->setProperty("minisplitter", true);

		// create new view
		new_view = create_view(new_splitter);
		new_view->clone_src_container(curr_view->get_src_container());
		set_view_properties(*curr_view, *new_view);

		if (orientation == Qt::Vertical)
			size = curr_view->height() >> 1;
		else
			size = curr_view->width() >> 1;

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
		if (orientation == Qt::Vertical)
			size = curr_view->height() >> 1;
		else
			size = curr_view->width() >> 1;

		sizes.push_back(size);
		sizes.push_back(size);

		// get parent splitter sizes
		QWidget *widget = parent->widget(index == 1 ? 0 : 1);

		if (parent->orientation() == Qt::Vertical) {
			if (index == 1) {
				p_split_sizes.push_back(widget->height());
				p_split_sizes.push_back(curr_view->height());
			} else {
				p_split_sizes.push_back(curr_view->height());
				p_split_sizes.push_back(widget->height());
			}
		} else {
			if (index == 1) {
				p_split_sizes.push_back(widget->width());
				p_split_sizes.push_back(curr_view->width());
			} else {
				p_split_sizes.push_back(curr_view->width());
				p_split_sizes.push_back(widget->width());
			}
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

		view_splitters_.push_back(new_splitter);
	}

	num_splits_++;
}

/**
 * Removes current split.
 */

void view_manager::unsplit()
{
	if (num_splits_ == 0)
		return;

	view *curr_view = get_current_view();

	QSplitter *grand_parent;
	std::list<QSplitter *>::iterator s_it;
	std::list<view *>::iterator v_it;

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
		// parent index in the grandparent splitter
		int index = grand_parent->indexOf(parent);

		// view index in the parent splitter
		int view_index = parent->indexOf(curr_view);

		if (view_index == 0)
			grand_parent->insertWidget(index, parent->widget(1));
		else
			grand_parent->insertWidget(index, parent->widget(0));

		set_current_view(static_cast<view *>(grand_parent->widget(index)));
		remove_from_splitter_list(parent);

		debug(DEBUG, VIEW_MANAGER, "splitter count: " << parent->count());

		destroy_view(curr_view);
		delete parent;
	} else {
		debug(CRIT, VIEW_MANAGER, "Unknown parent widget");
		return;
	}

	num_splits_--;
}

/**
 * Removes all splits keeping only the current view.
 */

void view_manager::remove_all_splits()
{
	if (num_splits_ == 0)
		return;

	view *curr_view = get_current_view();
	curr_view->setParent(this);
	layout_->addWidget(curr_view);

	std::list<view *>::iterator it(view_list_.begin());

	for (; it != view_list_.end();) {
		if (*it != curr_view) {
			(*it)->setParent(NULL);	// take the view from splitter
			delete *it;
			it = view_list_.erase(it);
		} else {
			it++;
		}
	}

	clear_splitter_list();	// removes all splitters
	num_splits_ = 0;
}

/**
 * Copies all properties from one view to the other.
 * @param old_view - the view where the properties are copied from.
 * @param new_view -
 */

void view_manager::set_view_properties(view &old_view, view &new_view)
{
	new_view.show_src_tab_bar(old_view.src_tab_bar_visible());
	new_view.show_status_bar(old_view.status_bar_visible());
}

/**
 * Sets recent files widget pointer.
 */

void view_manager::set_recent_files_widget(recent_files *recent_files_widget)
{
	recent_files_ = recent_files_widget;
}

/**
 * Shows the source files tabs bar.
 * @param show: true, show tab bar, or false, hide tab bar.
 */

void view_manager::show_src_tab_bar(bool show)
{
	std::list<view *>::iterator it;

	for (it = view_list_.begin(); it != view_list_.end(); it++)
		(*it)->show_src_tab_bar(show);
}

/**
 * Shows status bar.
 * @param show: true, show status bar, or false, hide status bar.
 */

void view_manager::show_status_bar(bool show)
{
	std::list<view *>::iterator it;

	for (it = view_list_.begin(); it != view_list_.end(); it++)
		(*it)->show_status_bar(show);
}

/**
 * Updates the status bar for all views.
 */

void view_manager::update_status_bar(const QString &fileName, unsigned int id)
{
	std::list<view *>::iterator v_it;

	// update status bar in all views
	for (v_it = view_list_.begin(); v_it != view_list_.end(); v_it++)
		(*v_it)->update_status_bar(fileName, id);
}


