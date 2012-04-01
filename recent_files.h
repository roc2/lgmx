#ifndef RECENT_FILES_H
#define RECENT_FILES_H

#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QSettings>

#define MAX_FILES	10

class recent_files : public QMenu
{
	Q_OBJECT
	
public:
	recent_files(const QString &title, QWidget *parent = 0);
	~recent_files();

    void add_file(const QString &file_name);
	const QStringList& get_file_list(); 

    void load_files_from_disk(const QSettings &settings);
	void save_files_to_disk(QSettings &settings);

private:
	void update_menu();

signals:
    void open_recent_file(QString &);

private slots:
	void open_recent_file();

private:
    QStringList file_list;
    QAction *recentFileActions[MAX_FILES];
};

#endif
