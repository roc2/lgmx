#ifndef HL_MANAGER_H
#define HL_MANAGER_H

#include "highlighter.h"
#include <QSharedPointer>

#include <syntax_highlighter.h>

class QString;
class src_file;
class file_type;
//class highlighter;

class highlight_manager
{
public:
	highlight_manager(file_type *type_manager);
	~highlight_manager();

	syntax_highlighter* build_highlighter(src_file *file);



private:
	file_type *type_manager_;

};


#endif
