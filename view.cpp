#include "view.h"
#include "view_manager.h"
#include "stdlib.h"
#include "debug.h"

ctr_wrapper::ctr_wrapper(view *parent)
{
	this->src_container_ = new src_container(parent);
	status_line_ = new status_line();
	main_layout_ = new QVBoxLayout();
	
	status_line_->set_src_container(src_container_);
	
	main_layout_->addWidget(src_container_);
	main_layout_->addWidget(status_line_);
	main_layout_->setContentsMargins(0, 0, 0, 0);
	
	setLayout(main_layout_);
}


view::view(view_manager *manager, view *parent, bool root) : QWidget(parent)
{
	splitter_ = nullptr;
    child_view_[0] = nullptr;
    child_view_[1] = nullptr;
    
    manager_ = manager;		// view manager
    parent_ = parent;		// parent view
    
    wrapper_ = new ctr_wrapper(this);
    //src_container_ = new src_container(this);	// current src_container
	src_container_ = wrapper_->src_container_;
	status_line_ = wrapper_->status_line_;

    root_ = root;
    if (root)
		cout << "root view - " << this << endl;
    
    splitted_ = false;
    
    // set src_container as current layout
    layout_ = new QStackedLayout();
    //layout_->insertWidget(0, src_container_);
    layout_->insertWidget(0, wrapper_);
    layout_->setCurrentIndex(0);
    layout_->setContentsMargins(0, 0, 0, 0);
    
    main_layout_ = new QVBoxLayout(this);
    main_layout_->addLayout(layout_);
    main_layout_->setContentsMargins(0, 0, 0, 0);
    setLayout(main_layout_);
    
	/* close file signal */
	QObject::connect(src_container_, SIGNAL(tabCloseRequested(int)), manager, SLOT(close_file(int)));
 
	/* update status line signal */
	QObject::connect(src_container_, SIGNAL(currentChanged(int)), status_line_, SLOT(update_file_name(int)));
 
    this->setFocusPolicy(Qt::StrongFocus);
    //this->setContentsMargins(0, 0, 0, 0);
    
    manager_->add_to_view_list(this);
}

/**
 * Copy costructor.
 */

view::view(const view &copy) : QWidget(copy.get_parent_view())
{
	splitter_ = nullptr;
    child_view_[0] = nullptr;
    child_view_[1] = nullptr;
    
    manager_ = copy.get_view_manager();		// view manager
	parent_ = copy.get_parent_view();		// parent view

	src_container_ = new src_container(this);	// current src_container
	clone_src_container(copy.get_src_container());

	root_ = copy.is_root();
	splitted_ = copy.is_splitted();
	
	/* close file signal */
	QObject::connect(src_container_, SIGNAL(tabCloseRequested(int)), manager_, SLOT(close_file(int)));
 
    this->setFocusPolicy(Qt::StrongFocus);
    manager_->add_to_view_list(this);
}

/**
 * Destructor.
 */

view::~view()
{
	cout << "~view()" << endl;
	debug(DEBUG, VIEW, "");
	manager_->remove_from_view_list(this);

	if (child_view_[0])
		delete child_view_[0];
	if (child_view_[1])
		delete child_view_[1];
	if (splitter_)
		delete splitter_;
	if (src_container_)
		delete src_container_;

	delete layout_;
	delete main_layout_;
}

/**
 * 
 */

