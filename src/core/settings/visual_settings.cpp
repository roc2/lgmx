#include <visual_settings.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <iostream>

using namespace std;

visual_settings::visual_settings()
{
	show_white_spaces_ = false;
}

bool visual_settings::save()
{
	QJsonObject obj;
	
	obj.insert("white_spaces", show_white_spaces_);

	QJsonDocument doc(obj);
	
	QString val(doc.toJson());
	cout << val.toStdString() << endl;

	return true;
}
