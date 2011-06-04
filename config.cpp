
#include "config.h"
#include <iostream>

using namespace std;

Config::Config()
{
    QDir dir;

    conf_file = dir.homePath();
    conf_file.append(CONF_FILE);

    //cout << conf_file.toStdString() << endl;
}