int view::new_file(const QString &file_name)
{
	int index;
	
	index = src_container_->new_src_tab(file_name);
	
	if (index < 0)
		return index;
	
	if (splitted_) {
		debug(DEBUG, VIEW, "clone files");
		src_file *file = src_container_->get_src_file(index);
		child_view_[0]->clone_file(file);
		child_view_[1]->clone_file(file);
	}
	
	// destroy current src container
	
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

void view::clone_file(src_file *file)
{
	if (splitted_) {
		child_view_[0]->clone_file(file);
		child_view_[1]->clone_file(file);
	} else {
		int index = src_container_->new_clone_tab(file);
		
		if (index >= 0) {
			src_file *new_file;
			new_file = src_container_->get_src_file(index);
			
			if (new_file) {				
				new_file->setTextCursor(file->textCursor());
				new_file->set_modified(file->is_modified());
			}
		}
	}
}

/**
 * @param base_src_ctr -> the source container to be cloned.
 */

void view::clone_src_container(src_container *base_src_ctr)
{
	src_file *file = nullptr;
	int count = base_src_ctr->count();
	
	for (int i = 0; i < count; i++) {
		file = base_src_ctr->get_src_file(i);
		
		if (file) {
			this->clone_file(file);
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

/**
 * Check whether the view is splitted or not.
 * @return true, if splitted, false otherwise
 */

bool view::is_splitted() const
{
	return splitted_;
}

/**
 * 
 */

bool view::is_root() const
{
	return root_;
}

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
 * Split current window in two new windows, either horizontally or 
 * vertically.
 * @param orientation -> split direction, vertical or horizontal.
 */

void view::split(Qt::Orientation orientation)
{
	if (splitted_)
		return;
		
	splitter_ = new QSplitter(orientation, this);
	splitter_->setHandleWidth(1);
	splitter_->setChildrenCollapsible(false);
	splitter_->setProperty("minisplitter", true);

	child_view_[0] = new view(manager_, this);
	child_view_[1] = new view(manager_, this);
	
	//cout << "new child_view_[0] - " << child_view_[0] << endl;
	//cout << "new child_view_[1] - " << child_view_[1] << endl;
	debug(DEBUG, VIEW, "new child_view_[0] - " << child_view_[0]);
	debug(DEBUG, VIEW, "new child_view_[1] - " << child_view_[1]);
	
	
	splitter_->addWidget(child_view_[0]);
	splitter_->addWidget(child_view_[1]);
	
	splitted_ = true;

	child_view_[0]->clone_src_container(this->src_container_);
	child_view_[1]->clone_src_container(this->src_container_);

	layout_->insertWidget(1, splitter_);
	layout_->setCurrentIndex(1);
	
    if (!root_) {
		//delete this->src_container_;
		//this->src_container_ = 0;
	}
}

/**
 * Remove current view.
 */

void view::unsplit(view *to_be_destroyed)
{
	//cout << "I am - " << this << endl;
	debug(DEBUG, VIEW, "I am - " << this);
	
	if (!child_view_[0] || !child_view_[1]) {
		//cout << "no children" << endl;
		debug(DEBUG, VIEW, "no children");
		
		debug(DEBUG, VIEW, "child_view_[0] " << child_view_[0]);
		debug(DEBUG, VIEW, "child_view_[1] " << child_view_[1]);
		//cout << "child_view_[0] " << child_view_[0] << endl;
		//cout << "child_view_[1] " << child_view_[1] << endl;
		return;
	}
	
	if (to_be_destroyed->is_splitted()) {
		debug(CRIT, VIEW, "Tried to destroy a view that has child views!!");
		return;
	}
	
	int keep, destroy;
	Qt::Orientation orientation;
	QList<int> sizes;
	
	if (child_view_[0]->is_splitted() || child_view_[1]->is_splitted()) {
		
		cout << "splitted child" << endl;
		if (to_be_destroyed == child_view_[0]) {
			destroy = 0;
			keep = 1;
			orientation = child_view_[1]->get_splitter()->orientation();
			sizes = child_view_[1]->get_splitter()->sizes();
		} else {
			destroy = 1;
			keep = 0;
			orientation = child_view_[0]->get_splitter()->orientation();
			sizes = child_view_[0]->get_splitter()->sizes();
		}
		
		/*
		 * delete splitter chama os destrutores das views, portanto o melhor a fazer 
		 * é destruir as views que estão no splitter, assim o splitter remove elas 
		 * automaticamente, e então depois adicionar as novas views.
		 */
		
		delete child_view_[destroy];
		child_view_[destroy] = child_view_[keep]->take_child_view(0, this);	// check if child view exists
		
		view *aux = child_view_[keep];
		child_view_[keep] = child_view_[keep]->take_child_view(1, this);	// check if child view exists
		delete aux;
				
		//splitter_->setOrientation(orientation);

		if (child_view_[0]) {
			cout << "is root : " << child_view_[0]->is_root() << endl;
			cout << "child_view_[0] set - " << child_view_[0] << endl;
			splitter_->addWidget(child_view_[0]);
		}

		if (child_view_[1]) {
			cout << "is root : " << child_view_[1]->is_root() << endl;
			cout << "child_view_[1] set - " << child_view_[1] << endl;
			splitter_->addWidget(child_view_[1]);
		}

		splitter_->setOrientation(orientation);
		splitter_->setSizes(sizes);
		layout_->setCurrentIndex(1);
	} else {
		/* destroy both child views */
		layout_->setCurrentIndex(0);
		layout_->takeAt(1);
		
		delete child_view_[0];
		delete child_view_[1];
		delete splitter_;
		
		child_view_[0] = child_view_[1] = nullptr;
		splitter_ = nullptr;
		
		splitted_ = false;
	}
}

/**
 * 
 */

view* view::take_child_view(int index, view *new_parent)
{
	if (index < 0 || index > 1)
		return 0;
	
	view *ret = child_view_[index];
	ret->set_parent(new_parent);
	child_view_[index] = 0;

	return ret;
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

QSplitter* view::get_splitter()
{
	return splitter_;
}

view* view::get_parent_view() const
{
	return parent_;
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
	
	if (child_view_[0])
		child_view_[0]->show_src_tab_bar(show);
	if (child_view_[1])
		child_view_[1]->show_src_tab_bar(show);
}
	
/**
 * 
 */

void view::set_parent(view *parent)
{
	parent_ = parent;		// parent view
	this->setParent(parent);
}	
	
	
	
	
	
