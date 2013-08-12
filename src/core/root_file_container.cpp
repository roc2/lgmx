#include <root_file_container.h>
#include <view_manager.h>
#include <settings.h>
#include <src_file.h>
#include <exception.h>
#include <debug.h>


/**
 * Constructor.
 */

root_file_container::root_file_container(view_manager& manager, Settings& settings) : manager_(manager), settings_(settings)
{
}

/**
 * Destructor.
 */

root_file_container::~root_file_container()
{
	std::map<unsigned int, src_file *>::iterator it;

	for (it = root_files_.begin(); it != root_files_.end(); it++)
		delete it->second;
}

/**
 * Creates a new file.
 * @param file_name - Complete file name.
 * @param file_id - Unique file ID.
 * @return true, if file created successfully, false otherwise.
 */

bool root_file_container::new_file(const QString &file_name, unsigned int file_id)
{
	src_file *new_file;

	try {
		new_file = new src_file(file_name, file_id, this, *manager_.get_settings(), manager_.get_highlight_manager(), manager_.get_type_manager());
		debug(INFO, ROOT_CONTAINER, "New file created with ID " << file_id);
	} catch(lgmx::exception &excp) {
		debug(ERR, ROOT_CONTAINER, excp.get_message());
		return false;
	}

	root_files_[file_id] = new_file;

	return true;
}

/**
 * Destroys the file.
 * @param file_id - Unique file ID.
 * @return true, if file destroyed successfully, false otherwise.
 */

bool root_file_container::destroy_file(unsigned int file_id)
{
	std::map<unsigned int, src_file *>::iterator it;

	if ((it = root_files_.find(file_id)) == root_files_.end()) {
		debug(ERR, ROOT_CONTAINER, "File with ID " << file_id << " not found");
		return false;
	}

	delete it->second;
	root_files_.erase(it);
	return true;
}

/**
 * Returns the source file through its unique ID.
 * @param file_id - Unique file ID.
 * @return pointer to the file, NULL pointer if the ID is invalid.
 */

src_file* root_file_container::get_file(unsigned int file_id)
{
	std::map<unsigned int, src_file *>::iterator it;

	if ((it = root_files_.find(file_id)) == root_files_.end())
		return NULL;

	return it->second;
}

