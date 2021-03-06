#ifndef VIEW_H
#define VIEW_H

#include <QWidget>



class view_manager;
class view;
class status_line;
class src_container;
class src_file;
class QVBoxLayout;
class QFont;


class view : public QWidget
{
	Q_OBJECT

private:
	view(const view&);
	view& operator=(const view&);

public:
	view(view_manager *manager, QWidget *parent = NULL);
	~view();

	void new_visual_file(src_file *file);
	void destroy_src_file(unsigned int id);
	
	void show_src_tab_bar(bool show);
	bool src_tab_bar_visible();
	void show_status_bar(bool show);
	bool status_bar_visible();
	void update_status_bar(const QString &fileName, unsigned int id);
	
	view_manager* get_view_manager() const;
	src_container* get_src_container() const;
	//void focusInEvent(QFocusEvent *event);
	
	unsigned int get_id() const;
	void set_font(QFont &font);
	
	void clone_src_container(src_container *base_src_ctr);

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
