#ifndef ROOT_FILE_CONTAINER_H
#define ROOT_FILE_CONTAINER_H

#include <map>
#include <file_type.h>

class src_file;
class view_manager;
class Settings;
class QString;

class root_file_container
{
private:
	root_file_container(const root_file_container&);
	root_file_container& operator=(const root_file_container&);

public:
	root_file_container(view_manager& manager, Settings& settings);
	~root_file_container();

	bool new_file(const QString &file_name, unsigned int file_id);
	bool destroy_file(unsigned int file_id);
	src_file* get_file(unsigned int file_id);

private:
	view_manager &manager_;
	Settings &settings_;
	std::map<unsigned int, src_file *> root_files_;
};

#endif

