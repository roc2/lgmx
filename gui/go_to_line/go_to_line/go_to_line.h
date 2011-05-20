#ifndef GO_TO_LINE_H
#define GO_TO_LINE_H

#include <QDialog>

namespace Ui {
    class go_to_line;
}

class go_to_line : public QDialog
{
    Q_OBJECT

public:
    explicit go_to_line(QWidget *parent = 0);
    ~go_to_line();

private:
    Ui::go_to_line *ui;
};

#endif // GO_TO_LINE_H
