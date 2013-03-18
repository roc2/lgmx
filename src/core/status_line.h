#ifndef STATUS_LINE_H
#define STATUS_LINE_H

#include <QWidget>


class QComboBox;
class QHBoxLayout;
class src_container;

#define NO_NAME "--" NEW_FILE_NAME "--"

class status_line : public QWidget
{
	Q_OBJECT

private:
	status_line();
	status_line(const status_line&);
	status_line& operator=(const status_line&);

public:
	status_line(src_container *container);
	~status_line();
	
	void update_file_name(const QString &fileName, unsigned int id);

private:
	void set_default_colors();

signals:

public slots:	
	void add_file(const QString &file_name, unsigned int id);
	void remove_file(unsigned int id);
	void update_file_name(int index);
	void set_src_container(src_container *container);

private slots:
	void current_file_changed(int index);

private:
	QComboBox *file_list_;
	src_container *src_container_;
	
	QHBoxLayout *layout_;
};

#endif
