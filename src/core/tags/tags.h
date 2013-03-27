#ifndef TAGS_H
#define TAGS_H

#include <readtags.h>
#include <QString>
#include <list>

/**
 * This class relies on exuberant ctags 5.8 API for tags search.
 */

class tag
{
private:
	tag(const tag&);
	tag& operator=(const tag&);

public:
	tag();
	~tag();

	bool add_tags_file(const QString &file_name, QString &res);
	bool remove_tags_file(const QString &file_name, QString &err);
	void clear_tags();
	
	bool find_tag(const QString &name, QString &file_name, int &line);
	
private:
	QString tag_file_;
	std::list<QString> tag_files_;
};

#endif
