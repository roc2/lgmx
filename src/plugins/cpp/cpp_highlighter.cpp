#include "cpp_highlighter.h"



QString cpp_highlighter::test_interface() const
{
	QString str("C++ Highlighter");

	return str;
}



Q_EXPORT_PLUGIN2(lgmx_cpp_highlighter, cpp_highlighter)
