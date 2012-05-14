#include <iostream>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include "src_container.h"


/*
 * criar a classe src_container q vai ter um qtabwidget.
 * Criar uma classe que vai ter o CodeEditor, e file properties.
 * O construtor dessa classe cria um CodeEditor e um file properties, e
 * adiciona o CodeEditor no layout.
 * A classe src_container vai ter um metodo "new_tab" que vai addTab("classe criada");
 */


/**
 * Constructor
 */

src_container::src_container(QWidget *parent) : QTabWidget(parent)
{
	_parent = parent;
	
    setObjectName(QString::fromUtf8("src_tab_widget"));
	setTabsClosable(true);
	setMovable(true);
	setContentsMargins(0, 0, 0, 0);

    tab_bar = tabBar();
    tab_bar->setContentsMargins(0, 0, 0, 0);
    
    //this->setFocusPolicy(Qt::StrongFocus);
    
    tab_bar->setStyleSheet("border-width: 0px;");
    
    setStyleSheet("border-width: 0px;");
    setStyleSheet("QTabBar::tab { height: 25px; }");
}

/**
 * Copy constructor.
 */

/**
 * @todo fix this copy constructor
 */

src_container::src_container(const src_container &copy) : QTabWidget(copy.get_parent())
{
	_parent = copy.get_parent();
	
	setObjectName(QString::fromUtf8("src_tab_widget"));
	setTabsClosable(true);
	setMovable(true);

    tab_bar = tabBar();
    
    tab_bar->setStyleSheet("border-width: 0px;");
    
    setStyleSheet("border-width: 0px;");
    setStyleSheet("QTabBar::tab { height: 25px; }");
}

/**
 * Destructor.
 */

src_container::~src_container()
{
	int count = this->count();
	
	for (int i = 0; i < count; i++)
		destroy_src_tab(i);
}

void src_container::focusInEvent(QFocusEvent *event)
{
	cout << "this Tab widget has focus!!" << endl;
}

/**
 * Create new source tab.
 */

int src_container::new_src_tab(const QString &file_name)
{
	int index;
	src_file *src_tab;
	QString show_name;
	string s_name;

	try {
		index = addTab(new src_file(file_name), "");
		//cout << "added new tab at index " << index << endl;
	} catch(...) {
		
		return -1;
	}

	if ((src_tab = static_cast<src_file *>(this->widget(index))) == 0)
    	return -1;	/* index out of range */

    if (file_name.isEmpty())
        show_name = NEW_FILE_NAME;
    else
        show_name = src_tab->get_src_file_name();

    s_name = show_name.toStdString();

    setTabText(index, QApplication::translate("main_window", s_name.c_str(), 0, QApplication::UnicodeUTF8));
	setCurrentIndex(index);

	/*
	 * ligar o sinal void QPlainTextEdit::textChanged () [signal] em
	 * um slot que troca a cor do texto da tab por vermelho
	 */

	return index;
}

/**
 * 
 */

QWidget* src_container::get_parent() const
{
	return _parent;
}

/**
 * This is just a wrapper, currentIndex() may be used directly.
 */

int src_container::get_current_tab_index()
{
	return currentIndex();
}

/**
 * Current source file.
 * @return pointer to the current source file, NULL pointer if there 
 * is no current file.
 */

src_file *src_container::get_current_src_file()
{
	return static_cast<src_file *>(currentWidget());
}

/**
 * 
 */

src_file *src_container::get_src_file(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return 0;	/* index out of range */
		
	return src_tab;
}


/**
 * Removes the specified tab from the tab widget.
 * @param index -> tab index
 */

void src_container::destroy_src_tab(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return;	/* index out of range */
	
	// removeTab nao destroi o widget!!!
	// precisa pegar o endereço do widget, remover e depois destruir!!
	removeTab(index);
	
	delete src_tab;
	cout << "destroyed tab at index " << index << endl;
}

/**
 *
 */

bool src_container::get_src_tab_full_name(int index, QString &file_name)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	//file_name = src_tab->get_src_file_full_name();
	src_tab->get_src_file_full_name(file_name);
	return true;
}

/**
 * 
 */

QString src_container::get_src_tab_short_name(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return "";	/* index out of range */

	return src_tab->get_src_file_name();
}

/**
 * 
 */

QString src_container::get_src_tab_path(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return "";	/* index out of range */

	return src_tab->get_src_file_path();
}

/**
 * Retrieves the content of the entire file
 */

bool src_container::get_src_tab_content(int index, QString &content)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	content = src_tab->get_content();

	return true;
}

/**
 * 
 * 
 */

bool src_container::src_tab_write_file(int index, const QString &fileName)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	return src_tab->write_file(fileName);
}

/**
 * Check if file content was modified
 * @brief Check if file content was modified
 * @param index -> tab index
 * @return true -> file modified, false -> not modified or invalid index
 */

bool src_container::is_modified(int index)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

    cout << "is modified index " << index << endl;
	return src_tab->is_modified();
}

/**
 * 
 */
#if 0
bool src_container::exists(int index)
{
	src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

	return src_tab->exists();
}


bool src_container::saved_on_disk(int index)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	return src_tab->saved_on_disk();

}
#endif

bool src_container::set_modified(int index, bool modified)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	return src_tab->set_src_file_modified(modified);
}

bool src_container::set_file_name(int index, QString &fileName)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	src_tab->set_src_file_name(fileName);
    
    return true;
}

/**
 * 
 */

//void src_container::setFont(QFont &font)
//{
    //this->setFont(font);
//}

/**
 * Updates the information from a file already saved on disk
 * @brief Updates the information from a file already saved on disk
 * @param index -> tab index
 * @return true -> info updated ok, false -> invalid index
 */

bool src_container::update_file_info(int index)
{
    src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return false;	/* index out of range */

	src_tab->update_src_file_info();    /* refresh file info */
    
    /* set tab text with the short file name */
    setTabText(index, src_tab->get_src_file_name());
    
    return true;
}

bool src_container::get_curr_font(int index, QFont &font)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
  //  if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

	font = src_tab->get_font();
	return true;
}

/**
 * 
 */

void src_container::go_to_line(int index, int line)
{
	src_file *src_tab;

    if ((src_tab = static_cast<src_file *>(widget(index))) == 0)
		return;		/* index out of range */

	cout << index << " " << line << endl;

	src_tab->go_to_line(line);

}

/**
 * Show or hide the source files tab bar.
 * @param show -> true, show tabs; false, hide tabs
 */

void src_container::show_tabs(bool show)
{
	if (show)
        tab_bar->show();
	else
        tab_bar->hide();
}

/**
 * 
 */

int src_container::get_file_index(const QString &file_name)
{
	int index;
    int count = this->count();
    QString file;
    
    for (index = 0; index < count; index++) {
        //if (file_name == this->get_src_tab_full_name(index))
        this->get_src_tab_full_name(index, file);
        if (file_name == file)
            return index;
    }
    
    return -1;
}



