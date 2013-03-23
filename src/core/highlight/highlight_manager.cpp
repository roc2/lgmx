#include <highlight_manager.h>
#include <syntax_highlighter.h>
#include <QSet>
#include <src_file.h>
#include <file_type.h>
#include <cpp/cpp_hl.h>

#include <iostream>
using namespace std;

/**
 * Constructor.
 */

highlight_manager::highlight_manager(file_type *type_manager)
{
	type_manager_ = type_manager;
}

/**
 * Destructor.
 */

highlight_manager::~highlight_manager()
{
}

/**
 * Creates a new highlighter instance for the specified file. The 
 * highlighter type is chosen according to the file extension. The file 
 * takes the ownership of the highlighter object and therefore shall 
 * destroy it when the file is closed of rehighlighted with another type.
 * @param file - pointer to the file to be highlighted.
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
		hl = new C_highlighter(file, get_C_keywords());
		break;
	
	default:
		hl = NULL;
		break;
	}

	return hl;
}

/*
 * The "get_*_keywords" methods return a shared pointer to the set of 
 * keywords, while the "highlight_manager" class holds a weak pointer 
 * to the set.
 * This saves memory, since all highlighters of the same type use the 
 * same set of keywords, and when there are no files of a specific 
 * type open, the respective set is destroyed automatically.
 */

/**
 * Returns the C language set of keywords.
 * @return shared pointer to the set of keywords.
 */

QSharedPointer<QSet<QString> > highlight_manager::get_C_keywords()
{
	QSharedPointer<QSet<QString> > ret;
	
	if (!(ret = C_keywords_.toStrongRef())) {
		cout << "\n\nnew qset!!!!\n\n" << endl;
		ret = QSharedPointer<QSet<QString> >(new QSet<QString>());
		C_keywords_ = ret;
		(*ret) << "auto" << "break" << "case" << "char" << "const" << "continue"
		       << "default" << "do" << "double" << "else" << "enum" << "extern"
		       << "float" << "for" << "goto" << "if" << "int" << "long"
		       << "register" << "return" << "short" << "signed" << "sizeof" 
		       << "static" << "struct" << "switch" << "typedef" << "union"
		       << "unsigned" << "void" << "volatile" << "while";
	}
	
	return ret;
}



