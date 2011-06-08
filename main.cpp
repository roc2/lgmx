#include <QMainWindow>
#include "ui_main_window.h"
#include "code_editor.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Ui_MainWindow main_window;
    
    main_window.show();
    
    cout << "argc = " << argc << endl;
    if (argc > 1)
        main_window.openParameterFile(argc, argv);
    
    return a.exec();
}
