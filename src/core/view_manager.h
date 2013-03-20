#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <QWidget>
#include <QPointer>

#include <list>
#include <set>

#include <recent_files.h>
#include <id.h>
#include <plugin_manager.h>
#include <settings.h>
#include <tags.h>


class QSplitter;
class QTextDocument;
class QVBoxLayout;
class highlight_manager;
class view;
class src_container;
class src_file;

class view_manager : public QWidget
{
	Q_OBJECT

private:
	view_manager();
	view_manager(const view_manager&);
	view_manager& operator=(const view_manager&);

public:
	view_manager(QWidget *parent, file_type *type_manager, Settings *settings);
	~view_manager();

	void add_to_view_list(view *v);
	void remove_from_view_list(view *v);
	void set_current_view(view* curr_view);
	void set_recent_files_widget(recent_files *recent_files_widget);
	void set_current_file_index(int index);

	view* get_root_view() const;
	view* get_current_view() const;
	src_container* get_root_src_container() const;
	src_container* get_current_src_container() const;
	int get_current_file_index(const QString &file_name);
	src_file* get_current_src_file() const;

	void set_tab_width(int size);
	int get_tab_width() const;
	void set_line_wrap(bool wrap);
	bool get_line_wrap() const;

	QString get_version_number();

	unsigned int generate_view_id();
	void release_view_id(unsigned int id);
	
	bool check_unsaved_files();
	highlight_manager* get_highlight_manager();
	
	Settings* get_settings();

private:
	void close_file(QTextDocument *content);
	void close_file(src_container *container, src_file *src_tab, int index);
	
	bool save_file_as(src_container *src_c, int index);
	bool save_file(src_container *src_c, const QString &fileName, int index);
	
	void add_to_splitter_list(QSplitter *s);
	void remove_from_splitter_list(QSplitter *s);

	bool new_file(const QString &file_name);
	
	view* create_view(QWidget *parent);
	void destroy_view(view* v);
	void clear_view_list();
	void clear_splitter_list();
	
	void set_view_properties(view &old_view, view &new_view);
	void update_status_bar(const QString &fileName, unsigned int id);

public slots:
	void split_horizontally();
	void split_vertically();
	void split(Qt::Orientation orientation);
	void unsplit();
	void remove_all_splits();

	void close_file(int index);
	void close_file();
	void new_file();
	void open_file();
	void open_file(const QString &file_name);
	void reload_current_file();
	bool save();
	
	void show_src_tab_bar(bool show);
	void show_status_bar(bool show);
	void set_next_file_as_current();
	void go_to_tag();

private slots:

signals:

private:
    std::list<view *> view_list_;	/**< list of pointers to all existent views */
    std::list<QSplitter*> view_splitters_;
    
    QPointer<view> curr_view_;
    src_container *root_container_;
    Settings *settings_;
    
    file_type *type_manager_;
    plugin_manager plugin_manager_;
    highlight_manager *highlight_manager_;
    
    std::set<QString> open_files_; /**< current open files */
    recent_files *recent_files_;
    
    QVBoxLayout *layout_;
    int num_splits_;

    Id file_id_;	/**< source file ID generator */
    Id view_id_;	/**< view ID generator */
    
    tag *tag_;
};

#endif
