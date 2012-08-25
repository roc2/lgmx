#include "status_line.h"
#include <QPalette>

status_line::status_line()
{
	 layout_ = new QHBoxLayout;
     file_name_ = new QLabel(NO_NAME, this);
     
     layout_->addWidget(file_name_);
     layout_->setContentsMargins(0, 0, 0, 0);
     
     this->setLayout(layout_);
     set_default_colors();
}

status_line::~status_line()
{
	delete file_name_;
	delete layout_;
}

/**
 * 
 */

void status_line::set_file_name(const QString &file_name)
{
	file_name_->setText(file_name);
}

/**
 * [slot] Updates the current file name.
 * @param index -> index of the current file, -1 if there are 
 * no files.
 */

void status_line::update_file_name(int index)
{
	if (index < 0)
		set_file_name(NO_NAME);
	else {
		QString file_name;
		src_container_->get_src_tab_full_name(index, file_name);
		set_file_name(file_name);
	}
}

void status_line::set_src_container(src_container *container)
{
	src_container_ = container;
}

/**
 * Sets status line's default colors.
 */

void status_line::set_default_colors()
{
	QPalette p;
	QColor c(0, 0, 0);
	p.setColor(QPalette::WindowText, c);
	file_name_->setPalette(p);
	//QColor c2(0, 0, 255);
	//p.setColor(QPalette::Window, c2);
	//file_name_->setPalette(p);
    file_name_->setStyleSheet("background: blue");
}













