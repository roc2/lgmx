#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>


class QString;
class QTextDocument;


class Highlighter
{
	
public:
    virtual ~Highlighter() {}
    virtual QString test_interface() const = 0;
    virtual void highlight(QTextDocument *doc) = 0;
};

Q_DECLARE_INTERFACE(Highlighter, "com.lgmx.highlighter/1.0")

 

#endif











