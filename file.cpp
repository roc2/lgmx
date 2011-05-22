#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QtGui/QApplication>
#include "file.h"

using namespace std;

/**
 * Reads a file from disk and stores its content in "val".
 * @brief Reads a file from disk.
 * @param fileName -> the name of the file.
 * @param val -> buffer to store the file content.
 * @param error -> buffer to return error messages.
 * @return true -> file read ok, false -> error
 */

bool File::read_file(const QString &fileName, QString &val, QString &error)
{
	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		error = file.errorString();
        return false;
	}

	QTextStream in(&file);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);	/* set cursor to "waiting" */
	
	val = in.readAll();		/* read file content */
    file.close();
	
	QApplication::restoreOverrideCursor();	/* restore cursor */

	return true;
}

int File::get_file_name(QString complete_name, string &name)
{
	string::iterator it;
	string aux = complete_name.toStdString();

	it = aux.end();
	
	while (*it != '/' && it != aux.begin())
		it--;
		
	name.assign(it + 1, aux.end());
	cout << "short name = " << name << endl;
	

	return 0;
}

bool File::write_file(const QString &fileName, const QString &content, QString &error)
{
	QFile file(fileName);

	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		//QMessageBox::warning(this, tr("Application"),
				  //tr("Cannot write file %1:\n%2.")
				  //.arg(fileName)
				  //.arg(file.errorString()));
		error = file.errorString();
		return false;
	}

	QTextStream out(&file);
	
	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	out << content;
	
	QApplication::restoreOverrideCursor();
	
	return true;
}

