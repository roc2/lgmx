#include <iostream>
#include <QTextStream>
#include <QPalette>
#include <QMessageBox>
#include <QString>
#include "src_file.h"
#include "src_container.h"

using namespace std;

/**
 * Constructor.
 */

src_file::src_file(const QString &file_name, unsigned int id)
{
	clone_ = false;
	child_file_[0] = NULL;
	child_file_[1] = NULL;

	id_ = id;

	//this->setContentsMargins(0, 0, 0, 0);
    //this->setFocusPolicy(Qt::StrongFocus);

    this->setObjectName(QString::fromUtf8("src_editor"));
    //editor->setTabStopWidth(num_spaces * font_size_in_pixels);
    this->setTabStopWidth(4 * 8);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    //this->document()->setModified(false);   // false by default
    this->installEventFilter(this);
    
    // set default font
    set_default_font();
    
    // change editor colors
    QPalette p(this->palette());
    //p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::cyan);
    this->setPalette(p);
    
    //scroll_area_ = new QScrollArea(this);
	//scroll_area->setBackgroundRole(QPalette::Dark);
	//scroll_area_->setWidget(this);
    
    //cursor = new QTextCursor(this->textCursor());
    
    /* file properties */
	if (file_name.isEmpty())
		file_info_ = new QFileInfo();
	else {
		file_info_ = new QFileInfo(file_name);
		if (!load_file(file_name)) {      /* reads file from disk */
			delete file_info_;
			throw 0;
		}
	}

	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));

	//this->setFocus(Qt::OtherFocusReason);
	//this->setFocus();
    /* syntax highlighting */ // modificar para aplicar somente no que aparece na tela
    //highlighter = new Highlighter(this->document());
    //this->print_visible_blocks();
    //ht = new hilight_thread(this->document());
    //ht->start();
}

/**
 * Constructor for clone files. The file is created with the same 
 * QTextDocument from base_file.
 */

src_file::src_file(src_file *base_file)
{
	clone_ = true;
	child_file_[0] = NULL;
	child_file_[1] = NULL;

	id_ = base_file->get_id();
	
	//this->setContentsMargins(0, 0, 0, 0);
    //this->setFocusPolicy(Qt::StrongFocus);

    this->setObjectName(QString::fromUtf8("src_editor"));
    //editor->setTabStopWidth(num_spaces * font_size_in_pixels);
    this->setTabStopWidth(4 * 8);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    //this->document()->setModified(false);   // false by default
    this->installEventFilter(this);
    
    // set default font
    set_default_font();
    
    // change editor colors
    QPalette p = this->palette();
    p.setColor(QPalette::Base, Qt::white);
    //p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::blue);
    this->setPalette(p);
    
    //scroll_area_ = new QScrollArea(this);
	//scroll_area->setBackgroundRole(QPalette::Dark);
	//scroll_area_->setWidget(this);
    
    //cursor = new QTextCursor(this->textCursor());
    
    /* file properties */
	file_info_ = base_file->get_file_info();
	set_content(base_file->get_mutable_content());


	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));

	//this->setFocus(Qt::OtherFocusReason);
	//this->setFocus();
    /* syntax highlighting */ // modificar para aplicar somente no que aparece na tela
    //highlighter = new Highlighter(this->document());
    //this->print_visible_blocks();
}

/**
 * Destructor.
 */

src_file::~src_file()
{
	cout << "destroying file..." << endl;
	
	//delete scroll_area_;
	//delete cursor;
	
	if (!clone_)
		delete file_info_;
	//delete highlighter;
}

/**
 * 
 */

/*
 QFile file("in.txt");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = in.readLine();
         process_line(line);
     }
*/

// teria q rodar alguma coisa tipo o codigo acima em uma thread separada



bool src_file::load_file(const QString &fileName)
{
    QFile file(fileName);
    
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    this->setPlainText(in.readAll());

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
 * 
 */

bool src_file::saved_on_disk()
{
	return !(file_info_->fileName().isEmpty());
}

/**
 * 
 */

void src_file::set_file_info(QFileInfo *file_info)
{
	if (file_info)
		file_info_ = file_info;
}

/**
 * 
 */

QFileInfo* src_file::get_file_info() const
{
	return file_info_;
}

/**
 * 
 */

QString src_file::get_content()
{
    return this->toPlainText();
}

QTextDocument *src_file::get_mutable_content()
{
	return this->document();
}

void src_file::set_content(QTextDocument *content)
{
	this->setDocument(content);
	QObject::connect(this->document(), SIGNAL(modificationChanged(bool)), this, SIGNAL(modificationChanged(bool)));
}

/**
 * 
 */

void src_file::set_child_src_file(src_file *child, int index)
{
	cout << child << endl;
	if (!child) {
		cout << "Invalid child file" << endl;
		return;
	}

	child_file_[index] = child;
}

unsigned int src_file::get_id() const
{
	return id_;
}

QTextCursor src_file::get_cursor()
{
	return this->textCursor();
}

int src_file::get_cursor_position()
{
	return this->textCursor().position();
}

void src_file::set_cursor(const QTextCursor &cursor)
{
	this->setTextCursor(cursor);
}

/**
 * Writes file content to disk
 */

bool src_file::write_file(const QString &fileName)
{
    QFile file(fileName);
    QString error;

	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::critical(this, tr("Application"),
				  tr("Cannot write file %1:\n%2.")
				  .arg(fileName)
				  .arg(file.errorString()));
		error = file.errorString();
		return false;
	}

	QTextStream out(&file);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	out << this->toPlainText();
	
	QApplication::restoreOverrideCursor();
    
    return true;
}

/**
 * Returns the name of the file (without the path)
 * @brief Returns the name of the file (without the path)
 * @return 
 */

QString src_file::get_src_file_name()
{
    return file_info_->fileName();
}

/**
 * Returns the file path. It does not include the file name
 * @brief Returns the file path. It does not include the file name
 */

QString src_file::get_src_file_path()
{
    return file_info_->absolutePath();
}

/**
 * 
 */

void src_file::set_src_file_name(const QString &fileName)
{
    file_info_->setFile(fileName);
    //update_src_file_info();
    emit modificationChanged(this->is_modified());
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
 * Returns the complete file name, path + name.
 */

QString src_file::get_src_file_full_name()
{
    return file_info_->absoluteFilePath();
}

/**
 * 
 */

bool src_file::get_src_file_full_name(QString &file_path)
{
    file_path = file_info_->absoluteFilePath();
    return true;
}

/**
 * 
 */

bool src_file::exists()
{
    return file_info_->exists();
}

/**
 * 
 */

QFont src_file::get_font()
{
    return this->font();
}

void src_file::set_font(QFont &font)
{
    this->setFont(font);
}

/**
 * Sets the source file default font.
 */

void src_file::set_default_font()
{
	QFont initial("monospace", 10);
    initial.setFixedPitch(true);
    
    this->setFont(initial);
}

/**
 * Moves cursor to the beginning of the specified line.
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
 * 
 */
/*
void src_file::set_clone(src_file *clone)
{
	_clone = clone;
}
*/
/**
 * 
 */

void src_file::focusInEvent(QFocusEvent *event)
{
	cout << "src_file widget has focus!!" << endl;
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
            std::cout << "VI mode on!!!" << std::endl;

            return true;
        } else if (vi_mode && PressedKey == Qt::Key_I) {
            
            vi_mode = false;
            std::cout << "Insert mode on!!!" << std::endl;
            
            return true;
        }
    }

    // standard event processing
    return QObject::eventFilter(pObject, pEvent);
}












