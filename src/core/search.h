#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QRegExp>
#include "src_container.h"
#include <QTextCharFormat>
#include <QBitArray>
#include <QSettings>
#include <QPointer>

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QTextDocument>
#include <QTextCursor>

class view_manager;

// persistent flags
#define REGEX			(1 << 0)
#define CASE_SENSITIVE	(1 << 1)
#define WHOLE_WORDS		(1 << 2)
#define HIGHLIGHT_ALL	(1 << 3)
#define WRAP_AROUND		(1 << 4)

#define MAX_ITEMS		10

namespace lgmx {

class search : public QDialog
{
	Q_OBJECT

private:
	search(const search&);
	search& operator=(const search&);

public:
	search(view_manager &manager, QWidget *parent = 0);
	~search();
	
	void save_settings();
	void load_settings();
	void set_default_settings();
	
	void search_string(QString &pattern);

private slots:
	void hide_search_dialog();
	void update_current_file(int);
	void show_search_dialog();
	void find_next();
	void find_previous();
	void match_whole_words(bool checked);
	void case_sensitive(bool checked);
	void use_regex(bool checked);
	void wrap_around(bool checked);
	void highlight_all_matches(bool checked);
	void replace_and_next();
	void replace_and_previous();
	void replace();
	void replace_all();

private:
	QComboBox *create_combo_box(const QString &text);

	void setup_ui(QDialog *Find);
	void destroy_ui();
	void retranslate_ui(QDialog *Find);
	void connect_slots();

	void highlight_all_matches(QString &pattern);
	
	void add_to_search_history(const QString &text);

private:	
	QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QComboBox *search_cbox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_2;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_6;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_5;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_button;
    QPushButton *previous_button;
    QPushButton *next_button;


	QTextDocument::FindFlags flags;
	QTextCursor cursor;
	QPointer<visual_src_file> curr_file_;
	src_container *src_ctr;
	view_manager &manager_;
	
	bool regex;
	bool wrap;
	bool highlight_all;
	
	QRegExp regex_pattern;
	QTextCharFormat match_format;
	QComboBox *text_combo_box;
	QString search_pattern;
	QString replace_pattern;
};

} //namespace lgmx

#endif
