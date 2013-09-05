#include <settings.h>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

#include <iostream>
using namespace std;

Settings::Settings()
{
	pending_save_ = false;
}

/**
 * Writes JSON object to file.
 */

bool Settings::write_settings(QJsonObject &obj, const QString &file_name)
{
	QJsonDocument doc(obj);
	
	// write json to file
	
	QString val(doc.toJson());
	cout << val.toStdString() << endl;

	return true;
}

