#include <QVBoxLayout>
#include <QFont>

#include <view.h>
#include <view_manager.h>
#include <src_file.h>
#include <src_container.h>
#include <visual_src_file.h>
#include <debug.h>
#include <exception.h>
#include <status_line.h>

/**
 * Constructor.
 */

view::view(view_manager *manager, QWidget *parent) : QWidget(parent)
{
	manager_ = manager;		// view manager
	id_ = manager_->generate_view_id();	// ask manager for ID

	src_container_ = new src_container(manager_, manager->get_settings(), this);
	status_line_ = new status_line(src_container_);
	main_layout_ = new QVBoxLayout(this);

	status_line_->set_src_container(src_container_);

	main_layout_->addWidget(src_container_);
	main_layout_->addWidget(status_line_);
	main_layout_->setContentsMargins(0, 0, 0, 0);
	main_layout_->setSpacing(0);

	setLayout(main_layout_);
	setMinimumSize(5, 5);

	/* close file signal */
	QObject::connect(src_container_, SIGNAL(tabCloseRequested(int)), manager, SLOT(close_file(int)));

	/* update status line signal */
	QObject::connect(src_container_, SIGNAL(currentChanged(int)), status_line_, SLOT(update_file_name(int)));

	this->setFocusPolicy(Qt::StrongFocus);
	debug(DEBUG, VIEW, "New view created - ID: " << id_);
}

/**
 * Destructor.
 */

view::~view()
{
	debug(DEBUG, VIEW, "~view()");
	manager_->release_view_id(id_);	// release my ID

	delete status_line_;
	delete src_container_;
	delete main_layout_;
}

/**
 * Clones a source file. Creates a new file structure using a reference to
 * the original file content, therefore the changes in the clone file are
 * reflected in the original file as well as in the other clone files.
 * @param file - the file to be cloned.
 */

void view::new_visual_file(src_file *base_file)
{
	visual_src_file *new_file;

	try {
		new_file = src_container_->new_visual_src_file(base_file);
	} catch (lgmx::exception &excp) {
		debug(ERR, VIEW, excp.get_message());
		return;
	}

	new_file->setTextCursor(base_file->textCursor());
	new_file->set_modified(base_file->is_modified());
	new_file->highlight();

	status_line_->add_file(base_file->get_src_file_full_name(), base_file->get_id());
}

/**
 * Clones a source container. Creates a clone file for each file from
 * the original container.
 * @param base_src_ctr -> the source container to be cloned.
 */

void view::clone_src_container(src_container *base_src_ctr)
{
	visual_src_file *file;
	int count = base_src_ctr->count();

	for (int i = 0; i < count; i++) {
		file = base_src_ctr->get_src_file(i);

		if (file) {
			this->new_visual_file(file->get_root_file());
		}
	}

	this->src_container_->setCurrentIndex(base_src_ctr->currentIndex());
}

/*
void view::focusInEvent(QFocusEvent *event)
{
	cout << "this view has focus!!" << endl;
	//parent_->set_current_widget(this);
}
*/

/*
void view::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		cout << "clicked!!" << endl;
	} else {
		// pass on other buttons to base class
		QWidget::mousePressEvent(event);
	}
}*/

/**
 * Destroys a source file.
 * @param id - source file unique ID.
 */

void view::destroy_src_file(unsigned int id)
{
	debug(DEBUG, VIEW, "Destroying file ID - " << id);
	src_container_->destroy_src_tab(id);
	status_line_->remove_file(id);
}

/**
 * Returns the view unique ID.
 */

unsigned int view::get_id() const
{
	return id_;
}

void view::set_font(QFont &font)
{
	src_container_->setFont(font);
}

/**
 * Returns the address of the source container from this view.
 */

src_container* view::get_src_container() const
{
	return src_container_;
}

/**
 * Returns the view manager's address.
 */

view_manager* view::get_view_manager() const
{
	return manager_;
}

/**
 * Show or hide the source files tab bar.
 * @param show -> true, show tabs; false, hide tabs
 */

void view::show_src_tab_bar(bool show)
{
	src_container_->show_tabs(show);
}

/**
 * Checks whether the tab bar is visible or not.
 * @return true, if tab bar is visible, false otherwise.
 */

bool view::src_tab_bar_visible()
{
	return src_container_->tabs_visible();
}

/**
 * Show or hide the status bar.
 * @param show -> true, show status bar; false, hide status bar
 */

void view::show_status_bar(bool show)
{
	if (show)
		status_line_->show();
	else
		status_line_->hide();
}

/**
 * Checks whether the status bar is visible or not.
 * @return true, if status bar is visible, false otherwise.
 */

bool view::status_bar_visible()
{
	return !status_line_->isHidden();
}

/**
 * Updates the status bar file name for the specified file ID.
 * @param fileName - the new file name.
 * @param id - the unique file ID.
 */

void view::update_status_bar(const QString &fileName, unsigned int id)
{
	status_line_->update_file_name(fileName, id);
}


