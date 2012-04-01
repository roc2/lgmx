#ifndef FILE_WATCHER
#define FILE_WATCHER

#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QSet>

using namespace std;

class file_watcher : public QObject
{
	Q_OBJECT

public:
	file_watcher();
	//file_watcher(Ui_MainWindow *mw);
	~file_watcher();
		
	void add_path(const QString &path);
	void remove_path(const QString &path);

private:
	QFileSystemWatcher _file_watcher;
    bool _handling;

    QSet<QString> changed_files;
	QMutex set_mutex;
	QMutex _handling_mutex;

	bool get_handling();
	void set_handling(bool handling);

private slots:
    void handle_changed_file(const QString &path);
    void changed_file(const QString &file_name);

public slots:
	void check_for_reload();

signals:
     void reload(const QString &path);
};

#endif
