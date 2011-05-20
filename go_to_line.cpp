#include "go_to_line.h"
#include "ui_go_to_line.h"
#include <iostream>

using namespace std;

go_to_line::go_to_line(QWidget *parent) : QDialog(parent)   /*, ui(new go_to_line)*/
{
    setupUi(this);
    
    line = 0;
    //Ui_go_to_line *ui;
    
    //ui = new Ui_go_to_line;
    //ui->setupUi(this);
    
    //connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(input_changed(int)));
    connect(ok_button, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
}

go_to_line::~go_to_line()
{
    //delete ui;
}

void go_to_line::accept()
{
    int val = spinBox->value();
    
    if (val > 0)
        line = val;
    else
        line = 0;
    
    done(1);
}

int go_to_line::get_line()
{
    return line;
}

void go_to_line::regular_size()
{
    resize(252, 100);
}

void go_to_line::input_changed(int val)
{
    cout << "value changed: " << val << endl;
    
    if (spinBox->value() > 0)
        ok_button->setEnabled(true);
    else
        ok_button->setEnabled(false);
}
