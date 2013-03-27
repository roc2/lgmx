#include <c_label.h>

#include <iostream>


c_label::c_label(const QString &text, QWidget *parent) : QLabel(text, parent)
{
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

c_label::~c_label()
{
}

void c_label::slotClicked()
{
	std::cout << "label clicked" << std::endl;
}
 
void c_label::mousePressEvent(QMouseEvent*) 
{
	emit clicked();
}
