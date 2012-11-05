#ifndef HL_MANAGER_H
#define HL_MANAGER_H

#include "highlighter.h"
#include <QSharedPointer>

#include <QSyntaxHighlighter>

class QString;
class src_file;
class file_type;
//class highlighter;

class highlight_manager
{
public:
	highlight_manager(file_type *type_manager);
	~highlight_manager();

	QSyntaxHighlighter* build_highlighter(src_file &file);



private:
	file_type *type_manager_;

};


#endif
