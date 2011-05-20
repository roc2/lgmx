#include <QtGui/QApplication>
#include "go_to_line.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    go_to_line w;
    w.show();

    return a.exec();
}
