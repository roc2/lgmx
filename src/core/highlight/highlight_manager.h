#ifndef HL_MANAGER_H
#define HL_MANAGER_H

#include <QSharedPointer>
#include <QWeakPointer>
#include <QTextCharFormat>
#include <vector>

class src_file;
class file_type;
class syntax_highlighter;


class highlight_manager
{
public:
	highlight_manager(file_type *type_manager);
	~highlight_manager();

	syntax_highlighter* build_highlighter(src_file *file);

	QSharedPointer<QSet<QString> > get_C_keywords();
	QSharedPointer<std::vector<QTextCharFormat> > get_C_formats();
	
	QSharedPointer<QSet<QString> > get_cpp_keywords();
	QSharedPointer<std::vector<QTextCharFormat> > get_cpp_formats();

private:
	file_type *type_manager_;
	
	QWeakPointer<QSet<QString> > C_keywords_;
	QWeakPointer<std::vector<QTextCharFormat> > C_formats_;
	
	QWeakPointer<QSet<QString> > cpp_keywords_;
	QWeakPointer<std::vector<QTextCharFormat> > cpp_formats_;
};

#endif
