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

//////////////////////////////////////////////////////////////////////////////////////
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
//////////////////////////////////////////////////////////////////////////////////////

class view_manager;

#define NUM_FLAGS	5

// persistent flags masks
#define REGEX			0X01
#define CASE_SENSITIVE	0X02
#define WHOLE_WORDS		0X04
#define HIGHLIGHT_ALL	0X08
#define WRAP_AROUND		0X10

#define MAX_ITEMS		10

namespace lgmx {

class search : public QDialog
{
	Q_OBJECT

public:
	search(src_container *src_files, QWidget *parent = 0);
	search(view_manager &manager, QWidget *parent = 0);
	~search();
	
	void save_settings();
	void load_settings();
	void set_default_settings();
	
	void search_string(QString &pattern);

private slots:
	//void browse();
	//void find();
	//void openFileOfItem(int row, int column);
	void hide_search_dialog();

private slots:
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
	
	//void replace(QString &pattern);
	//void replace_text(QString &search_pattern, QString &replace_pattern);
	void replace();

private:
	
	//enum flag_val {regex, wrap_v};
	
	QComboBox *create_combo_box(const QString &text);

	private:
	//QStringList findFiles(const QStringList &files, const QString &text);
	//void showFiles(const QStringList &files);
	//QPushButton *createButton(const QString &text, const char *member);
	//QComboBox *createComboBox(const QString &text = QString());
//	void createFilesTable();

	void setup_ui(QDialog *Find);
	void retranslate_ui(QDialog *Find);
	void connect_slots();

	void highlight_all_matches(QString &pattern);
	
	void add_to_search_history(const QString &text);
	
	QPointer<src_file> curr_file_;

//////////////////////////////////////////////////////////////////////////////////////
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
    QPushButton *pushButton_8;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_button;
    QPushButton *previous_button;
    QPushButton *next_button;

	QTextDocument::FindFlags flags;
	QTextCursor cursor;
//////////////////////////////////////////////////////////////////////////////////////

	src_container *src_ctr;
	view_manager &manager_;
	
	//QBitArray p_flags;	// persistent flags
	//uint p_flags;
	
	bool regex;
	//bool case_sensitive;
	bool wrap;
	bool highlight_all;
	
	QRegExp regex_pattern;

	QTextCharFormat match_format;

	QComboBox *text_combo_box;

	QString search_pattern;
	QString replace_pattern;

	//QPushButton *next_button;
	//QPushButton *previous_button;
	//QPushButton *close_button;
};

}

#endif
