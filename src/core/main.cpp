#include <QMainWindow>
#include "ui_main_window.h"

#define Q_WS_X11


int main(int argc, char *argv[])
{
#ifdef Q_WS_X11
    // QML is unusable with the xlib backend
    QApplication::setGraphicsSystem("raster");
#endif
	
    QApplication a(argc, argv);
    Ui_MainWindow *main_window;

    {
		list<QString> file_list;

		for (int i = 1; i < argc; i++)
			file_list.push_back(argv[i]);

		main_window = new Ui_MainWindow(&file_list);
	}
    
    main_window->show();
    return a.exec();
}
