/********************************************************************************
** Form generated from reading UI file 'go_to_line.ui'
**
** Created: Mon Jan 17 20:59:45 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GO_TO_LINE_H
#define UI_GO_TO_LINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_go_to_line
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ok_button;
    QPushButton *cancel_button;

    void setupUi(QDialog *go_to_line)
    {
        if (go_to_line->objectName().isEmpty())
            go_to_line->setObjectName(QString::fromUtf8("go_to_line"));
        
        go_to_line->resize(252, 127);
        verticalLayout = new QVBoxLayout(go_to_line);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(go_to_line);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        spinBox = new QSpinBox(go_to_line);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setRange(1, 100000000);

        horizontalLayout->addWidget(spinBox);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ok_button = new QPushButton(go_to_line);
        ok_button->setObjectName(QString::fromUtf8("ok_button"));
        ok_button->setEnabled(false);
        ok_button->setDefault(true);

        horizontalLayout_2->addWidget(ok_button);

        cancel_button = new QPushButton(go_to_line);
        cancel_button->setObjectName(QString::fromUtf8("cancel_button"));

        horizontalLayout_2->addWidget(cancel_button);


        verticalLayout->addLayout(horizontalLayout_2);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(spinBox);
#endif // QT_NO_SHORTCUT

        QWidget::setTabOrder(spinBox, ok_button);
        QWidget::setTabOrder(ok_button, cancel_button);

        retranslateUi(go_to_line);

        QMetaObject::connectSlotsByName(go_to_line);
        
        //connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(input_changed()));
        
        
        
        
    } // setupUi

    void retranslateUi(QDialog *go_to_line)
    {
        go_to_line->setWindowTitle(QApplication::translate("go_to_line", "Go to line", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("go_to_line", "Go to line:", 0, QApplication::UnicodeUTF8));
        ok_button->setText(QApplication::translate("go_to_line", "OK", 0, QApplication::UnicodeUTF8));
        cancel_button->setText(QApplication::translate("go_to_line", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class go_to_line: public Ui_go_to_line {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GO_TO_LINE_H
