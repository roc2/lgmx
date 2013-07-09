#include <QDir>
#include <QString>
#include <QShortcut>
#include <QPluginLoader>
#include <QTextBlock>
#include <QDockWidget>

#include <main_window.h>
#include <config.h>
#include <clipboard.h>
#include <view.h>
#include <text_manip.h>
#include <interfaces.h>
#include <debug.h>

#ifdef _DEBUG_
#include <QDateTime>
qint64 start;
#endif

//#include <QX11EmbedContainer>
#include <QProcess>

/**
 * @todo create user_opt class to parse command line user options
 */

/**
 * MainWindow Constructor with file list as parameter
 * @brief MainWindow Constructor
 */

MainWindow::MainWindow(std::list<QString> *files)
{
#ifdef _DEBUG_
	QDateTime boot_time;
	start =	boot_time.currentMSecsSinceEpoch();
#endif

	QCoreApplication::setOrganizationName(COMPANY);
    QCoreApplication::setApplicationName(APPLICATION);
	setObjectName(QString::fromUtf8("main_window"));
	setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::VerticalTabs);

	settings_ = new Settings();
	view_manager_ = new view_manager(this, &type_manager, settings_);
	gt_ln_dialog = NULL;

	search_dialog = new lgmx::search(*view_manager_, this);

	c_board = new clipboard(*view_manager_, this);
	text_manip_ = new text_manip(*view_manager_, this);

	createActions();

	view_manager_->set_recent_files_widget(menu_recent_files);

	widget_ = new QWidget(this);	// central widget
	main_layout_ = new QVBoxLayout(widget_);
	cli_ = new lgmx::cli(view_manager_);
	cli_->hide();

	main_layout_->addWidget(view_manager_);
	main_layout_->addWidget(cli_);
	main_layout_->setContentsMargins (0, 0, 0, 0);
	main_layout_->setSpacing(0);
	widget_->setLayout(main_layout_);
	setCentralWidget(widget_);

#if 0
	QDockWidget *dockWidget = new QDockWidget(tr("Dock Widget"), this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//dockWidget->setWidget(symbol_tab_widget);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	QX11EmbedContainer *container = new QX11EmbedContainer();
	container->show();
	//dockWidget->setWidget(container);
	QProcess *process = new QProcess(container);
	QString executable("/usr/bin/terminator");
	QStringList arguments;
	arguments << QString::number(container->winId());
	process->start(executable, arguments);
	container->embedClient(container->clientWinId());
	//this->show();

	QDockWidget *dockWidget_2 = new QDockWidget(tr("Dock Widget 2"), this);
	dockWidget_2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//dockWidget->setWidget(symbol_tab_widget);
	addDockWidget(Qt::RightDockWidgetArea, dockWidget_2);
#endif

	create_menus();

	/* add actions to main window, so they work when menuBar is hidden */
	addAction(actionNew);
	addAction(actionSave);
	addAction(actionOpen);
	addAction(action_close_);
	addAction(action_reload);
	addAction(action_split_horizontally);
	addAction(action_split_vertically);
	addAction(action_unsplit);
	addAction(action_remove_all_splits);

	/* view actions */
	//addAction(actionSide_Bar);
	addAction(actionStatus_Bar);
	addAction(actionMenuBar);
	addAction(actionFullScreen);
	addAction(actionSrcTabBar);

	menuBar->addAction(menuView->menuAction());
	menuBar->addAction(menu_Search->menuAction());
	menuView->addSeparator();
	menu_Search->addSeparator();
	menu_Search->addAction(actionGo_to_line);
	menu_Search->addAction(action_find);
	addAction(actionGo_to_line);
	addAction(action_find);

	create_shortcuts();
	create_connections();
	retranslateUi(this);

	readSettings();

	if (!files->empty()) {
		this->load_parameter_files(files);
	}

	//Config conf;
	debug(DEBUG, MAIN_WINDOW, "init time = " << boot_time.currentMSecsSinceEpoch() - start);
}

/**
 * Destructor.
 */

MainWindow::~MainWindow()
{
	delete text_manip_;
	delete c_board;
	delete search_dialog;

	if (gt_ln_dialog)
		delete gt_ln_dialog;

	destroy_menus();
	destroy_actions();
	destroy_shortcuts();

	delete view_manager_;
	delete settings_;
	delete cli_;
	delete main_layout_;
	delete widget_;
}

/**
 * Creates signal/slot connections.
 */

