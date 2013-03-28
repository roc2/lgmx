#include <file_watcher.h>
#include <view_manager.h>
#include <debug.h>


/**
 * Constructor.
 */

file_watcher::file_watcher(view_manager *manager) : manager_(manager), file_watcher_(this)
{
	connect(&file_watcher_, SIGNAL(fileChanged(const QString &)), this, SLOT(changed_file(const QString &)));
}

/**
 * Destructor.
 */

file_watcher::~file_watcher()
{
}

/**
 * Adds a file to watcher list.
 * @param file_name - absolute file name.
 */

void file_watcher::add_file(const QString &file_name)
{
	file_watcher_.addPath(file_name);
	debug(DEBUG, F_WATCHER, file_name.toStdString() << " added to file watcher");
}	

/**
 * Removes a file from watcher list.
 * @param file_name - absolute file name.
 */

void file_watcher::remove_file(const QString &file_name)
{
	file_watcher_.removePath(file_name);
	changed_files_.remove(file_name);
	debug(DEBUG, F_WATCHER, file_name.toStdString() << " removed from file watcher");
}

/**
 * Notification about internal modification. Updates the changed files map 
 * with the last modification timestamp. Whenever a "changed_file" signal 
 * is received the timestamps are compared to check whether the modification 
 * is external or not.
 * @param file_name - absolute file name.
 */

void file_watcher::notify_internal_modification(const QString &file_name)
{
	qint64 timestamp = time.currentMSecsSinceEpoch();
	 
	changed_files_.insert(file_name, timestamp);
	debug(DEBUG, F_WATCHER, "map size: " << changed_files_.size());
}

/**
 * [slot]
 */

void file_watcher::changed_file(const QString &file_name)
{
	qint64 timestamp = time.currentMSecsSinceEpoch();
	QMap<QString, qint64>::iterator it(changed_files_.find(file_name));
	
	if (it != changed_files_.end()) {
		if (timestamp - it.value() > 10) {
			debug(DEBUG, F_WATCHER, "External modification: " << it.value() << " " << timestamp);
		}
	} else {
		debug(DEBUG, F_WATCHER, "External modification: " << file_name.toStdString());
	}
}



