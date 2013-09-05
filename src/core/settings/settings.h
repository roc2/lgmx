#ifndef SETTINGS_H
#define SETTINGS_H

class QJsonObject;
class QString;

class Settings
{
public:
	Settings();
	virtual bool load() = 0;
	virtual bool save() = 0;
	virtual void apply() = 0;

protected:
	bool write_settings(QJsonObject &obj, const QString &file_name);

protected:
	bool pending_save_;
};

#endif
