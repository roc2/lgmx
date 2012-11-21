#include "search.h"
#include <QLayout>

#include <iostream>
#include <view_manager.h>

#define SEARCH_SAVE_KEY	"search_flags"

using namespace std;

lgmx::search::search(view_manager &manager, QWidget *parent) : QDialog(parent), 
		       flags(QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords), manager_(manager)
{
	wrap = false;
	highlight_all = true;
	cout << "flags: " << int(flags) << endl;
	
	match_format.setBackground(Qt::darkBlue);

	setup_ui(this);
	load_settings();
}

/**
 * Destructor.
 */

lgmx::search::~search()
{
	cout << "search destructor" << endl;
	save_settings();
}

/**
 * [slot] Updates the pointer to the current file.
 */

void lgmx::search::update_current_file(int)
{
	src_ctr = manager_.get_current_view()->get_src_container();
	
	/* Update current tab focus */
	QObject::connect(src_ctr, SIGNAL(currentChanged(int)), this, SLOT(update_current_file(int)));

	curr_file_ = src_ctr->get_current_src_file();
}

/**
 * 
 */

void lgmx::search::show_search_dialog()
{
	update_current_file(0);
	
	if (curr_file_)
		this->show();
		
	cout << "show" << endl;
}

/**
 * Saves current settings.
 */

void lgmx::search::save_settings()
{
	QSettings settings;
	uint p_flags = 0;	// persistent flags
	
	if (regex)
		p_flags |= REGEX;
	
	if ((flags & QTextDocument::FindCaseSensitively) == QTextDocument::FindCaseSensitively)
		p_flags |= CASE_SENSITIVE;
	
	if ((flags & QTextDocument::FindWholeWords) == QTextDocument::FindWholeWords)
		p_flags |= WHOLE_WORDS;
	
	if (highlight_all)
		p_flags |= HIGHLIGHT_ALL;
	
	if (wrap)
		p_flags |= WRAP_AROUND;
	
	QVariant v(p_flags);	
	settings.setValue(SEARCH_SAVE_KEY, v);
}

/**
 * Loads saved settings.
 */

void lgmx::search::load_settings()
{
	QSettings settings;
	
	if (settings.contains(SEARCH_SAVE_KEY)) {

		QVariant v(settings.value(SEARCH_SAVE_KEY));
	
		uint p_flags = settings.value(SEARCH_SAVE_KEY).toUInt();

		// use regex
		if ((p_flags & REGEX) == REGEX) {
			regex = true;
			checkBox->setChecked(true);
		} else {
			regex = false;
			checkBox->setChecked(false);
		}
		
		// case sensitive
		if ((p_flags & CASE_SENSITIVE) == CASE_SENSITIVE) {
			case_sensitive(true);
			checkBox_2->setChecked(true);
		} else {
			case_sensitive(false);
			checkBox_2->setChecked(false);
		}
		
		// match whole words only
		if ((p_flags & WHOLE_WORDS) == WHOLE_WORDS) {
			match_whole_words(true);
			checkBox_3->setChecked(true);
		} else {
			match_whole_words(false);
			checkBox_3->setChecked(false);
		}
		
		// highlight all matches
		if ((p_flags & HIGHLIGHT_ALL) == HIGHLIGHT_ALL) {
			highlight_all = true;
			checkBox_4->setChecked(true);
		} else {
			highlight_all = false;
			checkBox_4->setChecked(false);
		}
		
		// wrap around
		if ((p_flags & WRAP_AROUND) == WRAP_AROUND) {
			wrap = true;
			checkBox_6->setChecked(true);
		} else {	
			wrap = false;
			checkBox_6->setChecked(false);
		}
	} else {
		set_default_settings();
	}
}

/**
 * Sets default settings.
 */

void lgmx::search::set_default_settings()
{
	regex = true;
	checkBox->setChecked(true);
	
	case_sensitive(true);
	checkBox_2->setChecked(true);
	
	match_whole_words(false);
	checkBox_3->setChecked(false);
	
	highlight_all = true;
	checkBox_4->setChecked(true);
	
	wrap = false;
	checkBox_6->setChecked(false);
}

void lgmx::search::hide_search_dialog()
{
	this->hide();
}


void lgmx::search::highlight_all_matches(QString &pattern)
{
	//int QTextCursor::position ()
	//this->cursor.setCharFormat(match_format);
}

/**
 * 
 */

