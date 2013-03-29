#include <QFile>

#include <tags.h>
#include <view_manager.h>

#include <QStringListModel>
#include <QListView>

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
	
	model_ = new QStringListModel();
	view_ = new QListView(manager_);
	
	view_->setModel(model_);
	view_->setViewMode(QListView::ListMode);
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
	QFile file(file_name);
    
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		res = "Unable to open file: " + file_name;
		return false;
	}

	file.close();
	
	// check if file is already in the list
	std::list<QString>::iterator it(tag_files_.begin());
	
	for (; it != tag_files_.end(); it++) {
		if (*it == file_name)
			return true;
	}
	
	tag_files_.push_back(file_name);
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

bool tag::find_tag(const QString &tag_name, QString &file_name, int &line)
{
	tagFile *tag;
	tagFileInfo info;
	QStringList matches;
	
	if (tag_files_.empty()) {
		return false;
	}
	
	tagResult val;
	tagEntry entry;
	
	std::list<QString>::iterator it(tag_files_.begin());
	
	for (; it != tag_files_.end(); it++) {
	
		tag = tagsOpen((*it).toStdString().c_str(), &info);
		
		if (!info.status.opened) {
			std::cout << "could not open tags file" << std::endl;
			continue;
		}
		
		val = tagsFind(tag, &entry, tag_name.toStdString().c_str(), TAG_FULLMATCH | TAG_OBSERVECASE);

		if (val == TagSuccess) {
			file_name = entry.file;
			line = entry.address.lineNumber;
			cout << "Found: " << entry.name << " " << entry.file << " " << "line: " << entry.address.lineNumber << endl;
			
			tag_matches_.push_back(std::pair<QString, int>(entry.file, entry.address.lineNumber));
			
			matches << QString(entry.file) + " " + QString::number(entry.address.lineNumber);
			
			while (tagsFindNext(tag, &entry) == TagSuccess) {
				matches << QString(entry.file) + " " + QString::number(entry.address.lineNumber);
				tag_matches_.push_back(std::pair<QString, int>(entry.file, entry.address.lineNumber));
			}

		} else {
			cout << "Tag not found" << endl;
		}
	}

	if (matches.isEmpty()) {
		tag_matches_.clear();
		return false;
	} else if (matches.size() > 1) {
		show_tags_list(matches);
	} else {
		emit jump_to(tag_matches_[0].first, tag_matches_[0].second);
		tag_matches_.clear();
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
	
	if (event->type() == QEvent::KeyPress) {
        QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(event);
        int PressedKey = pKeyEvent->key();

		switch (PressedKey) {
		
		case Qt::Key_Escape:
            view->hide();
            break;
        
        case Qt::Key_Return:
        case Qt::Key_Enter: {
			cout << "chosen " << view->currentIndex().row() << endl;
			int index = view->currentIndex().row();
			view->hide();
			emit jump_to(tag_matches_[index].first, tag_matches_[index].second);
			tag_matches_.clear();
			break;
        }
        default:
			return false;		// default event handling
        }
    } else if (event->type() == QEvent::FocusOut) {
		view->hide();
	}
	
	return true;
}





















