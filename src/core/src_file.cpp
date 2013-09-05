#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

#include <src_file.h>
#include <exception.h>
#include <debug.h>
#include <highlight_manager.h>
#include <root_file_container.h>
#include <syntax_highlighter.h>

/**
 * [throw] Constructor.
 */

src_file::src_file(const QString &file_name, unsigned int id, root_file_container *parent, file_settings &settings, highlight_manager *hl_manager, file_type &type_manager) : settings_(settings), type_manager_(type_manager)
{
	id_ = id;
	parent_ = parent;
	highlight_manager_ = hl_manager;

	// file properties
	if (file_name.isEmpty()) {
		type_ = file_type::UNKNOWN;
	} else {
		file_info_.setFile(file_name);
		type_ = type_manager_.get_file_type(get_src_file_extension());
		
		if (!load_file(file_name)) {      // reads file from disk
			lgmx::exception excp("At src_file::src_file(): Unable to read file.");
			throw excp;
		}
	}

	highlighter_ = highlight_manager_->build_highlighter(this);

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
	debug(DEBUG, SRC_FILE, "New file created - ID: " << id_ << " Type: " << file_type::to_string(type_).toStdString());
}

/**
 * Destructor.
 */

src_file::~src_file()
{
	delete highlighter_;
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
 * Returns the file info structure.
 */

QFileInfo* src_file::get_file_info()
{
	return &file_info_;
}

/**
 * Returns the file type.
 */

file_type::type src_file::get_file_type() const
{
	return type_;
}

/**
 * Sets the file type.
 */

void src_file::set_file_type(file_type::type type)
{
	if (type != type_) {
		type_ = type;
		delete highlighter_;
		highlighter_ = highlight_manager_->build_highlighter(this);
		emit fileTypeChanged();
	}
}

/**
 * Returns a copy of the file's content. This method might be 
 * quite slow for big files.
 */

QString src_file::get_content() const
{
	return toPlainText();
}

/**
 * Returns a pointer for the file's content.
 */

QTextDocument *src_file::get_mutable_content()
{
	return this->document();
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
	file_info_.setFile(fileName);
	//update_src_file_info();
	emit modificationChanged(this->is_modified());
}

/**
 * Returns the name of the file (without the path).
 * @return file name without the path.
 */

QString src_file::get_src_file_name() const
{
	return file_info_.fileName();
}

/**
 * Returns the file path. It does not include the file name
 */

QString src_file::get_src_file_path() const
{
	return file_info_.absolutePath();
}

/**
 * Returns the complete file name, path + name.
 */

QString src_file::get_src_file_full_name() const
{
    return file_info_.absoluteFilePath();
}

/**
 * Returns the complete file name (path + name) on the reference parameter.
 */

bool src_file::get_src_file_full_name(QString &file_path) const
{
	file_path = file_info_.absoluteFilePath();
	return true;
}

/**
 * Returns the file name extension. Ex: file.cpp - returns "cpp"
 */

QString src_file::get_src_file_extension() const
{
	return file_info_.suffix().toLower();
}

/**
 * Updates file information
 * @brief Updates file information
 */

void src_file::update_src_file_info()
{
	file_info_.refresh();
}

/**
 * Checks if the file exists on disk.
 */

bool src_file::exists() const
{
    return file_info_.exists();
}

file_settings& src_file::get_settings()
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

