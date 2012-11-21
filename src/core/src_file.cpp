#include <QTextStream>
#include <QPalette>
#include <QMessageBox>
#include <QString>
#include <QFileInfo>
#include <QTextBlock>
#include <QtGui/QApplication>

#include <src_file.h>
#include <exception.h>
#include <debug.h>
#include <highlight_manager.h>


/**
 * [throw] Constructor.
 */

src_file::src_file(const QString &file_name, unsigned int id, highlight_manager *hl_manager)
{
	clone_ = false;
	id_ = id;
	type_ = file_type::UNKNOWN;

	this->setContentsMargins(0, 0, 0, 0);
    this->setObjectName(QString::fromUtf8("src_editor"));
    this->setTabStopWidth(4 * 8);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    this->installEventFilter(this);
    
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
		highlighter_ = highlight_manager_->build_highlighter(*this);
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

src_file::src_file(src_file *base_file)
{
	clone_ = true;
	id_ = base_file->get_id();
	type_ = base_file->get_file_type();
	
	this->setContentsMargins(0, 0, 0, 0);
    this->setObjectName(QString::fromUtf8("src_editor"));
    //editor->setTabStopWidth(num_spaces * font_size_in_pixels);
    this->setTabStopWidth(4 * 8);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    this->installEventFilter(this);

    highlight_manager_ = NULL;
	highlighter_ = NULL;
    
    // set default font
    set_default_font();
    
    // change editor colors
    set_base_color(Qt::white);
    set_text_color(Qt::black);

    /* get file info and content from the base file */
	file_info_ = base_file->get_file_info();
	set_content(base_file->get_mutable_content());

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

/**
 * Destructor.
 */

src_file::~src_file()
{
	if (!clone_) {
		if (highlighter_)
			delete highlighter_;

		delete file_info_;
	}
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
        QMessageBox::critical(this, "lgmx", tr("Cannot read file %1:\n%2.") .arg(fileName) .arg(file.errorString()));

        return false;
    }

    QTextStream in(&file);
    this->setPlainText(in.readAll());

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
	QApplication::restoreOverrideCursor();
    
    return true;
}

/**
 * Checks whether the file has unsaved modifications or not.
 * @return true, if file has unsaved modifications, false otherwise.
 */

bool src_file::is_modified() const
{
    return this->document()->isModified();
}

/**
 * Sets the file modification state.
 * @param modified -> true, to mark the file as modified, false to mark the file
 * with no modifications.
 */

void src_file::set_modified(bool modified)
{
    this->document()->setModified(modified);
    emit modificationChanged(modified);
}

/**
 * Sets the file_info structure. This method applies for clone files only.
 * @param file_info -> new file info structure.
 */

void src_file::set_file_info(QFileInfo *file_info)
{
	if (file_info && clone_)
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
 * Sets the source file background color.
 * @param color - the color to be applied.
 */

void src_file::set_base_color(const QColor &color)
{
    QPalette p(this->palette());
    p.setColor(QPalette::Base, color);
    this->setPalette(p);
}

/**
 * Sets the source file text color.
 * @param color - the color to be applied.
 */

void src_file::set_text_color(const QColor &color)
{
    QPalette p(this->palette());
    p.setColor(QPalette::Text, color);
    this->setPalette(p);
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
	this->centerCursor();
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

/**
 * Returns the file font.
 */

QFont src_file::get_font() const
{
    return this->font();
}

/**
 * Sets the file font.
 * @param font -> font type.
 */

void src_file::set_font(QFont &font)
{
    this->setFont(font);
}

/**
 * Sets the source file default font.
 */

void src_file::set_default_font()
{
	QFont def("monospace", 10);
    def.setFixedPitch(true);
    
    this->setFont(def);
}

/**
 * [slot] Moves the cursor to the beginning of the specified line.
 * @param line -> line number
 */

void src_file::go_to_line(int line)
{
    if (line > this->document()->lineCount())
        return;
    
    QTextCursor cursor(textCursor());
    int pos;
    
    pos = this->document()->findBlockByLineNumber(line - 1).position();
    cursor.setPosition(pos, QTextCursor::MoveAnchor);
    
    this->setTextCursor(cursor);
    this->centerCursor();
}

/**
 * Focus in event handler.
 */

void src_file::focusInEvent(QFocusEvent *event)
{
	debug(DEBUG, SRC_FILE, "src_file widget has focus!!");
}

/**
 * Focus out event handler.
 */

void src_file::focusOutEvent(QFocusEvent *event)
{
	debug(DEBUG, SRC_FILE, "Focus out!!");
}

/**
 * Events filter. For VI mode and shortcuts
 * 
 */

bool vi_mode = 0;

bool src_file::eventFilter(QObject* pObject, QEvent* pEvent)
{

    if (pEvent->type() == QEvent::KeyPress) {

        QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(pEvent);
        int PressedKey = pKeyEvent->key();

        if (PressedKey == Qt::Key_Escape) {
            
            vi_mode = true;
            debug(DEBUG, SRC_FILE, "VI mode on!!!");

            return true;
        } else if (vi_mode && PressedKey == Qt::Key_I) {
            
            vi_mode = false;
            debug(DEBUG, SRC_FILE, "Insert mode on!!!");
            
            return true;
        }
    }

    // standard event processing
    return QObject::eventFilter(pObject, pEvent);
}

