#include "go_to_line.h"
#include "ui_go_to_line.h"

go_to_line::go_to_line(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::go_to_line)
{
    ui->setupUi(this);
}

go_to_line::~go_to_line()
{
    delete ui;
}
