#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QSplitter>
#include <QStackedLayout>

#include "src_container.h"

#define nullptr	NULL

class view_manager;
class view;


class view : public QWidget
{
	Q_OBJECT
	
public:
	view(view_manager *manager, QWidget *parent = NULL);
	~view();
	
	void set_src_container(src_container *container);
	
	src_container* get_src_container() const;
	
	int new_file(const QString &file_name, unsigned int file_id);
	QVBoxLayout *get_main_layout();
	
	void clone_file(src_file *file, int index);
	
	void destroy_src_file(unsigned int id);
	
	void show_src_tab_bar(bool show);
	
	view_manager* get_view_manager() const;
	//void focusInEvent(QFocusEvent *event);
	
	unsigned int get_id() const;
	
	void clone_src_container(src_container *base_src_ctr, int index);

private:

	//void update_menu();
	//void mousePressEvent(QMouseEvent *event);

signals:
    //void open_recent_file(QString &);

private slots:
	//void open_recent_file();

private:
    QVBoxLayout *main_layout_;
    src_container *src_container_;
    status_line *status_line_;
    view_manager *manager_;
    unsigned int id_;
};

#endif
