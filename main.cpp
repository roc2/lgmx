#include <QMainWindow>
#include "ui_main_window.h"
#include "code_editor.h"

#define MAIN 0  

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef MAIN
    //QMainWindow *MainWindow = new QMainWindow;
    Ui_MainWindow main_window;
    
    main_window.show();
    
    //main_window.setupUi(MainWindow);
	//MainWindow->showMaximized();
	//MainWindow->show();
#else
    Ui_MainWindow *main_window = new Ui_MainWindow;
    main_window->setupUi(main_window);
    main_window->show();
#endif
	//CodeEditor editor;
    //editor.setWindowTitle(QObject::tr("Code Editor Example"));
    //editor.show();

    return a.exec();
}
