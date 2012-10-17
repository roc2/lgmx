#ifndef CPP_HIGHLIGHTER_H
#define CPP_HIGHLIGHTER_H

#include <interfaces.h>
#include <QString>
#include <QSyntaxHighlighter>

class cpp_highlighter;
class QTextDocument;

class cpp_highlighter_iface : public QObject, public Highlighter
{
	Q_OBJECT
	Q_INTERFACES(Highlighter)

public:
	~cpp_highlighter_iface() {}
	QString test_interface() const;
	
	void highlight(QTextDocument *doc);

private:
	cpp_highlighter *hl;
};



class cpp_highlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	
public:
	cpp_highlighter(QTextDocument *document = 0);
    ~cpp_highlighter();

    virtual void highlightBlock(const QString &text);
};

#endif

