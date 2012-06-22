#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <cstdlib>
#include <QDir>
#include <QString>
#include <QShortcut>

#include "ui_main_window.h"
#include "code_editor.h"
#include "config.h"
#include "unsvFileDialog.h"
#include "clipboard.h"

#include "string"

#include <QDateTime>

using namespace std;


/**
 * MainWindow Constructor with file list as parameter
 * @brief MainWindow Constructor
 */

qint64 start;

Ui_MainWindow::Ui_MainWindow(list<QString> *files) : _src_container(this), view_manager_(this)
{
	QDateTime boot_time;
	start =	boot_time.currentMSecsSinceEpoch();
	
    if (objectName().isEmpty())
		setObjectName(QString::fromUtf8("main_window"));
        
	//resize(831, 557);
	//showMaximized();

	/*
	 * atualmente este ponteiro é fixo, mas a ideia é que ele sempre aponte para o grupo de 
	 * arquivos ativo. No caso de um split screen este ponteiro deve ser atualizado, para possibilitar 
	 * os outros modulos de requerer o arquivo ativo.
	 * Todos os módulos devem receber o endereço deste ponteiro.
	 * src_files deve ser um vector, a cada split screen é adicionado um elemento no vector, e o ponteiro 
	 * src_container deve sempre apontar para o grupo de arquivos ativo. Como os módulos apontam para src_container, 
	 * sempre terão o endereço do grupo de arquivos ativo.
	 */
	_src_container_ptr = &_src_container;

	_root_view = view_manager_.get_root_view();
	_root_src_container = _root_view->get_src_container();
	//_root_src_container = view_manager_.get_root_src_container();

	gt_ln_dialog = NULL;
	
	QCoreApplication::setOrganizationName(COMPANY);
    //QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName(APPLICATION);
	
	
	search_dialog = new lgmx::search(view_manager_, this);
	
	c_board = new clipboard(&_src_container_ptr, this);
	
	createActions();
	
	view_manager_.set_recent_files_widget(menu_recent_files);
	
	centralWidget = new QWidget(this);
	centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
	
	horizontalLayout = new QHBoxLayout(centralWidget);
	horizontalLayout->setSpacing(6);
	horizontalLayout->setContentsMargins(0, 0, 0, 0);
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	
	// main splitter
	splitter = new QSplitter(centralWidget);
	splitter->setHandleWidth(1);
	splitter->setChildrenCollapsible(false);
	splitter->setProperty("minisplitter", true);
	splitter->setObjectName(QString::fromUtf8("splitter"));
	splitter->setOrientation(Qt::Horizontal);
	
	symbol_tab_widget = new QTabWidget(splitter);
	symbol_tab_widget->setObjectName(QString::fromUtf8("symbol_tab_widget"));
	symbol_tab_widget->setTabsClosable(true);
	symbol_tab_widget->setMovable(true);
	//symbol_tab_widget->show();
	//symbol_tab_widget->hide();
	
	tab = new QWidget();
	tab->setObjectName(QString::fromUtf8("tab"));
	symbol_tab_widget->addTab(tab, QString());
	
	tab_2 = new QWidget();
	tab_2->setObjectName(QString::fromUtf8("tab_2"));
	symbol_tab_widget->addTab(tab_2, QString());
	
	splitter->addWidget(symbol_tab_widget);
	//splitter->addWidget(&_src_container);
	splitter->addWidget(&view_manager_);
	
	
	horizontalLayout->addWidget(splitter);
	setLayout(horizontalLayout);
	
	
	/* Configure splitter sizes. This must be called after the child widgets 
	 * were inserted.
	 */
	splitter_size.append(100);
	splitter_size.append(900);
	splitter->setSizes(splitter_size);
	//splitter_size = splitter->sizes();	/* this returns the current splitter sizes */

	setCentralWidget(centralWidget);
	menuBar = new QMenuBar(this);
	menuBar->setObjectName(QString::fromUtf8("menuBar"));
	menuBar->setGeometry(QRect(0, 0, 831, 25));
	
	
	menu_File = new QMenu(menuBar);
	menu_File->setObjectName(QString::fromUtf8("menu_File"));
	
	menuView = new QMenu(menuBar);
	//menu_View = new QMenu(menuBar);
	menuView->setObjectName(QString::fromUtf8("menuView"));
	//menu_View->setObjectName(QString::fromUtf8("menu_View"));
	/* search menu */
	menu_Search = new QMenu(menuBar);
	menu_Search->setObjectName(QString::fromUtf8("menu_Search"));
	
	setMenuBar(menuBar);
		
	/* main window tool bar */
	mainToolBar = new QToolBar(this);
	mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
	addToolBar(Qt::TopToolBarArea, mainToolBar);
	mainToolBar->hide();	/* hide tool bar */
	//mainToolBar->show();	/* show tool bar */
	
	/* main window status bar */
	statusBar = new QStatusBar(this);
	statusBar->setObjectName(QString::fromUtf8("statusBar"));
	setStatusBar(statusBar);

	menuBar->addAction(menu_File->menuAction());
	menu_File->addAction(actionNew);
	menu_File->addAction(actionSave);
	menu_File->addAction(actionOpen);

	menu_File->addSeparator();
	menu_File->addMenu(menu_recent_files);
	
	menu_File->addSeparator();
	menu_File->addAction(actionQuit);
	
	menuView->addSeparator();
	menuView->addAction(actionSide_Bar);
	menuView->addAction(actionStatus_Bar);
	menuView->addAction(actionMenuBar);
	menuView->addAction(actionFullScreen);
	menuView->addAction(actionSrcTabBar);
	menuView->addSeparator();
	menuView->addAction(action_split_horizontally);
	menuView->addAction(action_split_vertically);
	menuView->addAction(action_unsplit);
	
	/* add actions to main window, so they work when menuBar is hidden */
	addAction(actionNew);
	addAction(actionSave);
	addAction(actionOpen);
	addAction(action_split_horizontally);
	addAction(action_split_vertically);
	addAction(action_unsplit);
	
	/* view actions */
	addAction(actionSide_Bar);
	addAction(actionStatus_Bar);
	addAction(actionMenuBar);
	addAction(actionFullScreen);
	addAction(actionSrcTabBar);
	
	menuBar->addAction(menuView->menuAction());
	menuBar->addAction(menu_Search->menuAction());
	menuView->addSeparator();
	//menuView->addAction(actionSide_Bar);
	//menuView->addAction(actionStatus_Bar);
	menu_Search->addSeparator();
	menu_Search->addAction(actionGo_to_line);
	menu_Search->addAction(action_find);
	addAction(actionGo_to_line);
	addAction(action_find);
		
	create_shortcuts();
	
	retranslateUi(this);
	
	create_connections();

	set_font();
	readSettings();
	
	if (!files->empty()) {
		this->load_parameter_files(files);
	}
	
	//Config conf;
	show_status_bar(false);
	std::cout << "init time = " << boot_time.currentMSecsSinceEpoch() - start << std::endl;
}

