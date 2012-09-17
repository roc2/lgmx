#include "view.h"
#include "view_manager.h"
#include "stdlib.h"
#include "debug.h"


view::view(view_manager *manager, QWidget *parent) : QWidget(parent)
{
    manager_ = manager;		// view manager
    parent_ = static_cast<view *>(parent);		// parent view
    
    id_ = manager_->generate_view_id();	// ask manager for ID

	src_container_ = new src_container(this);
	status_line_ = new status_line();
	main_layout_ = new QVBoxLayout(this);
	
	status_line_->set_src_container(src_container_);
	
	main_layout_->addWidget(src_container_);
	main_layout_->addWidget(status_line_);
	main_layout_->setContentsMargins(0, 0, 0, 0);
	
	setLayout(main_layout_);
    
	/* close file signal */
	QObject::connect(src_container_, SIGNAL(tabCloseRequested(int)), manager, SLOT(close_file(int)));
 
	/* update status line signal */
	QObject::connect(src_container_, SIGNAL(currentChanged(int)), status_line_, SLOT(update_file_name(int)));
 
    this->setFocusPolicy(Qt::StrongFocus);
    
    manager_->add_to_view_list(this);
}

/**
 * Destructor.
 */

view::~view()
{
	debug(DEBUG, VIEW, "~view()");
	manager_->remove_from_view_list(this);
	manager_->release_view_id(id_);	// release my ID

	delete status_line_;
	delete src_container_;
	delete main_layout_;
}

/**
 * 
 */

int view::new_file(const QString &file_name, unsigned int file_id)
{
	int index;
	
	index = src_container_->new_src_tab(file_name, file_id);
	
	return index;
}

/**
 * 
 */
/*
int view::close_file(int index)
{
	int index;
	
	index = src_container_->new_src_tab(file_name);
	
	if (index < 0)
		return index;
	
	if (splitted_) {
		cout << "clone files" << endl;
		src_file *file = src_container_->get_src_file(index);
		view_a_->clone_file(file);
		view_b_->clone_file(file);
	}
	
	// destroy current src container
	
	return index;
}
*/

/**
 * Clone a source file. Creates a new file structure using a reference to 
 * the original file content, therefore the changes in the clone file are
 * reflected in the original file as well as in the other clone files.
 */

void view::clone_file(src_file *file, int index)
{
	src_file *new_file;
	
	try {
		new_file = src_container_->new_clone_tab(file);
	} catch (...) {
		return;
	}
	
	new_file->setTextCursor(file->textCursor());
	new_file->set_modified(file->is_modified());
	file->set_child_src_file(new_file, index);
}

/**
 * @param base_src_ctr -> the source container to be cloned.
 * @param index -> index of the view that owns this source container.
 */

void view::clone_src_container(src_container *base_src_ctr, int index)
{
	src_file *file = nullptr;
	int count = base_src_ctr->count();
	
	for (int i = 0; i < count; i++) {
		file = base_src_ctr->get_src_file(i);
		
		if (file) {
			this->clone_file(file, index);
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
 * 
 */

void view::destroy_src_file(unsigned int id)
{	
	debug(DEBUG, VIEW, "Destroying file - " << this);
	src_container_->destroy_src_tab(id);
}

/**
 * 
 */

unsigned int view::get_id() const
{
	return id_;
}

/**
 * 
 */

void view::set_src_container(src_container *container)
{
	this->src_container_ = container;
}

/**
 * 
 */

src_container* view::get_src_container() const
{
	return src_container_;
}

view_manager* view::get_view_manager() const
{
	return manager_;
}

QVBoxLayout* view::get_main_layout()
{
	return main_layout_;
}

/**
 * 
 */
	
void view::show_src_tab_bar(bool show)
{
	src_container_->show_tabs(show);
}
	
	
	
	
	
	
