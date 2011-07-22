#include <iostream>
#include <QTextStream>
#include <QPalette>
#include <QMessageBox>
#include <QString>
#include "src_file.h"


using namespace std;

src_file::src_file(const QString file_name)
{
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("untitled"));
    
    horizontalLayout = new QHBoxLayout(tab);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    /* new source code file */
    editor = new CodeEditor(tab);
    editor->setObjectName(QString::fromUtf8("src_editor"));
    //editor->setTabStopWidth(num_spaces * font_size_in_pixels);
    editor->setTabStopWidth(4 * 8);
    //editor->document()->setModified(false);   // false by default
    editor->installEventFilter(this);
    
    // set default font
    QFont initial;
    
	initial.setFamily("monospace");
	initial.setFixedPitch(true);
	initial.setPointSize(12);
    
    editor->setFont(initial);
    
    // change editor colors
    QPalette p = editor->palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    editor->setPalette(p);
        
    //text_document = new QTextDocument(editor->toPlainText(), editor);
    //textEdit->clear();
    
    cursor = new QTextCursor(editor->textCursor());
    
    horizontalLayout->addWidget(editor);
    setLayout(horizontalLayout);
    
    /* file properties */
    if (file_name.isEmpty())
        file_info = new QFileInfo();
    else {
        file_info = new QFileInfo(file_name);
        load_file(file_name);      /* reads file from disk */
    }

    /* syntax highlighting */ // modificar para aplicar somente no que aparece na tela
    highlighter = new Highlighter(editor->document());
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
    //QString test;
    
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    editor->setPlainText(in.readAll());

    QApplication::restoreOverrideCursor();

    return true;
}

/**
 * Checks if the file was modified or is not saved yet
 */

bool src_file::is_modified()
{
    bool result;
    
    result = editor->document()->isModified();
    cout << "result = " << result << endl;
    
    return result;
}

bool src_file::saved_on_disk()
{
    if (file_info->fileName().isEmpty())
        return false;
    
    return true;
}

bool src_file::set_src_file_modified(bool modified)
{
    editor->document()->setModified(modified);
    cout << "set modified to false" << endl;
    
    return true;
}

/**
 * 
 */

QString src_file::get_content()
{
    return editor->toPlainText();
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
	
	out << editor->toPlainText();
	
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
 * 
 */

QString src_file::get_src_file_full_name()
{
    return file_info->absoluteFilePath();
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
    return editor->font();
}

/**
 * Moves cursor to the beginning of the specified line
 * @brief Moves cursor to the beginning of the specified line
 * @param line -> line number
 */

void src_file::go_to_line(int line)
{
    int max_lines;
    
    max_lines = editor->document()->lineCount();
    
    if (line > max_lines)
        return;
    
    /* move cursor to start of file */
    cursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
    /* move cursor down "line - 1" times */
    cursor->movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);
    
    editor->setTextCursor(*cursor);
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

