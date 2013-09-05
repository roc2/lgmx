#ifndef FILE_SETTINGS_H
#define FILE_SETTINGS_H

#include <settings.h>

class file_settings : public Settings
{
private:
	file_settings(const file_settings&);
	file_settings& operator=(const file_settings&);

public:
	file_settings();
	~file_settings();

	bool load();
	bool save();
	void apply();

	unsigned int get_tab_width() const;
	void set_tab_width(unsigned int width);
	bool get_line_wrap() const;
	void set_line_wrap(bool wrap);
	bool get_spaces_only() const;
	void set_spaces_only(bool spaces_only);

private:
	unsigned int tab_width_;
	bool line_wrap_;
	bool spaces_only_;
};

#endif

