#include <QMainWindow>
#include <main_window.h>
#include <list>
#include <debug.h>

#define Q_WS_X11


int main(int argc, char *argv[])
{
	debug(INFO, MAIN, "lgmx - Compiled with Qt " << QT_VERSION_STR);
	QApplication a(argc, argv);
	MainWindow *main_window;

	{
		std::list<QString> file_list;

		for (int i = 1; i < argc; i++)
			file_list.push_back(argv[i]);

		main_window = new MainWindow(&file_list);
	}

	main_window->show();
	int res = a.exec();
	delete main_window;

	return res;
}

