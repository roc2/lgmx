#ifndef HL_MANAGER_H
#define HL_MANAGER_H

#include <QSharedPointer>
#include <QWeakPointer>

class src_file;
class file_type;
class syntax_highlighter;

/**
 * These weak pointers are declared global due to a very strange runtime 
 * allocation abort when they are declared within the class. I don't know
 * why.
 * @todo find a better solution fot this, maybe a Qt newer than 4.7.
 */

static QWeakPointer<QSet<QString> > cpp_keywords_;

class highlight_manager
{
public:
	highlight_manager(file_type *type_manager);
	~highlight_manager();

	syntax_highlighter* build_highlighter(src_file *file);

	QSharedPointer<QSet<QString> > get_C_keywords();
	QSharedPointer<QSet<QString> > get_cpp_keywords();

private:
	file_type *type_manager_;
	
	QWeakPointer<QSet<QString> > C_keywords_;
};

#endif
