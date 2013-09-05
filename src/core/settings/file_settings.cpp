#include <file_settings.h>
#include <QJsonObject>

/**
 * Constructor. Set default values.
 */

file_settings::file_settings() : Settings()
{
	tab_width_ = 4;
	line_wrap_ = false;
	spaces_only_ = false;
}

/**
 * 
 */

file_settings::~file_settings()
{
}

/**
 * 
 */

bool file_settings::load()
{
	return true;
}

/**
 * Save settings on JSON file.
 * @return true if saved correctly, false otherwise.
 */

bool file_settings::save()
{
	if (!pending_save_)
		return true;
	
	QJsonObject obj;
	obj.insert("tab_width", static_cast<double>(tab_width_));
	obj.insert("line_wrap", line_wrap_);
	obj.insert("spaces_only", spaces_only_);
	
	pending_save_ = !write_settings(obj, "test_file");
	return !pending_save_;
}

/**
 * 
 */

void file_settings::apply()
{
}

/**
 * 
 */

unsigned int file_settings::get_tab_width() const
{
	return tab_width_;
}

/**
 * 
 */
	
void file_settings::set_tab_width(unsigned int width)
{
	tab_width_ = width;
}

/**
 * 
 */
	
bool file_settings::get_line_wrap() const
{
	return line_wrap_;
}

/**
 * 
 */

void file_settings::set_line_wrap(bool wrap)
{
	line_wrap_ = wrap;
}
	
/**
 * 
 */
	
bool file_settings::get_spaces_only() const
{
	return spaces_only_;
}

/**
 * 
 */

void file_settings::set_spaces_only(bool spaces_only)
{
	spaces_only_ = spaces_only;
}
	
	
	
