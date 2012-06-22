#ifndef STATUS_LINE_H
#define STATUS_LINE_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>

#include "src_container.h"

class status_line : public QWidget
{
	Q_OBJECT
	
public:
	status_line();
	~status_line();

private:
	void set_default_colors();

signals:

public slots:
	void set_file_name(const QString &file_name);
	void update_file_name(int index);

	void set_src_container(src_container *container);

private slots:

private:
	QLabel *file_name_;
	src_container *src_container_;
	
	QHBoxLayout *layout_;
};

#endif
