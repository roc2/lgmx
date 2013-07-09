#ifndef TAB_SETTINGS_H
#define TAB_SETTINGS_H

class tab_settings
{
private:
	tab_settings(const tab_settings&);
	tab_settings& operator=(const tab_settings&);

public:
	enum policy {tabs_only, spaces_only, mixed};

	tab_settings();

	bool set_tab_width(int size);
	int get_tab_width() const;
	void set_tab_policy(tab_settings::policy pol);
	tab_settings::policy get_tab_policy() const;

private:
	int tab_width_;
	policy policy_;
};

#endif