void MainWindow::create_connections()
{
	QObject::connect(actionSave, SIGNAL(triggered()), view_manager_, SLOT(save()));

	QObject::connect(actionOpen, SIGNAL(triggered()), view_manager_, SLOT(open_file()));

	QObject::connect(action_close_, SIGNAL(triggered()), view_manager_, SLOT(close_file()));

	QObject::connect(action_reload, SIGNAL(triggered()), view_manager_, SLOT(reload_current_file()));

	QObject::connect(actionNew, SIGNAL(triggered()), view_manager_, SLOT(new_file()));
	QObject::connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

	/* side bar */
	//QObject::connect(actionSide_Bar, SIGNAL(toggled(bool)), this, SLOT(show_side_bar(bool)));
	/* status bar */
	QObject::connect(actionStatus_Bar, SIGNAL(toggled(bool)), view_manager_, SLOT(show_status_bar(bool)));
	/* menu bar */
	QObject::connect(actionMenuBar, SIGNAL(toggled(bool)), this, SLOT(show_menu_bar(bool)));
	/* full screen */
	QObject::connect(actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(show_full_screen(bool)));
	/* source tab bar */
	QObject::connect(actionSrcTabBar, SIGNAL(toggled(bool)), view_manager_, SLOT(show_src_tab_bar(bool)));
	/* split horizontally */
	QObject::connect(action_split_horizontally, SIGNAL(triggered()), view_manager_, SLOT(split_horizontally()));
	/* split vertically */
	QObject::connect(action_split_vertically, SIGNAL(triggered()), view_manager_, SLOT(split_vertically()));
	/* unsplit */
	QObject::connect(action_unsplit, SIGNAL(triggered()), view_manager_, SLOT(unsplit()));
	/* remove all splits */
	QObject::connect(action_remove_all_splits, SIGNAL(triggered()), view_manager_, SLOT(remove_all_splits()));
	/* go to line */
	QObject::connect(actionGo_to_line, SIGNAL(triggered()), this, SLOT(go_to_ln()));
	/* find */
	QObject::connect(action_find, SIGNAL(triggered()), search_dialog, SLOT(show_search_dialog()));

	QMetaObject::connectSlotsByName(this);

	// file watcher signals
	//QObject::connect(&f_watcher, SIGNAL(reload(const QString &)), this, SLOT(reload_file(const QString &)));
	//QObject::connect(this, SIGNAL(windowActivated()), &f_watcher, SLOT(check_for_reload()));
}

/**
 * Creates main window shortcuts.
 */

void MainWindow::create_shortcuts()
{
	// next file - ctrl + tab
	next_file_ = new QShortcut(Qt::CTRL + Qt::Key_Tab, this);
	QObject::connect(next_file_, SIGNAL(activated()), view_manager_, SLOT(set_next_file_as_current()));
	// jump to tag - ctrl + ]
	go_to_tag_ = new QShortcut(Qt::CTRL + Qt::Key_BracketRight, this);
	QObject::connect(go_to_tag_, SIGNAL(activated()), view_manager_, SLOT(go_to_tag()));
	// show cli - ctrl + shift + c
	show_cli_ = new QShortcut(Qt::ALT + Qt::Key_C, this);
	QObject::connect(show_cli_, SIGNAL(activated()), this, SLOT(show_cli()));
}

/**
 * Destroys main window shortcuts.
 */

void MainWindow::destroy_shortcuts()
{
	delete next_file_;
	delete go_to_tag_;
	delete show_cli_;
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);

	if (e->type() == QEvent::ActivationChange) {
		if (isActiveWindow()) {
			//if (debugMainWindow)
			  //  qDebug() << "main window activated";
			debug(DEBUG, MAIN_WINDOW, "main window active");
			emit windowActivated();
		}
	} else if (e->type() == QEvent::WindowStateChange) {
#ifdef Q_WS_MAC
		bool minimized = isMinimized();

		//if (debugMainWindow)
		  //  qDebug() << "main window state changed to minimized=" << minimized;

		m_minimizeAction->setEnabled(!minimized);
		m_zoomAction->setEnabled(!minimized);
#else
		//bool isFullScreen = (windowState() & Qt::WindowFullScreen) != 0;
		//m_toggleFullScreenAction->setChecked(isFullScreen);
#endif
	}
}

bool MainWindow::is_active_window()
{
	return isActiveWindow();
}

/**
 * Open files passed as parameters.
 */

void MainWindow::load_parameter_files(std::list<QString> *files)
{
	QDir dir;
	QString curPath(dir.currentPath());

	for (std::list<QString>::iterator it = files->begin(); it != files->end(); it++) {
		if ((*it)[0] != '/') {  // append file path
			QString file(curPath + '/' + *it);
			view_manager_->open_file(file);
		} else
			view_manager_->open_file(*it);
	}
}

