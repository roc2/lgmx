#ifndef RECENT_FILES_H
#define RECENT_FILES_H

#include <QStringList>
#include <QMenu>

#define MAX_FILES	10

class QAction;
class QSettings;

class recent_files : public QMenu
{
	Q_OBJECT

private:
	recent_files();
	recent_files(const recent_files&);
	recent_files& operator=(const recent_files&);

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
    void open_recent_file(const QString &);

private slots:
	void open_recent_file();

private:
    QStringList file_list;
    QAction *recentFileActions[MAX_FILES];
};

#endif
