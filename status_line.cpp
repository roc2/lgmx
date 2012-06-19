#include "status_line.h"


status_line::status_line()
{
	 layout_ = new QHBoxLayout;
     
     layout_->addWidget(&file_name_);
     this->setLayout(layout_);
}

status_line::~status_line()
{
	delete layout_;
}

void status_line::set_file_name(const QString &file_name)
{
	file_name_.setText(file_name);
}