void lgmx::search::search_string(QString &pattern)
{
	src_file *curr_file;

	if (!curr_file_) {
		cout << "nop" << endl;
		// update current file!!!
		return;
	}

	if (!(curr_file = src_ctr->get_current_src_file()))
		return;
	
	this->cursor = curr_file_->get_cursor();
	
	if (regex) {
		regex_pattern.setPattern(pattern);
		this->cursor = curr_file_->get_mutable_content()->find(regex_pattern, this->cursor, flags);
	} else
		this->cursor = curr_file_->get_mutable_content()->find(pattern, this->cursor, flags);


	if (!this->cursor.isNull()) {
		
		curr_file_->set_cursor(this->cursor);
		curr_file_->centerCursor();
		
		if (highlight_all)
			highlight_all_matches(pattern);
		
		
	} else if (wrap) {
		// get cursor position again, since we cannot move a null cursor
		this->cursor = curr_file_->get_cursor();
		
		// if search is forward move to the beginning of file, otherwise move to the end
		if ((flags & QTextDocument::FindBackward) == QTextDocument::FindBackward)
			this->cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
		else
			this->cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		
		// search again
		if (regex) {
			this->cursor = curr_file_->get_mutable_content()->find(regex_pattern, this->cursor, flags);
		} else
			this->cursor = curr_file_->get_mutable_content()->find(pattern, this->cursor, flags);
			
		if (!this->cursor.isNull()) {
			curr_file_->set_cursor(this->cursor);
			curr_file_->centerCursor();
		}
	}
}

/**
 * Finds next pattern match.
 */

void lgmx::search::find_next()
{
	QString pattern(search_cbox->currentText());
		
	if (pattern.isEmpty())
		return;
	
	if ((flags & QTextDocument::FindBackward) == QTextDocument::FindBackward)
		flags ^= QTextDocument::FindBackward;

	search_string(pattern);
	add_to_search_history(pattern);
}

/**
 * Finds previous pattern match.
 */

void lgmx::search::find_previous()
{
	QString pattern(search_cbox->currentText());
	
	if (pattern.isEmpty())
		return;

	flags |= QTextDocument::FindBackward;

	search_string(pattern);
	add_to_search_history(pattern);
}

/**
 * Replaces current pattern match.
 */

void lgmx::search::replace()
{
	src_file *curr_file;
	
	if (!(curr_file = src_ctr->get_current_src_file()))
		return;
	
	search_pattern = search_cbox->currentText();
	replace_pattern = comboBox->currentText();

	this->cursor = curr_file->get_cursor();

	if (this->cursor.selectedText() == search_pattern) {
		this->cursor.insertText(replace_pattern);
	}
}

/**
 * Replaces current pattern match and finds next match.
 */

void lgmx::search::replace_and_next()
{
	replace();
	find_next();
}

/**
 * Replaces current pattern match and finds previous match.
 */

void lgmx::search::replace_and_previous()
{
	replace();
	find_previous();
}

/**
 * Replaces all matches.
 */

void lgmx::search::replace_all()
{
	if (!curr_file_) {
		cout << "nop" << endl;
		// update current file!!!
		return;
	}

	QString pattern(search_cbox->currentText());
	QString replace_pattern(comboBox->currentText());

	cursor = curr_file_->get_cursor();
	// set cursor to the beginning of file
	cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

	do {
		if (regex) {
			regex_pattern.setPattern(pattern);
			cursor = curr_file_->get_mutable_content()->find(regex_pattern, this->cursor, flags);
		} else {
			cursor = curr_file_->get_mutable_content()->find(pattern, this->cursor, flags);
		}

		if (!cursor.isNull())	// replace
			cursor.insertText(replace_pattern);

	} while (!cursor.isNull());
}

/**
 * 
 */

void lgmx::search::match_whole_words(bool checked)
{
	if (checked)
		flags |= QTextDocument::FindWholeWords;
	else if ((flags & QTextDocument::FindWholeWords) == QTextDocument::FindWholeWords)
		flags ^= QTextDocument::FindWholeWords;
}

/**
 * 
 */

void lgmx::search::case_sensitive(bool checked)
{
	if (checked) {
		flags |= QTextDocument::FindCaseSensitively;
		regex_pattern.setCaseSensitivity(Qt::CaseSensitive);
	} else if ((flags & QTextDocument::FindCaseSensitively) == QTextDocument::FindCaseSensitively) {
		flags ^= QTextDocument::FindCaseSensitively;
		regex_pattern.setCaseSensitivity(Qt::CaseInsensitive);
	}
}

void lgmx::search::use_regex(bool checked)
{
	regex = checked;
}

void lgmx::search::wrap_around(bool checked)
{
	wrap = checked;
}

void lgmx::search::highlight_all_matches(bool checked)
{
	highlight_all = checked;
}

QComboBox* lgmx::search::create_combo_box(const QString &text)
{
	QComboBox *comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->addItem(text);
	comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	return comboBox;
}

