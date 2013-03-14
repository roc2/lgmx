#include "file_watcher.h"

#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <iostream>


file_watcher::file_watcher()
{
	_handling = false;
	connect(&_file_watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(changed_file(const QString &)));
}

/*
file_watcher::file_watcher(Ui_MainWindow *mw)
{
	_handling = false;
	main_window = mw;
	connect(&_file_watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(changed_file(const QString &)));
	//connect(main_window, SIGNAL(windowActivated()), this, SLOT(check_for_reload()));
}
*/

file_watcher::~file_watcher()
{
}

void file_watcher::add_path(const QString &path)
{
	//this->_file_watcher.addPath(path);
	//cout << "Added " << path.toStdString() << " to file watcher" << endl;
}	

void file_watcher::remove_path(const QString &path)
{
	//this->_file_watcher.removePath(path);
	//cout << "Removed " << path.toStdString() << " from file watcher" << endl;
}

bool file_watcher::get_handling()
{
	_handling_mutex.lock();
	bool result(_handling);
	_handling_mutex.unlock();
	
	return result;
}

void file_watcher::set_handling(bool handling)
{
	_handling_mutex.lock();
	_handling = handling;
	_handling_mutex.unlock();
}

/**
 * @todo create a QSet in this class, whenever the editor saves a file 
 * it warns the file watcher adding the file to the set, when this slot 
 * is called we check whether the modified file is in the set, if it is the 
 * modification was done by the editor and shall be discarded.
 */

void file_watcher::changed_file(const QString &file_name)
{
	cout << "changed file!!" << endl;
    //const bool was_empty = changed_files.isEmpty();
    set_mutex.lock();
    
    bool reload = changed_files.isEmpty();

	if (!changed_files.contains(file_name))
		changed_files.insert(file_name);

	set_mutex.unlock();

    //if (reload && !changed_files.isEmpty()) {
    if (reload)
        QTimer::singleShot(200, this, SLOT(check_for_reload()));
}

void file_watcher::check_for_reload()
{
	cout << "check for reload" << endl;
	
	if (changed_files.isEmpty())
		return;
		
	//if (!main_window->is_active_window())
		//return;
	
	QMessageBox msg_box;
	
	for (QSet<QString>::iterator it = changed_files.begin(); it != changed_files.end(); it++) {
		msg_box.setInformativeText(tr("This file has been modified, reload from disk?"));
		msg_box.setStandardButtons(QMessageBox::Cancel);

		if (msg_box.exec() == QMessageBox::AcceptRole) {
			emit reload(*it);
		}
	}
}

void file_watcher::handle_changed_file(const QString &path)
{
	QString file;
	bool done = false;
	cout << "handle file changed" << endl;
	
	if (!get_handling()) {
		
		_handling = true;
		
		QMessageBox msg_box;
		//QPushButton reload_button(tr("Reload"));

		do {
			// message box
			msg_box.setInformativeText(tr("This file has been modified, reload from disk?"));
			msg_box.setStandardButtons(QMessageBox::Cancel);
			//msg_box.addButton(&reload_button, QMessageBox::AcceptRole);
			//msg_box.setDefaultButton(&reload_button);
			//msg_box.adjustSize();
			
			//msg_box.move(QApplication::desktop()->screen()->rect().center() - msg_box.rect().center());

			if (msg_box.exec() == QMessageBox::AcceptRole) {
				emit reload(path);
			}
			
			//_list_mutex.lock();
			//if (!_file_list.empty()) {
			//	file = _file_list.front();
			//	_file_list.pop_front();
			//} else
				done = true;

			//_list_mutex.unlock();
		} while (!done);

		set_handling(false);
	} else {
		cout << "pushed to list" << endl;
		//_list_mutex.lock();
		//_file_list.push_back(path);
		//_list_mutex.unlock();
	}
}







