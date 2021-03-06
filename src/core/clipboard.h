#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QSignalMapper>

#define MAX_BUFFS	10
#define NUM_SHORTCUTS	10

class view_manager;
class QShortcut;

class clipboard : public QObject
{
	Q_OBJECT

private:
	clipboard(const clipboard&);
	clipboard& operator=(const clipboard&);

public:
	clipboard(view_manager &manager, QWidget *parent = 0);
	~clipboard();

signals:

public slots:
	void copy_buff(int index);
	void cut_buff(int index);
	void paste_buff(int index);

private:
	view_manager &manager_;
	
	QSignalMapper copy_signal_map;
	QSignalMapper cut_signal_map;
	QSignalMapper paste_signal_map;
	
    QString buffers[MAX_BUFFS];
    
    QShortcut *copy_s[NUM_SHORTCUTS];
    QShortcut *cut_s[NUM_SHORTCUTS];
    QShortcut *paste_s[NUM_SHORTCUTS];
};

#endif
