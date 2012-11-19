#ifndef TEXT_MANIP_H
#define TEXT_MANIP_H

#include <view_manager.h>

class QShortcut;

class text_manip : public QObject
{
	Q_OBJECT
	
public:
	text_manip(view_manager &manager, QWidget *parent = 0);
	~text_manip();

signals:

public slots:
	void duplicate_up();
	void duplicate_down();

private slots:

private:
	view_manager &manager_;
    
    QShortcut *dup_up;
    QShortcut *dup_down;
};

#endif
