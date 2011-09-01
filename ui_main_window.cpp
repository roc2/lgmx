#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <cstdlib>
#include <QDir>
#include <QString>

#include "ui_main_window.h"
#include "editor.h"
#include "code_editor.h"
#include "config.h"
#include "unsvFileDialog.h"

#include "string"

using namespace std;


/**
 * Closes a specific source file (tab).
 * @param index -> tab index in the tab widget
 */

void Ui_MainWindow::close_file(int index)
{
    QString file_name, msg;
	QMessageBox::StandardButton ret;
	set<QString>::iterator it;
    
    file_name = src_files.get_src_tab_full_name(index);
    
    /* check if file needs to be saved */
	if (src_files.is_modified(index)) {
		
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
	
	src_files.destroy_src_tab(index);   /* closes the file */
    
    it = open_files.find(file_name);    /* pull out of open files list */
    if (it != open_files.end())
        open_files.erase(*it);
}

/**
 * Save file to disk
 * @brief Save file to disk
 */

bool Ui_MainWindow::save()
{
    QString file_name;
    int index;
    
    index = src_files.get_current_tab_index();  /* get current file index */
    
    if (index < 0)      /* no file open */
        return false;
    
    file_name = src_files.get_src_tab_full_name(index);
    
    if (file_name.isEmpty()) {
        cout << "file name empty" << endl;
        return saveAs(index);
    }
    
    return saveFile(file_name, index);
}

/**
 * Save file as. This function saves a file which was not written on the 
 * disk yet.
 * @brief Save file as
 * 
 */

bool Ui_MainWindow::saveAs(int index)
{
    QString fileName = QFileDialog::getSaveFileName(this);
    // mudar para abrir na home do user se o arquivo nao existe:
    //files = QFileDialog::getSaveFileName(this, tr("Save File"), path, tr("All files (*.c *.cpp *.h)"));
    
    if (fileName.isEmpty())
        return false;   /* no file specified */

    if (saveFile(fileName, index)) {
        src_files.set_file_name(index, fileName);
        src_files.update_file_info(index);
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

    if (!src_files.src_tab_write_file(index, fileName))
        return false;

    src_files.set_modified(index, false);
    open_files.insert(fileName);

    return true;
}

/**
 * Open a file
 */

void Ui_MainWindow::open_file()
{
    QStringList files;
	QString path;
    QDir dir;
	int index, size;
    
    index = src_files.get_current_tab_index();  /* get current file index */
    
    /* 
     * "open file" dialog path is the path of the current open file, or "home"
     * if there is no file open
     */
    if (index < 0 || (path = src_files.get_src_tab_path(index)) == "")
        path = dir.homePath();
 
    files = QFileDialog::getOpenFileNames(this, tr("Open File"), path, tr("All files (*.c *.cpp *.h)"));

    size = files.size();
    for (index = 0; index < size; ++index) {
        if (open_files.find(files.at(index)) == open_files.end()) {
            if (src_files.new_src_tab(files.at(index)) < 0)
                continue;

            open_files.insert(files.at(index));
        } else {
            cout << "file already open" << endl;
            index = this->get_file_index(files.at(index));
            if (index >= 0)
                this->set_current_index(index);
        }
    }
}

void Ui_MainWindow::set_current_index(int index)
{
    if (index >= src_files.count())
        return; /* index out of range */
    
    src_files.setCurrentIndex(index);
}

int Ui_MainWindow::get_file_index(const QString &file_name)
{
    int index;
    int count = src_files.count();
    
    for (index = 0; index < count; index++) {
        if (file_name == src_files.get_src_tab_full_name(index))
            return index;
    }
    
    return -1;
}

/**
 * Opens files passed as parameters
 * @brief Opens files passed as parameters
 */

void Ui_MainWindow::load_parameter_files(list<QString> *files)
{
    QDir dir;
    QString curPath = dir.currentPath();
    QString file;
    
    for (list<QString>::iterator it = files->begin(); it != files->end(); it++) {
        if ((*it)[0] != '/') {  // append file path
            file = curPath;
            file.append('/');
            file.append(*it);
            src_files.new_src_tab(file);    // fazer com que new file não crie o arquivo caso não encontre
        } else
            src_files.new_src_tab(*it);    // fazer com que new file não crie o arquivo caso não encontre
    }
}

/**
 * Create new empty file
 */

void Ui_MainWindow::new_file()
{
    QString fileName = "";
    
	src_files.new_src_tab(fileName);
}

/**
 * 
 */

void Ui_MainWindow::set_font()
{
	QFont initial;
    
	initial.setFamily("monospace");
	initial.setFixedPitch(true);
	initial.setPointSize(12);

    cout << "will set font..." << endl;
    src_files.setFont(initial);
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
	
	src_files.get_curr_font(0, font);
	
	if ((size = font.pixelSize()) < 0) {
		//size = font.pointSize();
		size = font_metrics.width('a');
	}
	cout << "size = " << size << endl;
}

/**
 * Returns the user home directory absolute path
 * @brief Returns the user home directory absolute path
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
 * Show or hide the source files tab bar
 * @brief Show or hide the source files tab bar
 * @param show -> true, show menu; false, hide tab bar
 */

void Ui_MainWindow::show_src_tab_bar(bool show)
{
	src_files.show_tabs(show);
}

/**
 * Go to specified line
 * @brief Go to specified line
 */

void Ui_MainWindow::go_to_ln()
{
	int line = 0;
	int index;
	
	index = src_files.get_current_tab_index();
	if (index < 0)
		return;
	
	if (!gt_ln_dialog) {
		cout << "new go to line object" << endl;
		gt_ln_dialog = new go_to_line;
	}
	
	gt_ln_dialog->regular_size();
	if (gt_ln_dialog->exec() == 1) {
		line = gt_ln_dialog->get_line();
		
		if (line <= 0)
			return;
		
		src_files.go_to_line(index, line);
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

	//settings.setValue("geometry", saveGeometry());
    //settings.setValue("windowState", saveState());
    
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


}

/**
 * Constructs the message to warn the user the file has non saved changes
 * @brief Constructs the message to warn the user the file has non saved changes
 * @param index -> tab index
 * @param msg -> string to hold the message
 */

void Ui_MainWindow::build_close_file_msg(int index, QString &msg)
{
    QString file = src_files.get_src_tab_short_name(index);
            
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
    
    tabs = src_files.count();
    
    /* check for unsaved files and save them if requested */
    for (index = 0; index < tabs; index++) {

        if (src_files.is_modified(index)) {
            src_files.setCurrentIndex(index);
            
            this->build_close_file_msg(index, msg);
            
            ret = QMessageBox::warning(this, APPLICATION, msg,
			      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            
            if (ret == QMessageBox::Save) { /* save file */
            
                file_name = src_files.get_src_tab_full_name(index);
                
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
 * Create main window actions
 * @brief Create main window actions
 */

void Ui_MainWindow::createActions()
{
    actionSave = new QAction(this);
    actionSave->setObjectName(QString::fromUtf8("actionSave"));
    
    actionOpen = new QAction(this);
    actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
    
    actionNew = new QAction(this);
    actionNew->setObjectName(QString::fromUtf8("actionNew"));
    
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
    
    /* go to line */
    actionGo_to_line = new QAction(this);
    actionGo_to_line->setObjectName(QString::fromUtf8("actionGo_to_line"));
}

/**
 * MainWindow Constructor
 * @brief MainWindow Constructor
 */


Ui_MainWindow::Ui_MainWindow()
{
    if (objectName().isEmpty())
            setObjectName(QString::fromUtf8("main_window"));
        //resize(831, 557);
        //showMaximized();

        gt_ln_dialog = NULL;
        
        createActions();
        
        centralWidget = new QWidget(this);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        
        splitter = new QSplitter(centralWidget);
        //splitter->setHandleWidth(2);
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
        splitter->addWidget(&src_files);
        
        horizontalLayout->addWidget(splitter);
        
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
        
        menuView->addSeparator();
        menuView->addAction(actionSide_Bar);
        menuView->addAction(actionStatus_Bar);
        menuView->addAction(actionMenuBar);
        menuView->addAction(actionFullScreen);
        menuView->addAction(actionSrcTabBar);
        
        /* add actions to main window, so they work when menuBar is hidden */
        addAction(actionNew);
		addAction(actionSave);
        addAction(actionOpen);
        
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
        addAction(actionGo_to_line);
        
        retranslateUi(this);
		QObject::connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
        QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(open_file()));
        QObject::connect(actionNew, SIGNAL(triggered()), this, SLOT(new_file()));
        
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
        
		/* go to line */
		QObject::connect(actionGo_to_line, SIGNAL(triggered()), this, SLOT(go_to_ln()));

        QObject::connect(&src_files, SIGNAL(tabCloseRequested ( int )), this, SLOT(close_file(int)));

        QMetaObject::connectSlotsByName(this);
        
        set_font();
        readSettings();
        //Config conf;
}

/**
 * MainWindow Constructor with file list as parameter
 * @brief MainWindow Constructor
 */


Ui_MainWindow::Ui_MainWindow(list<QString> *files)
{
    
    
    if (objectName().isEmpty())
            setObjectName(QString::fromUtf8("main_window"));
        //resize(831, 557);
        //showMaximized();

        gt_ln_dialog = NULL;
        
        createActions();
        
        centralWidget = new QWidget(this);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        
        splitter = new QSplitter(centralWidget);
        //splitter->setHandleWidth(2);
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
        splitter->addWidget(&src_files);
        
        horizontalLayout->addWidget(splitter);
        
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
        
        menuView->addSeparator();
        menuView->addAction(actionSide_Bar);
        menuView->addAction(actionStatus_Bar);
        menuView->addAction(actionMenuBar);
        menuView->addAction(actionFullScreen);
        menuView->addAction(actionSrcTabBar);
        
        /* add actions to main window, so they work when menuBar is hidden */
        addAction(actionNew);
		addAction(actionSave);
        addAction(actionOpen);
        
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
        addAction(actionGo_to_line);
        
        retranslateUi(this);
		QObject::connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
        QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(open_file()));
        QObject::connect(actionNew, SIGNAL(triggered()), this, SLOT(new_file()));
        
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
        
		/* go to line */
		QObject::connect(actionGo_to_line, SIGNAL(triggered()), this, SLOT(go_to_ln()));

        QObject::connect(&src_files, SIGNAL(tabCloseRequested ( int )), this, SLOT(close_file(int)));

        QMetaObject::connectSlotsByName(this);
        
        set_font();
        readSettings();
        
        if (!files->empty()) {
            this->load_parameter_files(files);
        
    }
        //Config conf;
}


