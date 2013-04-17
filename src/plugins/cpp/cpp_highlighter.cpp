#include "cpp_highlighter.h"
#include <iostream>
#include <QTextDocument>


/**
 * Interface definitions.
 */

QString cpp_highlighter_iface::test_interface() const
{
	QString str("C++ Highlighter");

	return str;
}

void cpp_highlighter_iface::highlight(QTextDocument *doc)
{
	hl = new cpp_highlighter(doc);
}

/////////////////////////////////////////////////////////////////////////

cpp_highlighter::cpp_highlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{
}

cpp_highlighter::~cpp_highlighter()
{
}

void cpp_highlighter::highlightBlock(const QString &text)
{
	std::cout << "--" << text.toStdString() << std::endl;

}

Q_EXPORT_PLUGIN2(lgmx_cpp_highlighter, cpp_highlighter_iface)
