#include <QTextStream>
#include <QPalette>
#include <QMessageBox>
#include <QString>
#include <QFileInfo>
#include <QTextBlock>
#include <QApplication>
#include <QScrollBar>
#include <QTextOption>
#include <QtGui/QPainter>

#include <src_file.h>
#include <exception.h>
#include <debug.h>
#include <highlight_manager.h>
#include <src_container.h>
#include <root_file_container.h>
#include <settings.h>
#include <syntax_highlighter.h>
#include <settings.h>
#include <QRect>

/**
 * [throw] Constructor.
 */
/*
src_file::src_file(const QString &file_name, unsigned int id, src_container *parent, Settings &settings, highlight_manager *hl_manager) : settings_(settings)
{
	id_ = id;
	parent_ = parent;
	type_ = file_type::UNKNOWN;
	blink_cursor_ = false;

	this->setContentsMargins(0, 0, 0, 0);
	this->setObjectName(QString::fromUtf8("src_editor"));
	this->installEventFilter(this);

	this->setTabStopWidth(parent->get_settings()->get_tab_width() * 8);
	//this->setLineWrapMode(QPlainTextEdit::NoWrap);
	//set_line_wrap(parent->get_settings()->get_line_wrap());

	//this->setOverwriteMode(true);

	// file properties
	if (file_name.isEmpty()) {
		file_info_ = new QFileInfo();
	} else {
		file_info_ = new QFileInfo(file_name);
		if (!load_file(file_name)) {      // reads file from disk
			delete file_info_;
			lgmx::exception excp("At src_file::src_file(): Unable to read file.");
			throw excp;
		}
	}

	if (hl_manager) {
		highlight_manager_ = hl_manager;
		highlighter_ = highlight_manager_->build_highlighter(this);
	} else {
		highlight_manager_ = NULL;
		highlighter_ = NULL;
	}

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}
*/
src_file::src_file(const QString &file_name, unsigned int id, root_file_container *parent, Settings &settings, highlight_manager *hl_manager) : settings_(settings)
{
	id_ = id;
	rparent_ = parent;
	type_ = file_type::UNKNOWN;
	blink_cursor_ = false;

	this->setContentsMargins(0, 0, 0, 0);
	this->setObjectName(QString::fromUtf8("src_editor"));
	this->installEventFilter(this);

	//this->setTabStopWidth(parent->get_settings()->get_tab_width() * 8);
	//this->setLineWrapMode(QPlainTextEdit::NoWrap);
	//set_line_wrap(parent->get_settings()->get_line_wrap());

	//this->setOverwriteMode(true);

	/* file properties */
	if (file_name.isEmpty()) {
		file_info_ = new QFileInfo();
	} else {
		file_info_ = new QFileInfo(file_name);
		if (!load_file(file_name)) {      /* reads file from disk */
			delete file_info_;
			lgmx::exception excp("At src_file::src_file(): Unable to read file.");
			throw excp;
		}
	}

	if (hl_manager) {
		highlight_manager_ = hl_manager;
		highlighter_ = highlight_manager_->build_highlighter(this);
	} else {
		highlight_manager_ = NULL;
		highlighter_ = NULL;
	}

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

/**
 * Constructor for clone files. This is not a copy constructor, since some
 * structures from the original file are shared. The file is created with
 * the same QTextDocument and QFileInfo from base_file.
 */
/*
src_file::src_file(src_file *base_file, src_container *parent) : settings_(base_file->get_settings())
{
	id_ = base_file->id_;
	parent_ = parent;
	type_ = base_file->type_;
	blink_cursor_ = false;

	cursor_visible_ = true;
	mb_format_.setBackground(Qt::red);

	// get file info and content from the base file
	file_info_ = base_file->file_info_;
	//set_content(base_file->get_mutable_content());
	set_content(base_file->document());

	this->setContentsMargins(0, 0, 0, 0);
	this->setObjectName(QString::fromUtf8("src_editor"));
	this->setTabStopWidth(parent->get_settings()->get_tab_width() * 8);
	//this->setLineWrapMode(QPlainTextEdit::NoWrap);
	//set_line_wrap(parent->get_settings()->get_line_wrap());
	this->installEventFilter(this);

	highlight_manager_ = NULL;
	//highlighter_ = NULL;
	highlighter_ = base_file->highlighter_;

	//setCursorWidth(get_font_width());
	//setCursorWidth(0);


	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));

	QScrollBar *scroll = verticalScrollBar();
	QObject *obj = (QObject *) scroll;		// ?????
	QObject::connect(obj, SIGNAL(valueChanged(int)), this, SLOT(highlight(int)));

	// when cursor changes highlight
	QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlight()));
}*/

/**
 * Destructor.
 */

src_file::~src_file()
{
	if (highlighter_)
		delete highlighter_;

	delete file_info_;
}

/**
 * Reads file from disk and loads it to QPlainTextEdit.
 * @param fileName -> complete path of the file to be loaded.
 * @return true, if file loaded ok, false otherwise.
 */

bool src_file::load_file(const QString &fileName)
{
	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox msgBox;
		msgBox.setWindowTitle(tr("Open"));
		msgBox.setText(tr("Cannot read file %1:\n%2.") .arg(fileName) .arg(file.errorString()));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();

		return false;
	}

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	this->setPlainText(in.readAll());
	file.close();
	QApplication::restoreOverrideCursor();

	return true;
}

