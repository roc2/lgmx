#include <tags.h>



#include <iostream>
using namespace std;


tag::tag()
{
}

tag::~tag()
{
	
}

bool tag::find_tag(const QString &tag_name, QString &file_name, int &line)
{
	tagFile *tag;
	tagFileInfo info;
	
	if (tag_file_.isEmpty()) {
	
		//return false;
	}
	
	//tag = tagsOpen(tag_file_.toStdString().c_str(), &info);
	tag = tagsOpen("/home/lgm/tags", &info);
	
	if (!info.status.opened) {
		std::cout << "could not open tags file" << std::endl;
		return false;
	}

	tagResult val;
	tagEntry entry;
	
	val = tagsFind(tag, &entry, tag_name.toStdString().c_str(), TAG_FULLMATCH | TAG_OBSERVECASE);

	if (val == TagSuccess) {
		file_name = entry.file;
		line = entry.address.lineNumber;
		cout << "Found: " << entry.file << " " << "line: " << entry.address.lineNumber << endl;
	} else {
		cout << "Fail" << endl;
		return false;
	}

	return true;

}
