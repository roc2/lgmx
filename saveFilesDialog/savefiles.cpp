#include "savefiles.h"
#include "ui_savefiles.h"

SaveFiles::SaveFiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveFiles)
{
    ui->setupUi(this);
}

SaveFiles::~SaveFiles()
{
    delete ui;
}