Ui_MainWindow::~Ui_MainWindow()
{
	delete c_board;
	delete search_dialog;

    destroy_actions();
}

void Ui_MainWindow::create_connections()
{
	QObject::connect(actionSave, SIGNAL(triggered()), &view_manager_, SLOT(save()));

	QObject::connect(actionOpen, SIGNAL(triggered()), &view_manager_, SLOT(open_file()));
	
	QObject::connect(actionNew, SIGNAL(triggered()), &view_manager_, SLOT(new_file()));
	QObject::connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
	
	/* side bar */
	QObject::connect(actionSide_Bar, SIGNAL(toggled(bool)), this, SLOT(show_side_bar(bool)));
	/* status bar */
	QObject::connect(actionStatus_Bar, SIGNAL(toggled(bool)), this, SLOT(show_status_bar(bool)));
	/* menu bar */
	QObject::connect(actionMenuBar, SIGNAL(toggled(bool)), this, SLOT(show_menu_bar(bool)));
	/* full screen */
	QObject::connect(actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(show_full_screen(bool)));
	/* source tab bar */
	QObject::connect(actionSrcTabBar, SIGNAL(toggled(bool)), this, SLOT(show_src_tab_bar(bool)));
	/* split horizontally */
	QObject::connect(action_split_horizontally, SIGNAL(triggered()), &view_manager_, SLOT(split_horizontally()));
	/* split vertically */
	QObject::connect(action_split_vertically, SIGNAL(triggered()), &view_manager_, SLOT(split_vertically()));
	/* unsplit */
	QObject::connect(action_unsplit, SIGNAL(triggered()), &view_manager_, SLOT(unsplit()));
	
	/* go to line */
	QObject::connect(actionGo_to_line, SIGNAL(triggered()), this, SLOT(go_to_ln()));
	/* find */
	QObject::connect(action_find, SIGNAL(triggered()), search_dialog, SLOT(show_search_dialog()));

	/* close file signal */
	QObject::connect(&_src_container, SIGNAL(tabCloseRequested ( int )), this, SLOT(close_file(int)));

	//Object::connect(this, SIGNAL(windowActivated()), this, SLOT(go_to_ln()));

	QMetaObject::connectSlotsByName(this);
	
	// file watcher signals
	QObject::connect(&f_watcher, SIGNAL(reload(const QString &)), this, SLOT(reload_file(const QString &)));
	//QObject::connect(this, SIGNAL(windowActivated()), &f_watcher, SLOT(check_for_reload()));
}

