#ifndef SETTINGS_H
#define SETTINGS_H

#include <json.h>
#include <tab_settings.h>

class QString;

class Settings
{
public:
	Settings();
	~Settings();

	bool read_settings(const QString &file_path);
	bool parse_json(std::string &buff);

	tab_settings& get_tab_settings();

	bool set_tab_width(int size);
	int get_tab_width();
	
	void set_line_wrap(bool wrap);
	bool get_line_wrap();

private:

	int tab_width_;
	bool line_wrap_;

	tab_settings tab_settings_;
};


#endif
