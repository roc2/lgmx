#ifndef STATUS_LINE_H
#define STATUS_LINE_H

#include <QWidget>
#include <QString>
#include <c_label.h>
#include <QListWidget>
#include <QHBoxLayout>

#include "src_container.h"

class QComboBox;
class src_container;

#define NO_NAME "--" NEW_FILE_NAME "--"

class status_line : public QWidget
{
	Q_OBJECT
	
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
	void file_name_clicked();

private slots:
	void current_file_changed(int index);

private:
	//src_container *container_;
	QComboBox *file_list_;
	src_container *src_container_;
	
	QHBoxLayout *layout_;
};

#endif
