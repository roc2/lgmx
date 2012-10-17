#include <file_type.h>
#include <QString>

/**
 * Default constructor.
 */

file_type::file_type()
{
	// default file types
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
 * 
 */

file_type::type file_type::get_file_type(const QString &suffix) const
{
	std::map<QString, file_type::type>::const_iterator it(type_map_.find(suffix));
	
	if (it != type_map_.end())
		return it->second;
	
	return UNKNOWN;
}

/**
 * 
 */

void file_type::set_file_type(const QString &suffix, file_type::type tp)
{
	type_map_[suffix] = tp;
}