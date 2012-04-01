#include "unsvFileDialog.h"


/**
 * 
 */

UnsavedFileDialog::UnsavedFileDialog(QWidget *parent, QStringList &files) : QDialog(parent)
{
    this->setObjectName(QString::fromUtf8("SaveFiles"));
        
    this->resize(470, 342);
    horizontalLayout_2 = new QHBoxLayout(this);
    horizontalLayout_2->setSpacing(6);
    horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    
    verticalLayout_2 = new QVBoxLayout();
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    
    QFont font;
    font.setFamily(QString::fromUtf8("Sans Serif"));
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);
    
    warnLabel = new QLabel(this);
    warnLabel->setObjectName(QString::fromUtf8("warnLabel"));
    warnLabel->setFont(font);
    warnLabel->setWordWrap(true);

    selectLabel = new QLabel(this);
    selectLabel->setObjectName(QString::fromUtf8("selectLabel"));
    
    fileTable = new QTableView(this);
    fileTable->setObjectName(QString::fromUtf8("fileTable"));
    
    lostLabel = new QLabel(this);
    lostLabel->setObjectName(QString::fromUtf8("lostLabel"));
    
    verticalLayout->addWidget(warnLabel);
    verticalLayout->addWidget(selectLabel);
    verticalLayout->addWidget(fileTable);
    verticalLayout->addWidget(lostLabel);

    verticalLayout_2->addLayout(verticalLayout);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    
    closeWithoutSaveButton = new QPushButton(this);
    closeWithoutSaveButton->setObjectName(QString::fromUtf8("closeWithoutSaveButton"));

    cancelButton = new QPushButton(this);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    saveButton = new QPushButton(this);
    saveButton->setObjectName(QString::fromUtf8("saveButton"));
    
    horizontalLayout->addWidget(closeWithoutSaveButton);
    horizontalLayout->addWidget(cancelButton);
    horizontalLayout->addWidget(saveButton);

    verticalLayout_2->addLayout(horizontalLayout);
    horizontalLayout_2->addLayout(verticalLayout_2);

    setWindowTitle(QApplication::translate("SaveFiles", "SaveFiles", 0, QApplication::UnicodeUTF8));
    warnLabel->setText(QApplication::translate("SaveFiles", "There are x documents with unsaved changes. Save changes before closing?", 0, QApplication::UnicodeUTF8));
    selectLabel->setText(QApplication::translate("SaveFiles", "Select the documents you want to save:", 0, QApplication::UnicodeUTF8));
    lostLabel->setText(QApplication::translate("SaveFiles", "All unsaved changes will be permanently lost.", 0, QApplication::UnicodeUTF8));
    closeWithoutSaveButton->setText(QApplication::translate("SaveFiles", "Close &without Saving", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("SaveFiles", "&Cancel", 0, QApplication::UnicodeUTF8));
    saveButton->setText(QApplication::translate("SaveFiles", "&Save", 0, QApplication::UnicodeUTF8));

    QMetaObject::connectSlotsByName(this);
    
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    
}



