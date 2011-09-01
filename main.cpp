#include <QMainWindow>
#include "ui_main_window.h"
#include "code_editor.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    list<QString> *file_list = new list<QString>;

    for (int i = 1; i < argc; i++)
        file_list->push_back(argv[i]);
    
    Ui_MainWindow main_window(file_list);
    delete file_list;
    
    main_window.show();
    return a.exec();
}
