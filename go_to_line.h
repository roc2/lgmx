#ifndef GO_TO_LINE_H
#define GO_TO_LINE_H

#include <QDialog>
#include "ui_go_to_line.h"
#include "src_area.h"

class go_to_line : public QDialog, public Ui_go_to_line
{
    Q_OBJECT

    int line;

public:
    go_to_line(QWidget *parent = 0);
    ~go_to_line();
    int get_line();
    void regular_size();

private slots:
    void input_changed(int val);
    void accept();
};

#endif
