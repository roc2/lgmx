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

src_file::src_file(const QString &file_name)
{
	_clone = 0;
	
	//this->setContentsMargins(0, 0, 0, 0);
    //this->setFocusPolicy(Qt::StrongFocus);

    this->setObjectName(QString::fromUtf8("src_editor"));
    //editor->setTabStopWidth(num_spaces * font_size_in_pixels);
    this->setTabStopWidth(4 * 8);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    //this->document()->setModified(false);   // false by default
    this->installEventFilter(this);
    
    // set default font
    QFont initial;
    
	initial.setFamily("monospace");
	initial.setFixedPitch(true);
	initial.setPointSize(10);
    
    this->setFont(initial);
    
    // change editor colors
    QPalette p = this->palette();
    //p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::blue);
    this->setPalette(p);
    
    //scroll_area_ = new QScrollArea(this);
	//scroll_area->setBackgroundRole(QPalette::Dark);
	//scroll_area_->setWidget(this);
    
    //cursor = new QTextCursor(this->textCursor());
    
    /* file properties */
    if (file_name.isEmpty())
        file_info = new QFileInfo();
    else {
        file_info = new QFileInfo(file_name);
        if (!load_file(file_name))      /* reads file from disk */
			throw 0;
    }

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
	delete file_info;
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
 * Checks if the file was modified or is not saved yet
 */

bool src_file::is_modified()
{
    bool result;
    
    result = this->document()->isModified();
    cout << "result = " << result << endl;
    
    return result;
}

bool src_file::saved_on_disk()
{
	return !(file_info->fileName().isEmpty());
}

bool src_file::set_src_file_modified(bool modified)
{
    this->document()->setModified(modified);
    cout << "set modified to false" << endl;
    
    return true;
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
    return file_info->fileName();
}

/**
 * Returns the file path. It does not include the file name
 * @brief Returns the file path. It does not include the file name
 */

QString src_file::get_src_file_path()
{
    return file_info->absolutePath();
}

/**
 * 
 */

void src_file::set_src_file_name(const QString &fileName)
{
    file_info->setFile(fileName);
}

/**
 * Updates file information
 * @brief Updates file information
 */

void src_file::update_src_file_info()
{
    file_info->refresh();
}

/**
 * Returns the complete file name, path + name.
 */

QString src_file::get_src_file_full_name()
{
    return file_info->absoluteFilePath();
}

/**
 * 
 */

bool src_file::get_src_file_full_name(QString &file_path)
{
    file_path = file_info->absoluteFilePath();
    return true;
}

/**
 * 
 */

bool src_file::exists()
{ 
    return file_info->exists();
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
 * Moves cursor to the beginning of the specified line
 * @brief Moves cursor to the beginning of the specified line
 * @param line -> line number
 */

void src_file::go_to_line(int line)
{
    if (line > this->document()->lineCount())
        return;
    
    QTextCursor cursor(textCursor());
    #if 0
    /* move cursor to start of file */
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    /* move cursor down "line - 1" times */
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);
    #endif
    int curr_line = cursor.blockNumber();
    cout << "block number " << curr_line << endl;
    
    if (line > curr_line)
		cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - curr_line - 1);
    else if (line < curr_line)
		cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, curr_line - line - 1);
    
    this->setTextCursor(cursor);
    this->centerCursor();
}

/**
 * 
 */

void src_file::set_clone(src_file *clone)
{
	_clone = clone;
}

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












