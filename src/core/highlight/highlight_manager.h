#ifndef HL_MANAGER_H
#define HL_MANAGER_H

class src_file;
class file_type;
class syntax_highlighter;

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
