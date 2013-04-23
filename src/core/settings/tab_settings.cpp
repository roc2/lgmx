#include <tab_settings.h>

/**
 * 
 */

tab_settings::tab_settings()
{
	// load settings from file, if error load defaults
	tab_width_ = 4;
	policy_ = tabs_only;
}

/**
 * 
 */

bool tab_settings::set_tab_width(int size)
{
	if (size <= 0)
		return false;
	
	tab_width_ = size;
	return true;
}

/**
 * 
 */

int tab_settings::get_tab_width()
{
	return tab_width_;
}

/**
 * 
 */

void tab_settings::set_tab_policy(tab_settings::policy pol)
{
	policy_ = pol;
}

/**
 * 
 */

tab_settings::policy tab_settings::get_tab_policy()
{
	return policy_;
}





