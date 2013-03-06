#ifndef SRC_CONTAINER_H
#define SRC_CONTAINER_H

#include <QtGui/QTabWidget>
#include <QTabBar>
#include <QFont>

#include <src_file.h>


#define NEW_FILE_NAME	"untitled"

class QVBoxLayout;
class view_manager;
class Settings;

class src_container : public QTabWidget
{
	Q_OBJECT
	
public:
	src_container(view_manager *manager, Settings *settings, QWidget *parent = 0);
	~src_container();
	
	int new_src_tab(const QString &file_name, unsigned int file_id);
	src_file* new_clone_tab(src_file *base_file);
	void destroy_src_tab(int index);
	void destroy_src_tab(src_file *file);
	void destroy_src_tab(unsigned int id);

	bool is_modified(int index);
    bool set_modified(int index, bool modified);
   
	bool set_file_name(int index, const QString &fileName);
    QString get_src_tab_short_name(int index);
    QString get_src_tab_path(int index);
	bool get_src_tab_full_name(int index, QString &file_name);
	
    bool update_file_info(int index);
	
    bool src_tab_write_file(int index, const QString &fileName);
    bool src_tab_load_file(const QString &fileName);
    
	bool get_src_tab_content(int index, QString &content);

	int get_current_tab_index();
	int index_of(src_file *src_tab) const;
	int get_index_of(src_file *src_tab);
	src_file *get_current_src_file();
	unsigned int get_src_file_id(int index);
	void set_current_src_file(unsigned int id);
	void set_next_src_file_as_current();
	src_file *get_src_file(int index);
	src_file *get_src_file(unsigned int id);
	
	void highlight_current_src_file();
	
	bool get_curr_font(int index, QFont &font);
	
    void show_tabs(bool show);
    bool tabs_visible();
	void go_to_line(int index, int line);
    
    int get_file_index(const QString &file_name);
    //void setFont(QFont &font);
    void set_line_wrap(bool wrap);
    bool get_line_wrap() const;
    
    void set_tab_width(int size);
	int get_tab_width() const;
	
	//void set_line_wrap(bool on);
    
    void focusInEvent(QFocusEvent *event);
    void update_current_view();
    
    Settings* get_settings();

public slots:
	void file_changed(bool changed);

private slots:
	void set_focus_to_current_tab(int index);

private:
    QTabBar *tab_bar;
    QVBoxLayout *main_layout_;
    QFont font;
    
    view_manager *manager_;
    Settings *settings_;
};

#endif