/**
 * Returns the user's home directory absolute path.
 * @return absolute path as a QString
 */

QString MainWindow::getHomePath()
{
	QDir dir;

	return dir.homePath();
}

/**
 * Show or hide the menu bar
 * @brief Show or hide the menu bar
 * @param show -> true, show menu; false, hide menu
 */

void MainWindow::show_menu_bar(bool show)
{
	if (show)
		menuBar->show();
	else
		menuBar->hide();
}

/**
 * Turns on/off the editor full screen mode
 * @brief Turns on/off the editor full screen mode
 */

void MainWindow::show_full_screen(bool)
{
	setWindowState(windowState() ^ Qt::WindowFullScreen);
}

/**
 * [slot] Creates go to line dialog and jumps to specified line.
 */

void MainWindow::go_to_ln()
{
	view *curr_view;
	visual_src_file *src_tab;

	curr_view = view_manager_->get_current_view();
	src_tab = curr_view->get_src_container()->get_current_src_file();

	if (!src_tab)
		return;

	// get current cursor position
	QTextCursor curr(src_tab->get_cursor());

	if (!gt_ln_dialog) {
		int line = curr.block().blockNumber() + 1;
		gt_ln_dialog = new go_to_line(this, line);
	}

	gt_ln_dialog->regular_size();
	gt_ln_dialog->set_focus();

	QObject::connect(gt_ln_dialog->spinBox, SIGNAL(valueChanged(int)), src_tab, SLOT(go_to_line(int)));

	// run modal dialog
	if (gt_ln_dialog->exec() == 1) {
		src_tab->go_to_line(gt_ln_dialog->get_line());
	} else {	// canceled, restore cursor
		src_tab->set_cursor(curr);
		src_tab->centerCursor();
	}

	QObject::disconnect(gt_ln_dialog->spinBox, SIGNAL(valueChanged(int)), src_tab, SLOT(go_to_line(int)));
}

/**
 * [slot] Shows the command line interface.
 */

void MainWindow::show_cli()
{
	cli_->show();
	cli_->setFocus();
}

/**
 * Store main window settings
 */

void MainWindow::writeSettings()
{
	QSettings settings(COMPANY, APPLICATION);

	//settings.setValue("mainWindow/geometry", saveGeometry());
	settings.setValue("mainWindow/size", size());
	settings.setValue("mainWindow/pos", pos());

	menu_recent_files->save_files_to_disk(settings);

	debug(DEBUG, MAIN_WINDOW, "Settings saved");
}

/**
 * Read and apply main window settings
 */

void MainWindow::readSettings()
{
	QSettings settings(COMPANY, APPLICATION);

	resize(settings.value("mainWindow/size", QSize(400, 400)).toSize());
	move(settings.value("mainWindow/pos", QPoint(200, 200)).toPoint());
	//restoreGeometry(settings.value("mainWindow/geometry").toByteArray());

	menu_recent_files->load_files_from_disk(settings);
}

/**
 * Closes the application. Checks if there are unsaved changes and saves
 * configuration before quiting.
 * @brief Closes the editor
 * @param event -> closing event
 */

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (!view_manager_->check_unsaved_files()) {
		event->ignore();
		return;
	}

	writeSettings();    /* save current settings */
	event->accept();
}

/**
 * Exit from application.
 */

void MainWindow::quit()
{
	this->close();
}

/**
 * Create main window actions.
 */

