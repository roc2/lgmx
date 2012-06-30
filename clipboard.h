#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "src_container.h"
#include <QShortcut>
#include <QSignalMapper>
#include <view_manager.h>

#define MAX_BUFFS	10
#define NUM_SHORTCUTS	10


class clipboard : public QObject
{
	Q_OBJECT
	
public:
	clipboard(view_manager &manager, QWidget *parent = 0);
	~clipboard();

signals:

public slots:
	void copy_buff(int index);
	void cut_buff(int index);
	void paste_buff(int index);

private slots:

private:
	src_container **src_ctr;
	view_manager &manager_;
	
	QSignalMapper copy_signal_map;
	QSignalMapper cut_signal_map;
	QSignalMapper paste_signal_map;
	
    QString buffers[26];
    
    QShortcut *copy_s[NUM_SHORTCUTS];
    QShortcut *cut_s[NUM_SHORTCUTS];
    QShortcut *paste_s[NUM_SHORTCUTS];
};

#endif
