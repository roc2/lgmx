#ifndef FILE_WATCHER
#define FILE_WATCHER

#include <QFileSystemWatcher>
#include <QObject>
#include <QString>
#include <QMap>
#include <QDateTime>

class view_manager;

class file_watcher : public QObject
{
	Q_OBJECT

private:
	file_watcher();
	file_watcher(const file_watcher&);
	file_watcher& operator=(const file_watcher&);

public:
	file_watcher(view_manager *manager);
	~file_watcher();
		
	void add_file(const QString &file_name);
	void remove_file(const QString &file_name);
	
	void notify_internal_modification(const QString &file_name);

private slots:
    void changed_file(const QString &file_name);

signals:
     void reload(const QString &path);

private:
	view_manager *manager_;
	QFileSystemWatcher file_watcher_;

    QMap<QString, qint64> changed_files_;
    QDateTime time;
};

#endif