void MainWindow::createActions()
{
	actionSave = new QAction(this);
	actionSave->setObjectName(QString::fromUtf8("actionSave"));

	actionOpen = new QAction(this);
	actionOpen->setObjectName(QString::fromUtf8("actionOpen"));

	action_close_ = new QAction(this);
	action_close_->setObjectName(QString::fromUtf8("actionClose"));

	actionNew = new QAction(this);
	actionNew->setObjectName(QString::fromUtf8("actionNew"));

	action_reload = new QAction(this);
	action_reload->setObjectName(QString::fromUtf8("actionReload"));

	actionQuit = new QAction(this);
	actionQuit->setObjectName(QString::fromUtf8("actionQuit"));

	/* recent files menu */
	menu_recent_files = new recent_files(tr("Recent Files"), this);
	connect(menu_recent_files, SIGNAL(open_recent_file(const QString &)), view_manager_, SLOT(open_file(const QString &)));

	/* menu side bar */
	//actionSide_Bar = new QAction(this);
	//actionSide_Bar->setObjectName(QString::fromUtf8("actionSide_Bar"));
	//actionSide_Bar->setCheckable(true);
	//actionSide_Bar->setChecked(true);	// default "checked"
	//actionSide_Bar->setStatusTip(tr("Show or hide side bar"));

	/* menu status bar */
	actionStatus_Bar = new QAction(this);
	actionStatus_Bar->setObjectName(QString::fromUtf8("actionStatus_Bar"));
	actionStatus_Bar->setCheckable(true);
	actionStatus_Bar->setChecked(true);	// default "checked"

	/* menu bar */
	actionMenuBar = new QAction(this);
	actionMenuBar->setObjectName(QString::fromUtf8("actionMenu_Bar"));
	actionMenuBar->setCheckable(true);
	actionMenuBar->setChecked(true);	// default "checked"

	/* full screen */
	actionFullScreen = new QAction(this);
	actionFullScreen->setObjectName(QString::fromUtf8("actionFull_Screen"));
	actionFullScreen->setCheckable(true);
	actionFullScreen->setChecked(false);	// default "unchecked"

	/* source tab bar */
	actionSrcTabBar = new QAction(this);
	actionSrcTabBar->setObjectName(QString::fromUtf8("actionSrcTabBar"));
	actionSrcTabBar->setCheckable(true);
	actionSrcTabBar->setChecked(true);	// default "checked"

	/* split horizontally */
	action_split_horizontally = new QAction(this);
	action_split_horizontally->setObjectName(QString::fromUtf8("splitHorizontally"));

	/* split vertically */
	action_split_vertically = new QAction(this);
	action_split_vertically->setObjectName(QString::fromUtf8("splitVertically"));

	/* unsplit */
	action_unsplit = new QAction(this);
	action_unsplit->setObjectName(QString::fromUtf8("unsplit"));

	/* remove all splits */
	action_remove_all_splits = new QAction(this);
	action_remove_all_splits->setObjectName(QString::fromUtf8("removeAllSplits"));

	/* go to line */
	actionGo_to_line = new QAction(this);
	actionGo_to_line->setObjectName(QString::fromUtf8("actionGo_to_line"));

	/* find */
	action_find = new QAction(this);
	action_find->setObjectName(QString::fromUtf8("actionFind"));
}

/**
 * Destroy main window actions.
 */

void MainWindow::destroy_actions()
{
	delete action_find;
	delete actionGo_to_line;
	delete actionSrcTabBar;
	delete actionFullScreen;
	delete action_split_horizontally;
	delete action_split_vertically;
	delete action_unsplit;
	delete action_remove_all_splits;
	delete actionMenuBar;
	delete actionStatus_Bar;
	//delete actionSide_Bar;
	delete menu_recent_files;
	delete actionQuit;
	delete actionNew;
	delete action_close_;
	delete actionOpen;
	delete action_reload;
	delete actionSave;
}

/**
 * Create menus.
 */

void MainWindow::create_menus()
{
	menuBar = new QMenuBar(this);
	menuBar->setObjectName(QString::fromUtf8("menuBar"));
	menuBar->setGeometry(QRect(0, 0, 831, 25));

	menu_File = new QMenu(menuBar);
	menu_File->setObjectName(QString::fromUtf8("menu_File"));

	menuView = new QMenu(menuBar);
	menuView->setObjectName(QString::fromUtf8("menuView"));
	/* search menu */
	menu_Search = new QMenu(menuBar);
	menu_Search->setObjectName(QString::fromUtf8("menu_Search"));

	setMenuBar(menuBar);

	menuBar->addAction(menu_File->menuAction());
	menu_File->addAction(actionNew);
	menu_File->addAction(actionSave);
	menu_File->addAction(actionOpen);

	menu_File->addSeparator();
	menu_File->addAction(action_close_);

	menu_File->addSeparator();
	menu_File->addMenu(menu_recent_files);

	menu_File->addSeparator();
	menu_File->addAction(action_reload);

	menu_File->addSeparator();
	menu_File->addAction(actionQuit);

	menuView->addSeparator();
	//menuView->addAction(actionSide_Bar);
	menuView->addAction(actionStatus_Bar);
	menuView->addAction(actionMenuBar);
	menuView->addAction(actionFullScreen);
	menuView->addAction(actionSrcTabBar);
	menuView->addSeparator();
	menuView->addAction(action_split_horizontally);
	menuView->addAction(action_split_vertically);
	menuView->addAction(action_unsplit);
	menuView->addAction(action_remove_all_splits);
}

void MainWindow::destroy_menus()
{
	delete menu_Search;
	delete menuView;
	delete menu_File;
	delete menuBar;
}

/**
 * Translate main window interface.
 */

