#ifndef TAGS_H
#define TAGS_H

#include <readtags.h>
#include <QString>

/**
 * This class relies on exuberant ctags 5.8 API for searching tags.
 */

class tag
{
private:
	tag(const tag&);
	tag& operator=(const tag&);

public:
	tag();
	~tag();

	bool find_tag(const QString &name, QString &file_name, int &line);
	
private:
	QString tag_file_;
};

#endif
