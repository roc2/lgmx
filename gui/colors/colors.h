#ifndef COLORS_H
#define COLORS_H

#include <QDialog>

namespace Ui {
    class colors;
}

class colors : public QDialog
{
    Q_OBJECT

public:
    explicit colors(QWidget *parent = 0);
    ~colors();

private:
    Ui::colors *ui;
};

#endif // COLORS_H