void lgmx::search::add_to_search_history(const QString &text)
{
	int index;

	index = search_cbox->findText(text, static_cast<Qt::MatchFlags>(Qt::MatchExactly | Qt::MatchCaseSensitive));
	if (index != -1) {
		if (index > 0) {
			search_cbox->removeItem(index);
			search_cbox->insertItem(0, text);
			search_cbox->setCurrentIndex(0);
		}
	} else
		search_cbox->insertItem(0, text);
}

/**
 * Creates search dialog UI.
 */

void lgmx::search::setup_ui(QDialog *Find)
{
	if (Find->objectName().isEmpty())
		Find->setObjectName(QString::fromUtf8("Find"));
		
	Find->resize(511, 244);
	horizontalLayout_6 = new QHBoxLayout(Find);
	horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
	verticalLayout_4 = new QVBoxLayout();
	verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
	verticalLayout_3 = new QVBoxLayout();
	verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
	horizontalLayout_4 = new QHBoxLayout();
	horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
	label = new QLabel(Find);
	label->setObjectName(QString::fromUtf8("label"));

	horizontalLayout_4->addWidget(label);

	search_cbox = new QComboBox(Find);
	search_cbox->setObjectName(QString::fromUtf8("search_cbox"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(search_cbox->sizePolicy().hasHeightForWidth());
	search_cbox->setSizePolicy(sizePolicy);
	search_cbox->setEditable(true);
	search_cbox->setMaxCount(MAX_ITEMS);
	search_cbox->setInsertPolicy(QComboBox::InsertAtTop);

	horizontalLayout_4->addWidget(search_cbox);
	verticalLayout_3->addLayout(horizontalLayout_4);

	horizontalLayout_3 = new QHBoxLayout();
	horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
	label_2 = new QLabel(Find);
	label_2->setObjectName(QString::fromUtf8("label_2"));

	horizontalLayout_3->addWidget(label_2);

	comboBox = new QComboBox(Find);
	comboBox->setObjectName(QString::fromUtf8("comboBox"));
	sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
	comboBox->setSizePolicy(sizePolicy);
	comboBox->setEditable(true);
	comboBox->setMaxCount(MAX_ITEMS);
	comboBox->setInsertPolicy(QComboBox::InsertAtTop);


	horizontalLayout_3->addWidget(comboBox);


	verticalLayout_3->addLayout(horizontalLayout_3);

	verticalLayout_4->addLayout(verticalLayout_3);

	horizontalLayout_2 = new QHBoxLayout();
	horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
	verticalLayout_2 = new QVBoxLayout();
	verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
	checkBox = new QCheckBox(Find);
	checkBox->setObjectName(QString::fromUtf8("checkBox"));
	checkBox->setChecked(true);

	verticalLayout_2->addWidget(checkBox);

	checkBox_5 = new QCheckBox(Find);
	checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));

	verticalLayout_2->addWidget(checkBox_5);

	checkBox_2 = new QCheckBox(Find);
	checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

	verticalLayout_2->addWidget(checkBox_2);

	horizontalLayout_2->addLayout(verticalLayout_2);

	verticalLayout = new QVBoxLayout();
	verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
	checkBox_3 = new QCheckBox(Find);
	checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

	verticalLayout->addWidget(checkBox_3);

	checkBox_4 = new QCheckBox(Find);
	checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
	checkBox_4->setChecked(true);

	verticalLayout->addWidget(checkBox_4);

	checkBox_6 = new QCheckBox(Find);
	checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

	verticalLayout->addWidget(checkBox_6);


	horizontalLayout_2->addLayout(verticalLayout);


	verticalLayout_4->addLayout(horizontalLayout_2);

	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	pushButton_4 = new QPushButton(Find);
	pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

	horizontalLayout->addWidget(pushButton_4);

	pushButton_6 = new QPushButton(Find);
	pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

	horizontalLayout->addWidget(pushButton_6);

	pushButton_7 = new QPushButton(Find);
	pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));

	horizontalLayout->addWidget(pushButton_7);

	pushButton_5 = new QPushButton(Find);
	pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

	horizontalLayout->addWidget(pushButton_5);


	verticalLayout_4->addLayout(horizontalLayout);

	horizontalLayout_5 = new QHBoxLayout();
	horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));

	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	horizontalLayout_5->addItem(horizontalSpacer);

	cancel_button = new QPushButton(Find);
	cancel_button->setObjectName(QString::fromUtf8("cancel_button"));

	horizontalLayout_5->addWidget(cancel_button);

	previous_button = new QPushButton(Find);
	previous_button->setObjectName(QString::fromUtf8("previous_button"));

	horizontalLayout_5->addWidget(previous_button);

	next_button = new QPushButton(Find);
	next_button->setObjectName(QString::fromUtf8("next_button"));

	horizontalLayout_5->addWidget(next_button);

	verticalLayout_4->addLayout(horizontalLayout_5);

	horizontalLayout_6->addLayout(verticalLayout_4);

	QWidget::setTabOrder(search_cbox, comboBox);
	QWidget::setTabOrder(comboBox, checkBox);
	QWidget::setTabOrder(checkBox, checkBox_5);
	QWidget::setTabOrder(checkBox_5, checkBox_2);
	QWidget::setTabOrder(checkBox_2, checkBox_3);
	QWidget::setTabOrder(checkBox_3, checkBox_4);
	QWidget::setTabOrder(checkBox_4, checkBox_6);
	QWidget::setTabOrder(checkBox_6, pushButton_4);
	QWidget::setTabOrder(pushButton_4, pushButton_6);
	QWidget::setTabOrder(pushButton_6, pushButton_7);
	QWidget::setTabOrder(pushButton_7, pushButton_5);
	QWidget::setTabOrder(pushButton_5, cancel_button);
	QWidget::setTabOrder(cancel_button, previous_button);
	QWidget::setTabOrder(previous_button, next_button);
	
	retranslate_ui(Find);
	connect_slots();

	//QMetaObject::connectSlotsByName(Find);
} // setupUi

