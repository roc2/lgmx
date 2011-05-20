#include <iostream>
#include "src_area.h"


/*
 * criar a classe src_area q vai ter um qtabwidget.
 * Criar uma classe que vai ter o CodeEditor, e file properties.
 * O construtor dessa classe cria um CodeEditor e um file properties, e
 * adiciona o CodeEditor no layout.
 * A classe src_area vai ter um metodo "new_tab" que vai addTab("classe criada");
 */


/**
 * Constructor
 */

src_area::src_area()
{
	//src_tab_widget = new QTabWidget;
	//src_tab_widget->setObjectName(QString::fromUtf8("src_tab_widget"));
	//src_tab_widget->setTabsClosable(true);
	//src_tab_widget->setMovable(true);
    //src_tab_widget->setStyleSheet("QTabBar::tab { height: 25px; }");


    setStyleSheet("border-width: 0px;");

    setObjectName(QString::fromUtf8("src_tab_widget"));
	setTabsClosable(true);
	setMovable(true);

    tab_bar = tabBar();
}


/**
 * Create new source tab
 */

int src_area::new_src_tab(QString content, QString file_name)
{
	int index;
	src_file *src_tab;
	QString show_name;
	string s_name;
    bool saved_on_disk = true;
    bool get_name = true;

    if (file_name == "") {
        show_name = NEW_FILE_NAME;
        get_name = false;
    }

	index = addTab(new src_file(file_name, content, saved_on_disk), QString());
	cout << "added new tab at index " << index << endl;

	if ((src_tab = (src_file *) this->widget(index)) == 0)
    	return false;	/* index out of range */

    if (get_name)
        show_name = src_tab->get_src_file_name();

    s_name = show_name.toStdString();

	//src_tab_widget->setTabText(index, QApplication::translate("main_window", s_name.c_str(), 0, QApplication::UnicodeUTF8));
	//src_tab_widget->setCurrentIndex(index);

    setTabText(index, QApplication::translate("main_window", s_name.c_str(), 0, QApplication::UnicodeUTF8));
	setCurrentIndex(index);

	/*
	 * ligar o sinal void QPlainTextEdit::textChanged () [signal] em
	 * um slot que troca a cor do texto da tab por vermelho
	 */

	return 0;
}

/**
 *
 */
#if 0
QTabWidget* src_area::get_widget()
{
	return src_tab_widget;
}
#endif
/**
 *
 */

int src_area::get_current_tab_index()
{
	return currentIndex();
}

/**
 * Removes the specified tab from the tab widget
 * @brief Removes the specified tab from the tab widget
 * @param index -> tab index
 */

void src_area::destroy_src_tab(int index)
{
	removeTab(index);
	cout << "destroyed tab at index " << index << endl;

    show_tabs(false);
}

/**
 *
 */

QString src_area::get_src_tab_full_name(int index)
{
	src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return "";	/* index out of range */

	return src_tab->get_src_file_full_name();
}

/**
 *
 */

bool src_area::get_src_tab_content(int index, QString &content)
{
	src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

	content = src_tab->get_content();

	return true;
}

/**
 * Check if file content was modified
 * @brief Check if file content was modified
 * @param index -> tab index
 * @return true -> file modified, false -> not modified or invalid index
 */

bool src_area::is_modified(int index)
{
	src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

    cout << "is modified index " << index << endl;
	return src_tab->is_modified();
}

bool src_area::exists(int index)
{
	src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

	return src_tab->exists();
}

bool src_area::saved_on_disk(int index)
{
    src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

	return src_tab->saved_on_disk();

}

bool src_area::set_saved_on_disk(int index, bool saved)
{
    src_file *src_tab;

	if ((src_tab = (src_file *) widget(index)) == 0)
		return false;	/* index out of range */

	return src_tab->set_saved_on_disk(saved);
}

bool src_area::get_curr_font(QFont &font)
{
	src_file *src_tab;

	//if ((src_tab = (src_file *) src_tab_widget->widget(0)) == 0)
    if ((src_tab = (src_file *) widget(0)) == 0)
		return false;	/* index out of range */

	font = src_tab->get_font();
	return true;
}

void src_area::go_to_line(int index, int line)
{
	src_file *src_tab;

	//if ((src_tab = (src_file *) src_tab_widget->widget(index)) == 0)
    if ((src_tab = (src_file *) widget(index)) == 0)
		return;		/* index out of range */

	cout << index << " " << line << endl;

	src_tab->go_to_line(line);

}

/**
 *
 */

void src_area::show_tabs(bool show)
{
	if (show)
        tab_bar->show();
	else
        tab_bar->hide();
}

