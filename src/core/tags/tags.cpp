#include <QFile>

#include <tags.h>



#include <iostream>
using namespace std;


tag::tag()
{
}

tag::~tag()
{
	
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
	
	if (tag_files_.empty()) {
		return false;
	}
	
	std::list<QString>::iterator it(tag_files_.begin());
	
	for (; it != tag_files_.end(); it++) {
	
		tag = tagsOpen((*it).toStdString().c_str(), &info);
		//tag = tagsOpen("/home/lgm/tags", &info);
		
		if (!info.status.opened) {
			std::cout << "could not open tags file" << std::endl;
			continue;
		}

		tagResult val;
		tagEntry entry;
		
		val = tagsFind(tag, &entry, tag_name.toStdString().c_str(), TAG_FULLMATCH | TAG_OBSERVECASE);

		if (val == TagSuccess) {
			file_name = entry.file;
			line = entry.address.lineNumber;
			cout << "Found: " << entry.file << " " << "line: " << entry.address.lineNumber << endl;
			return true;
		} else {
			cout << "Fail" << endl;
			return false;
		}
	}

	return true;
}
