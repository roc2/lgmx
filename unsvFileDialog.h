/********************************************************************************
** Form generated from reading UI file 'savefiles.ui'
**
** Created: Mon Jun 6 20:33:57 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SAVEFILEDIAG_H
#define SAVEFILEDIAG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class UnsavedFileDialog : public QDialog
{
    Q_OBJECT
    
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *warnLabel;
    QLabel *selectLabel;
    QTableView *fileTable;
    QLabel *lostLabel;
    QHBoxLayout *horizontalLayout;
    QPushButton *closeWithoutSaveButton;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    public:
    
    UnsavedFileDialog(QWidget *parent, QStringList &files);

    private slots:
    //void cancel();
};
/*
namespace Ui {
    class SaveFiles: public Ui_SaveFiles {};
} // namespace Ui
*/
QT_END_NAMESPACE

#endif // SAVEFILEDIAG_H
