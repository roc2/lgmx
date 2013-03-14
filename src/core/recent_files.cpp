#include <recent_files.h>
#include <QVariant>
#include <QAction>
#include <QSettings>

/**
 * Constructor.
 */

recent_files::recent_files(const QString &title, QWidget *parent) : QMenu(title, parent)
{
	for (int i = 0; i < MAX_FILES; i++) {
		recentFileActions[i] = new QAction(NULL);
		recentFileActions[i]->setVisible(false);
		this->addAction(recentFileActions[i]);
		connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(open_recent_file()));
	}
}

/**
 * Destructor.
 */

recent_files::~recent_files()
{
	for (int i = 0; i < MAX_FILES; i++)
		delete recentFileActions[i];
}

/**
 * Adds a new file to the list.
 * @param file_name - file to be added.
 */

void recent_files::add_file(const QString &file_name)
{
    int index;

    if ((index = file_list.indexOf(file_name)) < 0) {	// not found
        file_list.push_front(file_name);
        
        if (file_list.size() > MAX_FILES)
            file_list.pop_back();
    } else {
		// already in the list, just move to front
        file_list.insert(0, file_list.takeAt(index));
    }
    
    update_menu();
}

/**
 * Updates graphical menu.
 */

void recent_files::update_menu()
{
	int i;

	for (i = 0; i < file_list.size(); i++) {
		recentFileActions[i]->setText(file_list.at(i));
		recentFileActions[i]->setVisible(true);
	}
	
	for (; i < MAX_FILES; i++)
		recentFileActions[i]->setVisible(false);
}

/**
 * Opens file from the list.
 */

void recent_files::open_recent_file()
{
	QAction *action = static_cast<QAction *>(sender());
	
	QString file_name(action->text());

	emit open_recent_file(file_name);
}

/**
 * Returns the file list.
 */

const QStringList& recent_files::get_file_list()
{
	return file_list;
}

/**
 * Reads recent file list from permanent storage.
 */

void recent_files::load_files_from_disk(const QSettings &settings)
{
	QVariant v(settings.value("recent_files"));
	
	file_list = v.toStringList();
	update_menu();
}

/**
 * Writes recent file list to permanent storage.
 */

void recent_files::save_files_to_disk(QSettings &settings)
{
	QVariant v(file_list);
	
	settings.setValue("recent_files", v);
}

