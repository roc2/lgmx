#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include <QWidget>
#include <list>

#include "view.h"

class view_manager : public QWidget
{
	Q_OBJECT
	
public:
	view_manager(QWidget *parent = 0);
	~view_manager();

	void add_to_view_list(view *v);
	void remove_from_view_list(view *v);
	
	src_container* get_root_src_container();
	src_container* get_current_src_container();
	
	view* get_root_view() const;
	view* get_current_view() const;

	void set_current_view(view* curr_view);

private:
	void close_file(QTextDocument *content);

public slots:
	void split_horizontally();
	void split_vertically();
	void unsplit();
	void close_file(int index);
	void new_file();

private slots:
	//void open_recent_file();

signals:
    //void open_recent_file(QString &);

private:
    list<view *> view_list_;	/**< list of pointers to all existent views. root view is always at the beginning */
    view *root_view_;
    view *current_view_;
    
    QVBoxLayout *layout_;
};

#endif
