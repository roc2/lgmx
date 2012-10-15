#ifndef CPP_HIGHLIGHTER_H
#define CPP_HIGHLIGHTER_H

#include <interfaces.h>
#include <QString>

class cpp_highlighter : public QObject, public Highlighter
{
	Q_OBJECT
	Q_INTERFACES(Highlighter)

public:
	QString test_interface() const;
	~cpp_highlighter() {}


};

#endif

