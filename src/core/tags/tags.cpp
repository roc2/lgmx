#include <QFile>

#include <tags.h>
#include <view_manager.h>
#include <debug.h>

#include <QStringListModel>
#include <QListView>
#include <QDir>

#include <QEvent>
#include <QKeyEvent>
#include <iostream>
using namespace std;

/**
 * Constructor.
 */

tag::tag(view_manager *manager) : manager_(manager)
{
	QObject::connect(this, SIGNAL(jump_to(const QString &, int)), manager_, SLOT(jump_to(const QString &, int)));

	model_ = new QStringListModel(this);
	view_ = new QListView(manager_);
	
	view_->setModel(model_);
	view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
	view_->setSelectionMode(QAbstractItemView::SingleSelection);
	view_->installEventFilter(this);
}

/**
 * Destructor.
 */

tag::~tag()
{
	delete view_;
	delete model_;
}

/**
 * Adds a tags file.
 * @param file_name - absolute file name.
 */

bool tag::add_tags_file(const QString &file_name, QString &res)
{
	// canonical name (no symbolic links, "." or "..")
	QDir path(file_name);
	QString can_name(path.canonicalPath());
	QFile file(can_name);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		res = "Unable to open file: " + file_name;
		return false;
	}

	file.close();
	
	// check if file is already in the list
	std::list<QString>::iterator it(tag_files_.begin());
	
	for (; it != tag_files_.end(); it++) {
		if (*it == can_name) {
			res = "Tags file already added";
			return true;
		}
	}
	
	tag_files_.push_back(can_name);
	res = "Tags file added: " + file_name;
	return true;
}

/**
 * Removes a tags file.
 * @param file_name - absolute file name.
 */

bool tag::remove_tags_file(const QString &file_name, QString &err)
{
	std::list<QString>::iterator it(tag_files_.begin());

	while (it != tag_files_.end()) {
		if (*it == file_name) {
			tag_files_.erase(it);
			return true;
		} else {
			it++;
		}
	}
	
	err = "File not found: " + file_name;
	return false;
}

/**
 * 
 */

void tag::clear_tags()
{
	tag_files_.clear();
}

/**
 * 
 */

bool tag::find_tag(const QString &tag_name)
{
	if (tag_files_.empty()) {
		debug(INFO, TAGS, "No tags file set");
		return false;
	}
	
	tagFile *tag;
	tagFileInfo info;
	QStringList matches;
	tagResult val;
	tagEntry entry;
	
	tag_matches_.clear();	// clear matches from previous calls
	std::list<QString>::iterator it(tag_files_.begin());
	
	for (; it != tag_files_.end(); it++) {
	
		tag = tagsOpen((*it).toStdString().c_str(), &info);
		
		if (!info.status.opened) {
			debug(ERR, TAGS, "Unable to open tags file");
			continue;
		}
		
		val = tagsFind(tag, &entry, tag_name.toStdString().c_str(), TAG_FULLMATCH | TAG_OBSERVECASE);

		if (val == TagSuccess) {
			cout << "Found: " << entry.name << " " << entry.file << " " << "line: " << entry.address.lineNumber << endl;
			
			tag_matches_.push_back(std::pair<QString, int>(entry.file, entry.address.lineNumber));
			
			matches << QString(entry.file) + " " + QString::number(entry.address.lineNumber);
			
			while (tagsFindNext(tag, &entry) == TagSuccess) {
				matches << QString(entry.file) + " " + QString::number(entry.address.lineNumber);
				tag_matches_.push_back(std::pair<QString, int>(entry.file, entry.address.lineNumber));
			}

		} else {
			debug(INFO, TAGS, "Tag not found");
		}
	}

	if (matches.isEmpty()) {
		return false;
	} else if (matches.size() == 1) {
		emit jump_to(tag_matches_[0].first, tag_matches_[0].second);
	} else {
		show_tags_list(matches);
	}

	return true;
}

/**
 * Shows found tags list. In case of ambiguous tags match, this dialog 
 * displays all matches so the user can choose the correct one.
 * @param matches - list of tag matches.
 */

void tag::show_tags_list(QStringList &matches)
{
	model_->setStringList(matches);
	
	view_->setCurrentIndex(model_->index(0, 0));
	view_->resize (500, 100);
	view_->move (300, 100);
	view_->setFocus();
	view_->show();
}

/**
 * Tags list event filter. This event filter handles the tags list widget
 *  key presses, mouse clicks and focus.
 */

bool tag::eventFilter(QObject *obj, QEvent *event)
{
	QListView *view = static_cast<QListView*>(obj);
	
	if (obj == view_) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *key_event = static_cast<QKeyEvent *>(event);
			int PressedKey = key_event->key();

			switch (PressedKey) {
			
			case Qt::Key_Escape:
				view->hide();
				break;
			
			case Qt::Key_Return:
			case Qt::Key_Enter: {
				int index = view->currentIndex().row();
				view->hide();
				emit jump_to(tag_matches_[index].first, tag_matches_[index].second);
				break;
			}
			default:
				return false;	// default event handling
			}
			
			return true;
		} else if (event->type() == QEvent::FocusOut) {
			view->hide();
			return true;
		}
	}
	
	return false;		// default event handling
}





















