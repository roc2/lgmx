#ifndef SAVEFILES_H
#define SAVEFILES_H

#include <QDialog>

namespace Ui {
    class SaveFiles;
}

class SaveFiles : public QDialog
{
    Q_OBJECT

public:
    explicit SaveFiles(QWidget *parent = 0);
    ~SaveFiles();

private:
    Ui::SaveFiles *ui;
};

#endif // SAVEFILES_H
