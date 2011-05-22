#include <iostream>
#include <QPalette>
#include "src_file.h"

using namespace std;

src_file::src_file(const QString file_name, QString content, bool on_disk)
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
    editor->setPlainText(content);
    //editor->setTabStopWidth(num_spaces * font_size_in_pixels);
    editor->setTabStopWidth(4 * 8);
    //editor->document()->setModified(false);   // false by default
    editor->installEventFilter(this);
    
    
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
    else
        file_info = new QFileInfo(file_name);
    
    /* syntax highlighting */
    highlighter = new Highlighter(editor->document());
}

/**
 * Checks if the file was modified or is not saved yet
 */

bool src_file::is_modified()
{
    bool result;
    
    result = editor->document()->isModified();
    cout << "result = " << result << endl;
    
    //return editor->document()->isModified();
    return result;
}

bool src_file::saved_on_disk()
{
    if (file_info->fileName().isEmpty())
        return false;
    
    return true;
}

bool src_file::set_saved_on_disk(bool saved)
{
    editor->document()->setModified(false);
    cout << "set modified to false" << endl;
    //setWindowModified(false);
    //this->saved = saved;
    
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
 * 
 */

QString src_file::get_src_file_name()
{
    return file_info->fileName();
}

/**
 * 
 */

void src_file::set_src_file_name(const QString &fileName)
{
    file_info->setFile(fileName);
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

