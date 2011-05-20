#ifndef FILE_H
#define FILE_H

#include <QString>
#include <string>

using namespace std;

class File {
	
	public:
	bool read_file(const QString &fileName, QString &val, QString &error);
	bool write_file(const QString &fileName, const QString &content, QString &error);
	int get_file_name(QString complete_name, string &name);

};


#endif
