#include <main_window.h>
#include <debug.h>
#include <CLI_args.h>


int main(int argc, char *argv[])
{
	debug(INFO, MAIN, "lgmx - Compiled with Qt " << QT_VERSION_STR);
	QApplication *app;
	MainWindow *main_window;

	{
		CLI_args args(argc, argv);
		if (!args.exec_pre_init())
			return 0;		// found termination option

		app = new QApplication(argc, argv);
		main_window = new MainWindow();
		main_window->init(args);
	}

	main_window->show();
	int res = app->exec();
	delete main_window;
	delete app;

	return res;
}

