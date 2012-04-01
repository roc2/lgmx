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

class Ui_SaveFiles
{
public:
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

    void setupUi(QDialog *SaveFiles)
    {
        if (SaveFiles->objectName().isEmpty())
            SaveFiles->setObjectName(QString::fromUtf8("SaveFiles"));
        SaveFiles->resize(470, 342);
        horizontalLayout_2 = new QHBoxLayout(SaveFiles);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        warnLabel = new QLabel(SaveFiles);
        warnLabel->setObjectName(QString::fromUtf8("warnLabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("Sans Serif"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        warnLabel->setFont(font);
        warnLabel->setWordWrap(true);

        verticalLayout->addWidget(warnLabel);

        selectLabel = new QLabel(SaveFiles);
        selectLabel->setObjectName(QString::fromUtf8("selectLabel"));

        verticalLayout->addWidget(selectLabel);

        fileTable = new QTableView(SaveFiles);
        fileTable->setObjectName(QString::fromUtf8("fileTable"));

        verticalLayout->addWidget(fileTable);

        lostLabel = new QLabel(SaveFiles);
        lostLabel->setObjectName(QString::fromUtf8("lostLabel"));

        verticalLayout->addWidget(lostLabel);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        closeWithoutSaveButton = new QPushButton(SaveFiles);
        closeWithoutSaveButton->setObjectName(QString::fromUtf8("closeWithoutSaveButton"));

        horizontalLayout->addWidget(closeWithoutSaveButton);

        cancelButton = new QPushButton(SaveFiles);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        saveButton = new QPushButton(SaveFiles);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        horizontalLayout->addWidget(saveButton);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);


        retranslateUi(SaveFiles);

        QMetaObject::connectSlotsByName(SaveFiles);
    } // setupUi

    void retranslateUi(QDialog *SaveFiles)
    {
        SaveFiles->setWindowTitle(QApplication::translate("SaveFiles", "SaveFiles", 0, QApplication::UnicodeUTF8));
        warnLabel->setText(QApplication::translate("SaveFiles", "There are x documents with unsaved changes. Save changes before closing?", 0, QApplication::UnicodeUTF8));
        selectLabel->setText(QApplication::translate("SaveFiles", "Select the documents you want to save:", 0, QApplication::UnicodeUTF8));
        lostLabel->setText(QApplication::translate("SaveFiles", "All unsaved changes will be permanently lost.", 0, QApplication::UnicodeUTF8));
        closeWithoutSaveButton->setText(QApplication::translate("SaveFiles", "Close &without Saving", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("SaveFiles", "&Cancel", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("SaveFiles", "&Save", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SaveFiles: public Ui_SaveFiles {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SAVEFILEDIAG_H
