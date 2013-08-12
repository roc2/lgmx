#include <file_type.h>
#include <QString>

#include <iostream>

/**
 * Default constructor.
 */

file_type::file_type()
{
	// default "file extension -> file type" map
	type_map_["c"] = C;
	type_map_["cpp"] = CPP;
	type_map_["h"] = CPP;
	type_map_["hpp"] = CPP;
	type_map_["java"] = JAVA;
	type_map_["py"] = PYTHON;
}

/**
 * Destructor.
 */

file_type::~file_type()
{
}

/**
 * Returns the file type according to the file name extension.
 * @param suffix -> file name extension.
 * @return file type, or UNKNOWN if the extension is unknown.
 */

file_type::type file_type::get_file_type(const QString &suffix) const
{
	std::map<QString, file_type::type>::const_iterator it(type_map_.find(suffix));
	
	if (it != type_map_.end())
		return it->second;
	
	return UNKNOWN;
}

/**
 * Returns the file type according to the file name.
 * @param file_name -> the name of the file.
 * @return file type, or UNKNOWN if the extension is unknown.
 */

file_type::type file_type::get_file_type_from_name(const QString &file_name) const
{
	int size = file_name.size();
	int index = size - 1;

	if (size < 2)	// invalid file_name
		return UNKNOWN;

	int begin = 0;
	for (int i = size - 1; i >= 0; i--) {
		if (file_name[i] == '/' || file_name[i] == '\\') {
			begin = i + 1;
			break;
		}
	}

	while (file_name[index] != '.' && index > begin)
		index--;
	
	if (file_name[index] != '.' || (file_name[index] == '.' && index == begin))
		return UNKNOWN;

	QString suffix(file_name.mid(index + 1, size - index));
	
	return get_file_type(suffix.toLower());
}

/**
 * Sets the file type for the specified extension. Different extensions can refer 
 * to the same file type.
 * @param suffix -> file name extension.
 * @param tp -> file type.
 */

void file_type::set_file_type(const QString &suffix, file_type::type tp)
{
	type_map_[suffix] = tp;
}

/**
 * Returns a string with the file type. For debug purposes only.
 */

QString file_type::to_string(file_type::type t)
{
	switch (t) {
	case C:
		return "C";
	case CPP:
		return "CPP";
	case JAVA:
		return "JAVA";
	case PYTHON:
		return "PYTHON";
	default:
		return "UNKNOWN";
	}
}

