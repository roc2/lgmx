#ifndef STATUS_LINE_H
#define STATUS_LINE_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>

class status_line : public QWidget
{
	Q_OBJECT
	
public:
	status_line();
	~status_line();

signals:

public slots:
	void set_file_name(const QString &file_name);

private slots:

private:
	QLabel file_name_;
	
	QHBoxLayout *layout_;
};

#endif
