#include "status_line.h"
#include <src_container.h>
#include <QPalette>
#include <QComboBox>

#include <debug.h>
#include <exception.h>
#include <iostream>

using namespace std;

#define DEF_NAME	tr("Untitled")

status_line::status_line(src_container *container)
{
	src_container_ = container;
	
	layout_ = new QHBoxLayout;
	file_list_ = new QComboBox(this);
	file_list_->setStyleSheet("QComboBox {border: 0px solid gray;border-radius: 0px; color: black; background-color: gray} QComboBox:on { padding-top: 0px; padding-left: 0px;} QComboBox::drop-down {border-width: 0px;} QComboBox::down-arrow {image: url(noimg); border-width: 0px;}");

	connect(file_list_, SIGNAL(currentIndexChanged(int)), this, SLOT(current_file_changed(int)));

	layout_->addWidget(file_list_);
	layout_->setContentsMargins(0, 0, 0, 0);

	this->setLayout(layout_);
	//set_default_colors();
}

status_line::~status_line()
{
	delete file_list_;
	delete layout_;
}

/**
 * Adds a file to the status bar file list.
 * @param file_name - complete file name.
 * @param id - src file unique ID.
 */

void status_line::add_file(const QString &file_name, unsigned int id)
{
	int count = file_list_->count();
	bool def_name = file_name.isEmpty();
	int i;
	
	for (i = 0; i <= count; i++) {
		if (file_name <= file_list_->itemText(i))
			break;
	}
	
	file_list_->insertItem(i, def_name ? DEF_NAME : file_name, id);
}

/**
 * Removes file from the status bar file list.
 * @param id - src file unique ID.
 */

void status_line::remove_file(unsigned int id)
{
	int count = file_list_->count();
	
	for (int i = 0; i <= count; i++) {
		if (id == file_list_->itemData(i)) {
			file_list_->removeItem(i);
			break;
		}
	}
}

/**
 * [slot] Updates the current file name.
 * @param index -> index of the current file, -1 if there are 
 * no files.
 */

void status_line::update_file_name(int index)
{
	unsigned int id;
	cout << "update_file_name 1" << endl;
	
	try {
		id = src_container_->get_src_file_id(index);
		
		int count = file_list_->count();
	
		for (int i = 0; i <= count; i++) {
			if (id == file_list_->itemData(i)) {
				file_list_->setCurrentIndex(i);
				break;
			}
		}
		
	} catch (lgmx::exception &excp) {
		debug(ERR, STATUS_BAR, excp.get_message());
	}
}

/**
 * Updates the file name in the file specified by id.
 * @param fileName - the new file name.
 * @param id - the unique file ID.
 */

void status_line::update_file_name(const QString &fileName, unsigned int id)
{
	cout << "update_file_name 2" << endl;
	int count = file_list_->count();

	for (int i = 0; i <= count; i++) {
		if (id == file_list_->itemData(i)) {
			file_list_->setItemText(i, fileName);
			break;
		}
	}
}

void status_line::set_src_container(src_container *container)
{
	src_container_ = container;
}

/**
 * [slot]
 */

void status_line::file_name_clicked()
{
	std::cout << "label clicked" << std::endl;
}

/**
 * [slot]
 */

void status_line::current_file_changed(int index)
{
	unsigned int id;
	
	// get source file unique id
	id = file_list_->itemData(index).toUInt();
	src_container_->set_current_src_file(id);
}

/**
 * Sets status line's default colors.
 */

void status_line::set_default_colors()
{
	QPalette p;
	QColor c(0, 0, 0);
	p.setColor(QPalette::WindowText, c);
	file_list_->setPalette(p);
	//QColor c2(0, 0, 255);
	//p.setColor(QPalette::Window, c2);
	//file_name_->setPalette(p);
    file_list_->setStyleSheet("background: blue");
}













