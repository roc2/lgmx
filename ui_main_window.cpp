#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <cstdlib>
#include <QDir>

#include "ui_main_window.h"
#include "editor.h"
#include "code_editor.h"
#include "file.h"
#include "config.h"

using namespace std;


/**
 * Closes a specific source file (tab).
 * @param index -> tab index in the tab widget
 */

void Ui_MainWindow::close_file(int index)
{
    QString file_name;
	QMessageBox::StandardButton ret;
	
    /* check if file needs to be saved */
	if (src_files.is_modified(index)) {
		
        ret = QMessageBox::warning(this, tr("Application"),
			  tr("The document has been modified.\n"
				 "Do you want to save your changes?"),
			  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	
		if (ret == QMessageBox::Save) { /* save file */
            
            file_name = src_files.get_src_tab_full_name(index);
            
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
	
	src_files.destroy_src_tab(index);   // closes the file
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

    if (!src_files.src_tab_write_file(index, fileName)) {
        QMessageBox::warning(this, tr("Application"), tr("Cannot write file %1:\n%2.")
                                 .arg(fileName) .arg(error));
        return false;
    }

    src_files.set_modified(index, false);

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
	int index;
    
    index = src_files.get_current_tab_index();  /* get current file index */
    
    /* 
     * "open file" dialog path is the path of the current open file, or "home"
     * if there is no file open
     */
    if (index < 0 || (path = src_files.get_src_tab_path(index)) == "")
        path = dir.homePath();
 
    files = QFileDialog::getOpenFileNames(this, tr("Open File"), path, tr("All files (*.c *.cpp *.h)"));

    for (index = 0; index < files.size(); ++index)
        src_files.new_src_tab(files.at(index));
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
	bool ok;
	QFont initial;
	initial.setFamily("Courier");
	initial.setFixedPitch(true);
	initial.setPointSize(10);

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

	
	//editor->setFont(font);
}

void Ui_MainWindow::set_tab_width()
{
	QFont font;
	QFontMetrics font_metrics(font);
	int size;
	
	src_files.get_curr_font(font);
	
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
 * Show or hide the side bar.
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

void Ui_MainWindow::write_settings()
{
	QSettings settings(COMPANY, APPLICATION);

	//settings.setValue("geometry", geometry());
	settings.setValue("geometry", saveGeometry());
	cout << "Saved settings" << endl;
}

/**
 * Read and apply main window settings
 */

void Ui_MainWindow::read_settings()
{
	QSettings settings(COMPANY, APPLICATION);
	QRect rect;

	rect = settings.value("geometry", QRect(200, 200, 400, 400)).toRect();
	
	move(rect.topLeft());
	resize(rect.size());


}

/**
 * Closes the editor. Checks if there are unsaved changes and saves 
 * configuration before quiting.
 * @brief Closes the editor
 * @param event -> closing event
 */

void Ui_MainWindow::closeEvent(QCloseEvent *event)
{
    int cur_index;
    
	cout << "close event" << endl;
	
    while ((cur_index = src_files.get_current_tab_index()) >= 0) {
        close_file(cur_index);
    }
    
	if (okToContinue()) {
		write_settings();
		event->accept();
	} else {
		event->ignore();
	}
}


bool Ui_MainWindow::okToContinue()
{
	int ret;
	
	if (isWindowModified()) {
		ret = QMessageBox::warning(this, tr("Spreadsheet"),
		tr("The document has been modified.\n"
		"Do you want to save your changes?"),
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No,
		QMessageBox::Cancel | QMessageBox::Escape);
		
		if (ret == QMessageBox::Yes) {
			//return save();
			return true;
		} else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
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
 * 
 * 
 */


Ui_MainWindow::Ui_MainWindow()
{
    if (objectName().isEmpty())
            setObjectName(QString::fromUtf8("main_window"));
        resize(831, 557);
        showMaximized();

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
        //src_tab_widget = new QTabWidget(splitter);
        //src_tab_widget->setObjectName(QString::fromUtf8("src_tab_widget"));
        //src_tab_widget->setTabsClosable(true);
        //src_tab_widget->setMovable(true);
        
        splitter->addWidget(&src_files/*.get_widget()*/);
        
        horizontalLayout->addWidget(splitter);
        
        /* Configure splitter sizes. This must be called after the child widgets 
         * were inserted.
         */
        splitter_size.append(100);
        splitter_size.append(900);
        splitter->setSizes(splitter_size);
        //splitter_size = splitter->sizes();	/* this returns the current splitter sizes */

		//tabBar()->hide();		/* this hides the tab bar from a tabwidget */

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
        //statusBar->hide();	/* hide status bar */
        //statusBar->show();	/* show status bar */

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
        //QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(set_tab_width()));
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

		//QObject::connect(src_tab_widget, SIGNAL(tabCloseRequested ( int )), this, slot(closeTab(int))).
		//QObject::connect(src_files.get_widget(), SIGNAL(tabCloseRequested ( int )), this, SLOT(close_file(int)));
        QObject::connect(&src_files, SIGNAL(tabCloseRequested ( int )), this, SLOT(close_file(int)));

        //src_tab_widget->setCurrentIndex(0);
        //show_full_screen(true);

        QMetaObject::connectSlotsByName(this);
        
        Config conf;
}









