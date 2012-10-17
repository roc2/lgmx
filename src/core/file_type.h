#ifndef FILE_TYPE_H
#define FILE_TYPE_H

#include <map>

class QString;

/**
 * 
 */

class file_type
{
public:
	file_type();
	~file_type();
	
	enum type {UNKNOWN, C, CPP, JAVA, PYTHON};

	type get_file_type(const QString &suffix) const;
	void set_file_type(const QString &suffix, type);


private:
	std::map<QString, type> type_map_;
};


#endif

