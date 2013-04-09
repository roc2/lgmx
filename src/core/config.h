#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

#define CONF_FILE   "/.lgmxconfig"

class Config
{
private:
	Config(const Config&);
	Config& operator=(const Config&);
	
public:
    Config();
    ~Config();

private:
	QString conf_file;
};

#endif
