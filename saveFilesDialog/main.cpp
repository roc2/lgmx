#include <QtGui/QApplication>
#include "savefiles.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SaveFiles w;
    w.show();

    return a.exec();
}