void MainWindow::retranslateUi(QMainWindow *main_window)
{
	main_window->setWindowTitle(QApplication::translate("main_window", "lgmx", 0));
	actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
	actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
	actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
	actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));

	action_close_->setText(QApplication::translate("MainWindow", "Close", 0));
	action_close_->setShortcut(QApplication::translate("MainWindow", "Ctrl+F4", 0));

	actionNew->setText(QApplication::translate("MainWindow", "New", 0));
	actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
	action_reload->setText(QApplication::translate("MainWindow", "Reload", 0));
	action_reload->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));

	actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
	actionQuit->setShortcut(QApplication::translate("MainWindow", "Alt+F4", 0));

	//actionSide_Bar->setText(QApplication::translate("main_window", "Side Bar", 0));
	//actionSide_Bar->setShortcut(QApplication::translate("main_window", "Alt+X", 0));

	actionStatus_Bar->setText(QApplication::translate("main_window", "Status Bar", 0));
	actionStatus_Bar->setShortcut(QApplication::translate("main_window", "Alt+Z", 0));

	actionMenuBar->setText(QApplication::translate("main_window", "Menu Bar", 0));
	actionMenuBar->setShortcut(QApplication::translate("main_window", "Alt+4", 0));
	actionMenuBar->setShortcutContext(Qt::ApplicationShortcut);

	actionFullScreen->setText(QApplication::translate("main_window", "Full Screen", 0));
	actionFullScreen->setShortcut(QApplication::translate("main_window", "F11", 0));
	actionFullScreen->setShortcutContext(Qt::ApplicationShortcut);

	action_split_horizontally->setText(QApplication::translate("main_window", "Split Horizontally", 0));
	action_split_horizontally->setShortcut(QApplication::translate("main_window", "Ctrl+E, 2", 0));
	action_split_horizontally->setShortcutContext(Qt::ApplicationShortcut);

	action_split_vertically->setText(QApplication::translate("main_window", "Split Vertically", 0));
	action_split_vertically->setShortcut(QApplication::translate("main_window", "Ctrl+E, 3", 0));
	action_split_vertically->setShortcutContext(Qt::ApplicationShortcut);

	action_unsplit->setText(QApplication::translate("main_window", "Unsplit", 0));
	action_unsplit->setShortcut(QApplication::translate("main_window", "Ctrl+E, 0", 0));
	action_unsplit->setShortcutContext(Qt::ApplicationShortcut);

	action_remove_all_splits->setText(QApplication::translate("main_window", "Remove all splits", 0));
	action_remove_all_splits->setShortcut(QApplication::translate("main_window", "Ctrl+E, 1", 0));
	action_remove_all_splits->setShortcutContext(Qt::ApplicationShortcut);

	actionSrcTabBar->setText(QApplication::translate("main_window", "Source Tab Bar", 0));
	actionSrcTabBar->setShortcut(QApplication::translate("main_window", "Alt+3", 0));
	actionSrcTabBar->setShortcutContext(Qt::ApplicationShortcut);

	actionGo_to_line->setText(QApplication::translate("main_window", "Go to line", 0));
	actionGo_to_line->setShortcut(QApplication::translate("main_window", "Ctrl+L", 0));

	action_find->setText(QApplication::translate("main_window", "Find", 0));
	action_find->setShortcut(QApplication::translate("main_window", "Ctrl+F", 0));

	menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
	menuView->setTitle(QApplication::translate("main_window", "&View", 0));
	menu_Search->setTitle(QApplication::translate("main_window", "&Search", 0));
}

/**
 * Load plugins.
 */

void MainWindow::load_plugins()
{
	QDir pluginsDir = QDir(QApplication::applicationDirPath());
	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));

		QObject *plugin = loader.instance();	// get plugin instance

		if (plugin) {
			debug(DEBUG, MAIN_WINDOW, "plugin loaded: " << fileName.toStdString());

			Highlighter *hl = qobject_cast<Highlighter *>(plugin);

			if (hl) {
				debug(DEBUG, MAIN_WINDOW, "plugin Ok!!");
				debug(DEBUG, MAIN_WINDOW, hl->test_interface().toStdString());

				if (loader.unload()) {
					debug(DEBUG, MAIN_WINDOW, "plugin unloaded Ok!!");
				} else {
					debug(DEBUG, MAIN_WINDOW, "could not unload plugin");
				}
			} else
				debug(DEBUG, MAIN_WINDOW, "plugin not ok");

		} else {
			debug(DEBUG, MAIN_WINDOW, "plugin not loaded: " << fileName.toStdString());
		}
     }
}

