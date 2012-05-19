#include "view_manager.h"
#include <iostream>
#include <debug.h>

using namespace std;

/**
 * Constructor.
 */

view_manager::view_manager(QWidget *parent) : QWidget(parent)
{
	root_view_ = new view(this, 0, true);
	current_view_ = root_view_;
	
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
 * Add view to list.
 */

void view_manager::add_to_view_list(view *v)
{
	view_list_.insert(view_list_.end(), v);
	cout << "added to view list size: " << view_list_.size() << endl;
}

/**
 * Remove view from list.
 */

void view_manager::remove_from_view_list(view *v)
{
	view_list_.remove(v);
	cout << "removed from view list size: " << view_list_.size() << endl;
}

/**
 * Create new empty file.
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

src_container* view_manager::get_root_src_container()
{
	return root_view_->get_src_container();
}

view* view_manager::get_root_view() const
{
	return root_view_;
}

/**
 * 
 */

src_container* view_manager::get_current_src_container()
{
	view *current = get_current_view();
	
	if (current)
		return current_view_->get_src_container();

	return 0;
}

/**
 * 
 */

view* view_manager::get_current_view() const
{
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
 * Split current view horizontally. 
 */

void view_manager::split_horizontally()
{
	view *current = get_current_view();
	
	if (current)
		current->split(Qt::Vertical);
	else
		cout << "no current" << endl;
}

/**
 * Split current view vertically.
 */

void view_manager::split_vertically()
{
	view *current = get_current_view();
		
	if (current)
		current->split(Qt::Horizontal);
	else
		cout << "no current" << endl;
}

/**
 * Remove current split.
 */

void view_manager::unsplit()
{
	view *current = get_current_view();
	
	if (current) {
		if (current->is_root())
			return;
	
		current_view_ = current->get_parent_view();
		current_view_->unsplit(current);
	}
}

















