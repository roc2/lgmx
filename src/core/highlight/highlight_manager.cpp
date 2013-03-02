
#include <highlight_manager.h>
#include <src_file.h>
#include <file_type.h>
#include <cpp/cpp_hl.h>

/**
 * Constructor.
 */

highlight_manager::highlight_manager(file_type *type_manager)
{
	type_manager_ = type_manager;
}

/**
 * Deestructor.
 */

highlight_manager::~highlight_manager()
{
}

/**
 * Creates a new instance of the highlighter for the specified file. The 
 * highlighter type is chosen according to the file extension. The 
 * highlighter object life cycle is controled by the file itself, so it should 
 * be destroyed when the file is closed of rehighlighted with another type.
 * @param file - reference to the file to be highlighted.
 * @return pointer to the new highlighter object, or NULL if the file type 
 * is unknown.
 */

syntax_highlighter* highlight_manager::build_highlighter(src_file *file)
{
	syntax_highlighter *hl;
	file_type::type tp = type_manager_->get_file_type(file->get_src_file_extension());
	
	switch (tp) {

	case file_type::C:
	case file_type::CPP:
		hl = new C_highlighter(file);
		//hl = new dummy_highlighter(file);
		break;
	
	default:
		hl = NULL;
		break;
	}

	return hl;
}
