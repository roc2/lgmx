#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <QWidget>
#include <list>
#include <stack>
#include <set>

#include "view.h"
#include "recent_files.h"
#include "id.h"
#include <plugin_manager.h>

class QSplitter;

class view_manager : public QWidget
{
	Q_OBJECT
	
public:
	view_manager(QWidget *parent = 0, file_type *type_manager = 0);
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

	unsigned int generate_view_id();
	void release_view_id(unsigned int id);

private:
	void close_file(QTextDocument *content);
	//void build_close_file_msg(int index, QString &msg);
	
	bool save_file_as(src_container *src_c, int index);
	bool save_file(src_container *src_c, const QString &fileName, int index);
	
	int get_root_src_container_file_index(QTextDocument *content);
	
	void add_to_splitter_list(QSplitter *s);
	void remove_from_splitter_list(QSplitter *s);

	bool new_file(const QString &file_name);
	
	void set_view_properties(view &old_view, view &new_view);

public slots:
	void split_horizontally();
	void split_vertically();
	void split(Qt::Orientation orientation);
	void unsplit();

	void close_file(int index);
	void new_file();
	void open_file();
	void open_file(const QString &file_name);
	void reload_current_file();
	bool save();
	
	void show_src_tab_bar(bool show);

private slots:
	//void open_recent_file();

signals:
    //void open_recent_file(QString &);

private:
    std::list<view *> view_list_;	/**< list of pointers to all existent views. root view is always at the beginning */
    std::list<QSplitter*> view_splitters_;
    
    view *root_view_;
    view *current_view_;
    src_container *root_container_;
    
    file_type *type_manager_;
    plugin_manager plugin_manager_;
    
    std::set<QString> open_files_; /**< current open files */
    recent_files *recent_files_;
    
    QVBoxLayout *layout_;
    int m_num_splits;

    Id file_id_;	/**< source file ID generator */
    Id view_id_;	/**< view ID generator */ 
};

#endif