/**
 * Writes file content to disk.
 * @param fileName -> complete path of the file to be written.
 * @return true, if file written ok, false otherwise.
 */

bool src_file::write_file(const QString &fileName)
{
	QFile file(fileName);

	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, "lgmx",
				  tr("Cannot write file %1:\n%2.")
				  .arg(fileName)
				  .arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << this->toPlainText();
	file.close();
	QApplication::restoreOverrideCursor();

	return true;
}

/**
 * Checks whether the file has unsaved modifications or not.
 * @return true, if file has unsaved modifications, false otherwise.
 */

bool src_file::is_modified() const
{
	return document()->isModified();
}

/**
 * Sets the file modification state.
 * @param modified -> true, to mark the file as modified, false to mark the file
 * with no modifications.
 */

void src_file::set_modified(bool modified)
{
	document()->setModified(modified);
	//emit modificationChanged(modified);
}

/**
 * @todo maybe pull out this method
 * Sets the file_info structure. This method applies for clone files only.
 * @param file_info -> new file info structure.
 */

void src_file::set_file_info(QFileInfo *file_info)
{
	file_info_ = file_info;
}

/**
 * Returns the file info structure.
 */

QFileInfo* src_file::get_file_info() const
{
	return file_info_;
}

/**
 * Returns the file type.
 */

file_type::type src_file::get_file_type() const
{
	return type_;
}

/**
 * Sets file type.
 */

void src_file::set_file_type(file_type::type type)
{
	/*
	this must be done in the root file only, and the changes propagated to
	the clone files.
	if (type != type_) {
		if (highlighter_)
			delete highlighter_;

		highlighter_ = highlight_manager_->build_highlighter(this);
	}*/

	type_ = type;
}

/**
 * Returns a copy of the file's content.
 */

QString src_file::get_content() const
{
    return this->toPlainText();
}

/**
 * Returns a pointer for the file's content.
 */

QTextDocument *src_file::get_mutable_content()
{
	return this->document();
}

/**
 * @todo pull this out
 * Sets the content of the file.
 * @param content - pointer to the new file content.
 */

void src_file::set_content(QTextDocument *content)
{
	this->setDocument(content);
	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

/**
 * Returns the file unique ID.
 */

unsigned int src_file::get_id() const
{
	return id_;
}

/**
 * Gets a copy of the file cursor.
 */

QTextCursor src_file::get_cursor()
{
	return this->textCursor();
}

/**
 * Returns the current position of the text cursor.
 */

int src_file::get_cursor_position()
{
	return this->textCursor().position();
}

/**
 * Sets the file cursor.
 * @param cursor - the cursor to be set.
 */

void src_file::set_cursor(const QTextCursor &cursor)
{
	this->setTextCursor(cursor);
}

/**
 * Sets the file name.
 * @param fileName - the new file name.
 */

void src_file::set_src_file_name(const QString &fileName)
{
	file_info_->setFile(fileName);
	//update_src_file_info();
	emit modificationChanged(this->is_modified());
}

/**
 * Returns the name of the file (without the path).
 * @return file name without the path.
 */

QString src_file::get_src_file_name() const
{
	return file_info_->fileName();
}

/**
 * Returns the file path. It does not include the file name
 */

QString src_file::get_src_file_path() const
{
	return file_info_->absolutePath();
}

/**
 * Returns the complete file name, path + name.
 */

QString src_file::get_src_file_full_name() const
{
    return file_info_->absoluteFilePath();
}

/**
 * Returns the complete file name (path + name) on the reference parameter.
 */

bool src_file::get_src_file_full_name(QString &file_path) const
{
	file_path = file_info_->absoluteFilePath();
	return true;
}

/**
 * Returns the file name extension. Ex: file.cpp - returns "cpp"
 */

QString src_file::get_src_file_extension() const
{
	return file_info_->suffix().toLower();
}

/**
 * Updates file information
 * @brief Updates file information
 */

void src_file::update_src_file_info()
{
	file_info_->refresh();
}

/**
 * Checks if the file exists on disk.
 */

bool src_file::exists() const
{
    return file_info_->exists();
}

Settings& src_file::get_settings()
{
	return settings_;
}

syntax_highlighter *src_file::get_highlighter()
{
	return highlighter_;
}

/**
 *
 */

QTextBlock src_file::get_text_block(int block)
{
	return document()->findBlockByNumber(block);
}

