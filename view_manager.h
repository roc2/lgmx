#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <QWidget>
#include <list>
#include <stack>
#include <set>

#include "view.h"
#include "recent_files.h"
#include "id.h"

class view_manager : public QWidget
{
	Q_OBJECT
	
public:
	view_manager(QWidget *parent = 0);
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

	void add_visible_view(const view *v);
	void remove_visible_view(unsigned int id);

private:
	void close_file(QTextDocument *content);
	
	bool save_file_as(src_container *src_c, int index);
	bool save_file(src_container *src_c, const QString &fileName, int index);
	
	int get_root_src_container_file_index(QTextDocument *content);

public slots:
	void split_horizontally();
	void split_vertically();
	void unsplit();
	void close_file(int index);
	void new_file();
	void open_file();
	void open_file(const QString &file_name);
	void reload_current_file();
	bool save();

private slots:
	//void open_recent_file();

signals:
    //void open_recent_file(QString &);

private:
    list<view *> view_list_;	/**< list of pointers to all existent views. root view is always at the beginning */
    std::map<unsigned int, const view *> visible_views_;	/**< pointers to all visible views */
    view *root_view_;
    view *current_view_;
    src_container *root_container_;
    
    set<QString> open_files_; /**< current open files */
    recent_files *recent_files_;
    
    QVBoxLayout *layout_;
    int num_splits_;

	// source file ID
    Id file_id_;
    Id view_id_;
};

#endif