void Ui_MainWindow::create_shortcuts()
{

}

/**
 * 
 */

src_container* Ui_MainWindow::get_current_src_container()
{
	return view_manager_.get_current_src_container();
}

void Ui_MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    
    if (e->type() == QEvent::ActivationChange) {
        if (isActiveWindow()) {
			//if (debugMainWindow)
              //  qDebug() << "main window activated";
            cout << "main window activated" << endl;
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

bool Ui_MainWindow::is_active_window()
{
	return isActiveWindow();
}
/**
 * Closes a specific source file (tab).
 * @param index -> tab index in the tab widget
 */

void Ui_MainWindow::close_file(int index)
{
    QString file_name, msg;
	QMessageBox::StandardButton ret;
	set<QString>::iterator it;
    
    cout << __FUNCTION__ << endl;
    
    //file_name = _src_container.get_src_tab_full_name(index);
    _src_container.get_src_tab_full_name(index, file_name);
    
    /* check if file needs to be saved */
	if (_src_container.is_modified(index)) {
		
        this->build_close_file_msg(index, msg);
        
        ret = QMessageBox::warning(this, APPLICATION, msg,
			  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	
		if (ret == QMessageBox::Save) { /* save file */
            
            if (file_name.isEmpty()) {
                cout << "file name empty" << endl;
                if (!saveAs(index))
                    return; /* could not save, just return */
            } else {
                if (!saveFile(file_name, index))
                    return; /* could not save, just return */
            }
            
        } else if (ret == QMessageBox::Cancel)
			return;     /* if dialog is canceled, do nothing */
	}
	
	_src_container.destroy_src_tab(index);   /* closes the file */
    f_watcher.remove_path(file_name);   /* removes from modification watcher list */
    
    it = open_files.find(file_name);    /* pop from open files list */
    if (it != open_files.end())
        open_files.erase(*it);
}

void Ui_MainWindow::reload_file(const QString path)
{
	cout << "reloading file " << path.toStdString() << endl;

}

/**
 * Save file as. Saves a file which has not been written to the 
 * disk yet.
 * @param index - file index number in the tab widget
 */

bool Ui_MainWindow::saveAs(int index)
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    // mudar para abrir na home do user se o arquivo nao existe:
    //files = QFileDialog::getSaveFileName(this, tr("Save File"), path, tr("All files (*.c *.cpp *.h)"));
    
    if (fileName.isEmpty())
        return false;   /* no file specified */

    if (saveFile(fileName, index)) {
        _src_container.set_file_name(index, fileName);
        _src_container.update_file_info(index);
        f_watcher.add_path(fileName);
        return true;
    }
    
    return false;
}

/**
 * Saves file to disk
 * @brief Saves file to disk
 * @param fileName -> file name
 * @param index -> index in the source tab
 * @return true -> file saved successfully, false -> error
 */

bool Ui_MainWindow::saveFile(const QString &fileName, int index)
{
    QString error;
    
    cout << "file name -> " << fileName.toStdString() << endl;

    if (!_src_container.src_tab_write_file(index, fileName))
        return false;

    _src_container.set_modified(index, false);
    open_files.insert(fileName);
    menu_recent_files->add_file(fileName);

    return true;
}

void Ui_MainWindow::set_current_index(int index)
{
    if (index >= _src_container.count())
        return; /* index out of range */
    
    _src_container.setCurrentIndex(index);
}

int Ui_MainWindow::get_file_index(const QString &file_name)
{
	return _src_container.get_file_index(file_name);
}

/**
 * Open files passed as parameters.
 */

void Ui_MainWindow::load_parameter_files(list<QString> *files)
{
    QDir dir;
    QString curPath(dir.currentPath());
    
    for (list<QString>::iterator it = files->begin(); it != files->end(); it++) {
        if ((*it)[0] != '/') {  // append file path
			QString file(curPath + '/' + *it);
            view_manager_.open_file(file);
        } else
			view_manager_.open_file(*it);
    }
}

/**
 * 
 */

void Ui_MainWindow::set_font()
{
	QFont initial;
    
	initial.setFamily("monospace");
	initial.setFixedPitch(true);
	initial.setPointSize(10);

    cout << "will set font..." << endl;
    _root_src_container->setFont(initial);
/*
	cout << "choose font!!" << endl;

	QFont font = QFontDialog::getFont(&ok, QFont("Times", 12), this);
	if (ok) {
		cout << "OK" << endl;
     // font is set to the font the user selected
	} else {
		cout << "canceled" << endl;
     // the user canceled the dialog; font is set to the initial
     // value, in this case Times, 12.
	}
*/
	
	//editor->setFont(font);
}

void Ui_MainWindow::set_tab_width()
{
	QFont font;
	QFontMetrics font_metrics(font);
	int size;
	
	_root_src_container->get_curr_font(0, font);
	
	if ((size = font.pixelSize()) < 0) {
		//size = font.pointSize();
		size = font_metrics.width('a');
	}
	cout << "size = " << size << endl;
}

/**
 * Returns the user home directory absolute path.
 * @return absolute path as a QString
 */

QString Ui_MainWindow::getHomePath()
{
    QDir dir;
    
    return dir.homePath();
}

/**
 * Show or hide the side bar.
 * @brief Show or hide the side bar.
 * @param show -> true, show bar false, hide bar
 */

void Ui_MainWindow::show_side_bar(bool show)
{
	if (show)
		symbol_tab_widget->show();
	else
		symbol_tab_widget->hide();
}

/**
 * Show or hide the status bar.
 * @brief Show or hide the side bar.
 * @param show -> true, show bar false, hide bar
 */

void Ui_MainWindow::show_status_bar(bool show)
{
	if (show)
		statusBar->show();
	else
		statusBar->hide();
}

/**
 * Show or hide the menu bar
 * @brief Show or hide the menu bar
 * @param show -> true, show menu; false, hide menu
 */

void Ui_MainWindow::show_menu_bar(bool show)
{
	if (show)
		menuBar->show();
	else
        menuBar->hide();
}

/**
 * Turns on/off the editor full screen mode
 * @brief Turns on/off the editor full screen mode
 * @param fullscreen -> true, show full screen; false, show normal mode
 */

void Ui_MainWindow::show_full_screen(bool fullscreen)
{
	if (fullscreen)
        showFullScreen();
	else
        showNormal();
}

/**
 * Show or hide source files tab bar.
 * @param show -> true, tab bar; false, hide tab bar
 */

void Ui_MainWindow::show_src_tab_bar(bool show)
{
	_root_view->show_src_tab_bar(show);
}

/**
 * Go to specified line
 * @brief Go to specified line
 */

void Ui_MainWindow::go_to_ln()
{
	int line = 0;
	int index;
	view *curr_view;
	
	curr_view = view_manager_.get_current_view();
	
	if (!curr_view)
		return;
	
	index = curr_view->get_src_container()->get_current_tab_index();
	
	if (index < 0)
		return;
	
	if (!gt_ln_dialog) {
		cout << "new go to line object" << endl;
		gt_ln_dialog = new go_to_line(this);
	}
	
	gt_ln_dialog->regular_size();
	if (gt_ln_dialog->exec() == 1) {
		line = gt_ln_dialog->get_line();
		
		if (line <= 0)
			return;
		
		//_src_container.go_to_line(index, line);
		curr_view->get_src_container()->go_to_line(index, line);
	}
}

/**
 * Store main window settings
 */

void Ui_MainWindow::writeSettings()
{
	QSettings settings(COMPANY, APPLICATION);

    settings.setValue("pos", pos()); 
    settings.setValue("size", size());

	menu_recent_files->save_files_to_disk(settings);
	//search_dialog->save_settings();

	//settings.setValue("geometry", saveGeometry());
    //settings.setValue("windowState", saveState());
    
    delete search_dialog;
    
	cout << "Saved settings" << endl;
}

/**
 * Read and apply main window settings
 */

void Ui_MainWindow::readSettings()
{
	QSettings settings(COMPANY, APPLICATION);
	//QRect rect;

    //restoreGeometry(settings.value("myWidget/geometry").toByteArray());
    //restoreState(settings.value("myWidget/windowState").toByteArray());

    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);

	//rect = settings.value("geometry", QRect(200, 200, 400, 400)).toRect();
	
	//move(rect.topLeft());
	//resize(rect.size());
	menu_recent_files->load_files_from_disk(settings);
}

/**
 * Constructs the message to warn the user the file has non saved changes
 * @brief Constructs the message to warn the user the file has non saved changes
 * @param index -> tab index
 * @param msg -> string to hold the message
 */

void Ui_MainWindow::build_close_file_msg(int index, QString &msg)
{
    QString file = _src_container.get_src_tab_short_name(index);
            
    if (file.isEmpty()) {
        msg = tr("The file 'untitled' has been modified.\nDo you want to save your changes?");
    } else {
        msg = tr("The file '") + file + 
              tr("' has been modified.\nDo you want to save your changes?");
    }
}

/**
 * Checks if there are unsaved files before closing the application. The 
 * files are saved or discarded, according to the user's request.
 * @brief Checks if there are unsaved files before closing the application.
 * @return true -> files saved or discarded, Ok to close the application. 
 * false -> The closing was canceled.
 */

bool Ui_MainWindow::checkUnsavedFiles()
{
    int index, tabs;
    QMessageBox::StandardButton ret;
    QString file_name;
    QString msg;
    
    tabs = _src_container.count();
    
    /* check for unsaved files and save them if requested */
    for (index = 0; index < tabs; index++) {

        if (_src_container.is_modified(index)) {
            _src_container.setCurrentIndex(index);
            
            this->build_close_file_msg(index, msg);
            
            ret = QMessageBox::warning(this, APPLICATION, msg,
			      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            
            if (ret == QMessageBox::Save) { /* save file */
            
                //file_name = _src_container.get_src_tab_full_name(index);
                _src_container.get_src_tab_full_name(index, file_name);
                
                if (file_name.isEmpty()) {
                    if (!saveAs(index))
                        return false; /* could not save, just return */
                    
                } else {
                    if (!saveFile(file_name, index))
                        return false; /* could not save, just return */
                    
                }
            } else if (ret == QMessageBox::Cancel)
                return false;     /* if dialog is canceled, do nothing */
        }
    }
    
    return true;
}

/**
 * Closes the editor. Checks if there are unsaved changes and saves 
 * configuration before quiting.
 * @brief Closes the editor
 * @param event -> closing event
 */

void Ui_MainWindow::closeEvent(QCloseEvent *event)
{
    if (!checkUnsavedFiles()) {
        event->ignore();
        return;
    }
	
    writeSettings();    /* save current settings */
    event->accept();
}

/**
 * Exit from application.
 */

void Ui_MainWindow::quit()
{
	this->close();
}

void Ui_MainWindow::print_msg()
{
		cout << "hovered" << endl;
}

/**
 * Create main window actions.
 */

void Ui_MainWindow::createActions()
{
    actionSave = new QAction(this);
    actionSave->setObjectName(QString::fromUtf8("actionSave"));
    
    actionOpen = new QAction(this);
    actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
    
    actionNew = new QAction(this);
    actionNew->setObjectName(QString::fromUtf8("actionNew"));
    
    actionQuit = new QAction(this);
    actionQuit->setObjectName(QString::fromUtf8("actionQuit"));

	/* recent files menu */
    menu_recent_files = new recent_files(tr("Recent Files"), this);
    connect(menu_recent_files, SIGNAL(open_recent_file(const QString &)), &view_manager_, SLOT(open_file(const QString &)));
    
    /* menu side bar */
    actionSide_Bar = new QAction(this);
    actionSide_Bar->setObjectName(QString::fromUtf8("actionSide_Bar"));
    actionSide_Bar->setCheckable(true);
    actionSide_Bar->setChecked(true);	// default "checked"
    actionSide_Bar->setStatusTip(tr("Show or hide side bar"));
    
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

void Ui_MainWindow::destroy_actions()
{
    delete action_find;
    delete actionGo_to_line;
    delete actionSrcTabBar;
    delete actionFullScreen;
    delete action_split_horizontally;
    delete action_split_vertically;
    delete action_unsplit;
    delete actionMenuBar;
    delete actionStatus_Bar;
    delete actionSide_Bar;
    delete menu_recent_files;
    delete actionQuit;
    delete actionNew;
    delete actionOpen;
    delete actionSave;
}

void Ui_MainWindow::create_menu()
{

}

void Ui_MainWindow::destroy_menu()
{

}

/**
 * Translate main window interface.
 */

void Ui_MainWindow::retranslateUi(QMainWindow *main_window)
{
	main_window->setWindowTitle(QApplication::translate("main_window", "lgmx", 0, QApplication::UnicodeUTF8));
	actionSave->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
	actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
	actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
	actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
	actionNew->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
	actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
	
	actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
	actionQuit->setShortcut(QApplication::translate("MainWindow", "Alt+F4", 0, QApplication::UnicodeUTF8));
	
	actionSide_Bar->setText(QApplication::translate("main_window", "Side Bar", 0, QApplication::UnicodeUTF8));
	actionSide_Bar->setShortcut(QApplication::translate("main_window", "Alt+X", 0, QApplication::UnicodeUTF8));
	
	actionStatus_Bar->setText(QApplication::translate("main_window", "Status Bar", 0, QApplication::UnicodeUTF8));
	actionStatus_Bar->setShortcut(QApplication::translate("main_window", "Alt+Z", 0, QApplication::UnicodeUTF8));
	
	actionMenuBar->setText(QApplication::translate("main_window", "Menu Bar", 0, QApplication::UnicodeUTF8));
	actionMenuBar->setShortcut(QApplication::translate("main_window", "Alt+C", 0, QApplication::UnicodeUTF8));
	actionMenuBar->setShortcutContext(Qt::ApplicationShortcut);
	
	actionFullScreen->setText(QApplication::translate("main_window", "Full Screen", 0, QApplication::UnicodeUTF8));
	actionFullScreen->setShortcut(QApplication::translate("main_window", "F11", 0, QApplication::UnicodeUTF8));
	actionFullScreen->setShortcutContext(Qt::ApplicationShortcut);
	
	action_split_horizontally->setText(QApplication::translate("main_window", "Split Horizontally", 0, QApplication::UnicodeUTF8));
	action_split_horizontally->setShortcut(QApplication::translate("main_window", "Ctrl+E, 2", 0, QApplication::UnicodeUTF8));
	action_split_horizontally->setShortcutContext(Qt::ApplicationShortcut);
	
	action_split_vertically->setText(QApplication::translate("main_window", "Split Vertically", 0, QApplication::UnicodeUTF8));
	action_split_vertically->setShortcut(QApplication::translate("main_window", "Ctrl+E, 3", 0, QApplication::UnicodeUTF8));
	action_split_vertically->setShortcutContext(Qt::ApplicationShortcut);
	
	action_unsplit->setText(QApplication::translate("main_window", "Unsplit", 0, QApplication::UnicodeUTF8));
	action_unsplit->setShortcut(QApplication::translate("main_window", "Ctrl+E, 0", 0, QApplication::UnicodeUTF8));
	action_unsplit->setShortcutContext(Qt::ApplicationShortcut);
	
	actionSrcTabBar->setText(QApplication::translate("main_window", "Source Tab Bar", 0, QApplication::UnicodeUTF8));
	actionSrcTabBar->setShortcut(QApplication::translate("main_window", "Alt+3", 0, QApplication::UnicodeUTF8));
	actionSrcTabBar->setShortcutContext(Qt::ApplicationShortcut);
	
	actionGo_to_line->setText(QApplication::translate("main_window", "Go to line", 0, QApplication::UnicodeUTF8));
	actionGo_to_line->setShortcut(QApplication::translate("main_window", "Ctrl+L", 0, QApplication::UnicodeUTF8));
	
	action_find->setText(QApplication::translate("main_window", "Find", 0, QApplication::UnicodeUTF8));
	action_find->setShortcut(QApplication::translate("main_window", "Ctrl+F", 0, QApplication::UnicodeUTF8));
	
	symbol_tab_widget->setTabText(symbol_tab_widget->indexOf(tab), QApplication::translate("main_window", "Tab 1", 0, QApplication::UnicodeUTF8));
	symbol_tab_widget->setTabText(symbol_tab_widget->indexOf(tab_2), QApplication::translate("main_window", "Tab 2", 0, QApplication::UnicodeUTF8));

	menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
	menuView->setTitle(QApplication::translate("main_window", "&View", 0, QApplication::UnicodeUTF8));
	menu_Search->setTitle(QApplication::translate("main_window", "&Search", 0, QApplication::UnicodeUTF8));
} // retranslateUi

