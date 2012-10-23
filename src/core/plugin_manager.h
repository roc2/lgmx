#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <file_type.h>


class plugin_manager
{
public:
	plugin_manager();
	~plugin_manager();

	bool load_plugins(file_type::type tp);



private:
	bool load_cpp_plugins();
	
private:
	std::map<file_type::type, QStringList> plugin_map_;
};


#endif