void lgmx::search::retranslate_ui(QDialog *Find)
{
	Find->setWindowTitle(QApplication::translate("Find", "Find", 0, QApplication::UnicodeUTF8));
	label->setText(QApplication::translate("Find", "Search for:", 0, QApplication::UnicodeUTF8));
	label_2->setText(QApplication::translate("Find", "Replace with:", 0, QApplication::UnicodeUTF8));
	checkBox->setText(QApplication::translate("Find", "Use regular expressions", 0, QApplication::UnicodeUTF8));
	checkBox_5->setText(QApplication::translate("Find", "Use escape sequences", 0, QApplication::UnicodeUTF8));
	checkBox_2->setText(QApplication::translate("Find", "Case sensitive", 0, QApplication::UnicodeUTF8));
	checkBox_3->setText(QApplication::translate("Find", "Match whole words only", 0, QApplication::UnicodeUTF8));
	checkBox_4->setText(QApplication::translate("Find", "Highlight all matches", 0, QApplication::UnicodeUTF8));
	checkBox_6->setText(QApplication::translate("Find", "Wrap around", 0, QApplication::UnicodeUTF8));
	pushButton_4->setText(QApplication::translate("Find", "Replace All", 0, QApplication::UnicodeUTF8));
	pushButton_6->setText(QApplication::translate("Find", "Replace", 0, QApplication::UnicodeUTF8));
	pushButton_7->setText(QApplication::translate("Find", "Replace and previous", 0, QApplication::UnicodeUTF8));
	pushButton_5->setText(QApplication::translate("Find", "Replace and next", 0, QApplication::UnicodeUTF8));
	cancel_button->setText(QApplication::translate("Find", "Cancel", 0, QApplication::UnicodeUTF8));
	previous_button->setText(QApplication::translate("Find", "Previous", 0, QApplication::UnicodeUTF8));
	next_button->setText(QApplication::translate("Find", "Next", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void lgmx::search::connect_slots()
{
	connect(cancel_button, SIGNAL(clicked()), this, SLOT(hide_search_dialog()));
	
	connect(next_button, SIGNAL(clicked()), this, SLOT(find_next()));
	connect(previous_button, SIGNAL(clicked()), this, SLOT(find_previous()));
	
	connect(pushButton_5, SIGNAL(clicked()), this, SLOT(replace_and_next()));
	connect(pushButton_7, SIGNAL(clicked()), this, SLOT(replace_and_previous()));
	
	// replace all
	connect(pushButton_4, SIGNAL(clicked()), this, SLOT(replace_all()));
	
	// replace
	connect(pushButton_6, SIGNAL(clicked()), this, SLOT(replace()));	
	
	connect(checkBox_3, SIGNAL(toggled(bool)), this, SLOT(match_whole_words(bool)));
	connect(checkBox_2, SIGNAL(toggled(bool)), this, SLOT(case_sensitive(bool)));
	connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(use_regex(bool)));
	connect(checkBox_6, SIGNAL(toggled(bool)), this, SLOT(wrap_around(bool)));
	connect(checkBox_4, SIGNAL(toggled(bool)), this, SLOT(highlight_all_matches(bool)));
}



