#include <highlight_manager.h>
#include <syntax_highlighter.h>
#include <QSet>
#include <src_file.h>
#include <file_type.h>
#include <debug.h>
#include <cpp/cpp_hl.h>


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
		hl = new C_highlighter(file, get_C_keywords(), get_C_formats());
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
 * The "get_*_formats" methods use the same approach, but with a vector
 * instead.
 */

/**
 * Returns the C language set of keywords.
 * @return shared pointer to the set of keywords.
 */

QSharedPointer<QSet<QString> > highlight_manager::get_C_keywords()
{
	QSharedPointer<QSet<QString> > ret;
	
	if (!(ret = C_keywords_.toStrongRef())) {
		debug(DEBUG, HIGHLIGHT, "New C keywords set");
		ret = QSharedPointer<QSet<QString> >(new QSet<QString>());
		C_keywords_ = ret;
		(*ret) << "auto" << "break" << "case" << "char" << "const" << "continue"
		       << "default" << "do" << "double" << "else" << "enum" << "extern"
		       << "float" << "for" << "goto" << "if" << "inline" << "int" << "long"
		       << "register" << "return" << "short" << "signed" << "sizeof" 
		       << "static" << "struct" << "switch" << "typedef" << "union"
		       << "unsigned" << "void" << "volatile" << "while";
	}

	return ret;
}

/**
 * Returns the C language vector of formats.
 * @return shared pointer to the vector of formats.
 */

QSharedPointer<std::vector<QTextCharFormat> > highlight_manager::get_C_formats()
{
	QSharedPointer<std::vector<QTextCharFormat> > ret;
	
	if (!(ret = C_formats_.toStrongRef())) {
		debug(DEBUG, HIGHLIGHT, "New C formats set");
		ret = QSharedPointer<std::vector<QTextCharFormat> >(new std::vector<QTextCharFormat>());
		C_formats_ = ret;
		
		QTextCharFormat integerFormat;
		integerFormat.setForeground(Qt::blue);
		integerFormat.setBackground(Qt::transparent);
		integerFormat.setFontWeight(QFont::Bold);
		
		QTextCharFormat keywordFormat;
		keywordFormat.setForeground(Qt::darkCyan);
		keywordFormat.setBackground(Qt::transparent);
		keywordFormat.setFontWeight(QFont::Bold);
		
		QTextCharFormat CommentFormat;
		CommentFormat.setForeground(Qt::red);
		CommentFormat.setBackground(Qt::transparent);
		
		QTextCharFormat literalFormat;
		literalFormat.setForeground(Qt::magenta);
		literalFormat.setBackground(Qt::transparent);
		
		QTextCharFormat pre_processor;
		pre_processor.setForeground(Qt::darkGreen);
		pre_processor.setBackground(Qt::transparent);
		
		(*ret).resize(C_highlighter::SIZE);
		(*ret)[C_highlighter::NUMBER_IDX] = integerFormat;
		(*ret)[C_highlighter::KEYWORD_IDX] = keywordFormat;
		(*ret)[C_highlighter::COMMENT_IDX] = CommentFormat;
		(*ret)[C_highlighter::LITERAL_IDX] = literalFormat;
		(*ret)[C_highlighter::PRE_PROC_IDX] = pre_processor;
	}
	
	return ret;
}

/**
 * Returns the C++ language set of keywords.
 * @return shared pointer to the set of keywords.
 */

QSharedPointer<QSet<QString> > highlight_manager::get_cpp_keywords()
{
	QSharedPointer<QSet<QString> > ret;
	
	if (!(ret = cpp_keywords_.toStrongRef())) {
		debug(DEBUG, HIGHLIGHT, "New C++ keywords set");
		ret = QSharedPointer<QSet<QString> >(new QSet<QString>());
		cpp_keywords_ = ret;
		(*ret) << "alignas" << "alignof" << "and" << "and_eq" << "asm" 
			   << "auto" << "bitand" << "bitor" << "bool" << "break" << "case" 
			   << "catch" << "char" << "char16_t" << "char32_t" << "class" 
			   << "compl" << "const" << "constexpr" << "const_cast" << "continue" 
			   << "decltype" << "default" << "delete" << "do" << "double" 
			   << "dynamic_cast" << "else" << "enum" << "explicit" << "export" 
			   << "extern" << "false" << "float" << "for" << "friend" << "goto" 
			   << "if" << "inline" << "int" << "long" << "mutable" << "namespace" 
			   << "new" << "noexcept" << "not" << "not_eq" << "nullptr" 
			   << "operator" << "or" << "or_eq" << "private" << "protected" 
			   << "public" << "register" << "reinterpret_cast" << "return" 
			   << "short" << "signed" << "sizeof" << "static" << "static_assert" 
			   << "static_cast" << "struct" << "switch" << "template" << "this" 
			   << "thread_local" << "throw" << "true" << "try" << "typedef" 
			   << "typeid" << "typename" << "union" << "unsigned" << "using" 
			   << "virtual" << "void" << "volatile" << "wchar_t" << "while" 
			   << "xor" << "xor_eq";
	}
	
	return ret;
}

/**
 * Returns the C++ language vector of formats.
 * @return shared pointer to the vector of formats.
 */

QSharedPointer<std::vector<QTextCharFormat> > highlight_manager::get_cpp_formats()
{
	QSharedPointer<std::vector<QTextCharFormat> > ret;
	
	if (!(ret = cpp_formats_.toStrongRef())) {
		debug(DEBUG, HIGHLIGHT, "New C++ formats set");
		ret = QSharedPointer<std::vector<QTextCharFormat> >(new std::vector<QTextCharFormat>());
		cpp_formats_ = ret;
	}

	return ret;
}

