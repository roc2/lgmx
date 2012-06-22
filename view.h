#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QSplitter>
#include <QStackedLayout>

#include "src_container.h"

#define nullptr	0

class view_manager;
class view;

class ctr_wrapper : public QWidget
{
public:

	ctr_wrapper(view *parent = 0);
	~ctr_wrapper() {}
	QVBoxLayout *main_layout_;
    src_container *src_container_;
    status_line *status_line_;
};

class view : public QWidget
{
	Q_OBJECT
	
public:
	view(view_manager *manager, view *parent = 0, bool root = false);
	view(const view &copy);
	~view();
	
	void split(Qt::Orientation orientation);

	void set_src_container(src_container *container);
	void set_parent(view *parent);
	
	src_container* get_src_container() const;
	QSplitter* get_splitter();
	
	int new_file(const QString &file_name);
	QVBoxLayout *get_main_layout();
	
	bool is_splitted() const;
	bool is_root() const;
	
	void unsplit(view *to_be_destroyed);
	
	void show_src_tab_bar(bool show);
	
	view* get_parent_view() const;
	view_manager* get_view_manager() const;
	//void focusInEvent(QFocusEvent *event);
	
	view* take_child_view(int index, view *new_parent);

private:

	void clone_file(src_file *file);
	void clone_src_container(src_container *base_src_ctr);
	//void update_menu();
	//void mousePressEvent(QMouseEvent *event);

signals:
    //void open_recent_file(QString &);

private slots:
	//void open_recent_file();

private:
    QSplitter *splitter_;
    view *child_view_[2];
    view *parent_;
    QStackedLayout *layout_;
    QVBoxLayout *main_layout_;
    src_container *src_container_;
    status_line *status_line_;
    view_manager *manager_;
    ctr_wrapper *wrapper_;
    
    bool root_;
    bool splitted_;
};

#endif
