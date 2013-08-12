#ifndef TEXT_MANIP_H
#define TEXT_MANIP_H

#include <QObject>

class view_manager;
class QShortcut;

class text_manip : public QObject
{
	Q_OBJECT

private:
	text_manip();
	text_manip(const text_manip&);
	text_manip& operator=(const text_manip&);

public:
	text_manip(view_manager &manager, QWidget *parent = 0);
	~text_manip();

signals:

public slots:
	void duplicate_up();
	void duplicate_down();
	void delete_current_line();

private slots:

private:
	view_manager &manager_;

	QShortcut *dup_up;
	QShortcut *dup_down;
	QShortcut *delete_line;
};

#endif
