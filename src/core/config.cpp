
#include <config.h>
#include <QDir>


Config::Config()
{
    QDir dir;

    conf_file = dir.homePath();
    conf_file.append(CONF_FILE);
}

Config::~Config()
{
}

