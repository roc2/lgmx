#include "cpp_hl.h"
#include <QString>
#include <QList>
#include <iostream>

using namespace std;

extern QVector<QTextCharFormat> formatChanges;

#define COMMENT		0
#define NUMBER		2
#define KEYWORD		3
#define LITERAL		4
#define PRE_PROC	5

#define UNHIGHLIGHTED	-1
#define HIGHLIGHTED		0
#define IN_COMMENT		1
#define IN_LITERAL		2


QString words[] = {
	"if", "else", "while"
};

std::set<QString> key_(words, words + 3);

QSet<QString> keywords_;

dummy_highlighter::dummy_highlighter(src_file *parent) : syntax_highlighter(parent)
{
	keywordFormat = new QTextCharFormat();
	cout << "dummy constructor ok" << endl;
}

dummy_highlighter::~dummy_highlighter()
{
	delete keywordFormat;
}

void dummy_highlighter::highlight_block(const QString &text)
{
}

C_highlighter::C_highlighter(src_file *parent) : syntax_highlighter(parent)
{
	integerFormat = new QTextCharFormat();
	integerFormat->setForeground(Qt::blue);
    integerFormat->setBackground(Qt::transparent);
	integerFormat->setFontWeight(QFont::Bold);
	
	keywordFormat = new QTextCharFormat();
	keywordFormat->setForeground(Qt::darkCyan);
    keywordFormat->setBackground(Qt::transparent);
	keywordFormat->setFontWeight(QFont::Bold);
	
	CommentFormat = new QTextCharFormat();
	CommentFormat->setForeground(Qt::red);
    CommentFormat->setBackground(Qt::transparent);
    
    literalFormat = new QTextCharFormat();
    literalFormat->setForeground(Qt::magenta);
    literalFormat->setBackground(Qt::transparent);
    
    pre_processor = new QTextCharFormat();
    pre_processor->setForeground(Qt::darkGreen);
    pre_processor->setBackground(Qt::transparent);
    
    
    //keywords_ = new QSet<QString>();
    keywords_.insert("if");
    keywords_.insert("else");
    keywords_.insert("while");
    keywords_.insert("for");
    keywords_.insert("goto");
    keywords_.insert("return");
    keywords_.insert("const");
    keywords_.insert("unsigned");
    keywords_.insert("int");
    keywords_.insert("struct");
    keywords_.insert("static");
    keywords_.insert("long");
    keywords_.insert("void");
    keywords_.insert("break");
    keywords_.insert("char");
    cout << "C_highlighter constructor ok" << endl;
}

C_highlighter::~C_highlighter()
{
	delete keywordFormat;
	delete CommentFormat;
	delete integerFormat;
	delete literalFormat;
	delete pre_processor;
	
	//delete keywords_;
	cout << "~C_highlighter" << endl;
}


void C_highlighter::highlight_block(const QString &text)
{
	QList<hl_info> hl_info_list;
	QList<hl_info>::iterator it;

	formatChanges.fill(QTextCharFormat(), curr_block_.length() - 1);

	lex(text, hl_info_list);
	
	for (it = hl_info_list.begin(); it != hl_info_list.end(); it++) {
		
		switch (it->token) {
		case COMMENT:
			cout << "comment: " << it->begin << " " << it->offset - 1 << endl;
			set_format(it->begin, it->offset, *CommentFormat);
			break;
			
		case KEYWORD:
			cout << "keyword: " << it->begin << " " << it->offset - 1 << endl;
			set_format(it->begin, it->offset, *keywordFormat);
			break;
		
		case NUMBER:
			cout << "number: " << it->begin << " " << it->offset - 1 << endl;
			set_format(it->begin, it->offset, *integerFormat);
			break;
			
		case LITERAL:
			cout << "literal: " << it->begin << " " << it->offset - 1 << endl;
			set_format(it->begin, it->offset, *literalFormat);
			break;
		
		case PRE_PROC:
			cout << "pre proc: " << it->begin << " " << it->offset - 1 << endl;
			set_format(it->begin, it->offset, *pre_processor);
			break;

		default:
			break;
		}
	}
}

void C_highlighter::lex(const QString &data, QList<hl_info> &hl_info_list)
{
	int pos = 0;
	int size = data.size();
	struct hl_info info;
	bool match;
	
	cout << "\nsize = " << size << endl;
	
	int prev_state = get_previous_block_state();
	cout << "prev state = " << prev_state << endl;
	
	if (prev_state == IN_COMMENT) {
		info.begin = pos;

		cout << "first in comment!!" << endl;
		int index = data.indexOf("*/", pos);
		
		cout << "idx: " << index << " from: " << pos << endl;
		
		if (index < 0) {
			cout << "ends in comment" << endl;
			info.offset = size - info.begin;
			set_current_block_state(IN_COMMENT);
			info.token = COMMENT;
			hl_info_list.append(info);
			return;
		} else {
			info.offset = index - info.begin + 2;
			pos += info.offset;
			info.token = COMMENT;
			hl_info_list.append(info);
		}
	}

	while (pos < size) {
		match = true;
		
		switch (data[pos].unicode()) {
		
		case '/':	// match comments
		{
			if (pos + 1 < size) {
				if (data[pos + 1] == '/') {		// single line comment
					info.begin = pos;
					info.offset = data.length() - info.begin;
					info.token = COMMENT;
					pos = size;
					hl_info_list.append(info);
					break;
					
				} else if (data[pos+1] == '*') {
					info.begin = pos;

					int index = data.indexOf("*/", pos + 2);
					
					cout << "idx: " << index << " from: " << pos + 2 << endl;
					
					if (index < 0) {
						cout << "ends in comment" << endl;
						info.offset = size - info.begin;
						set_current_block_state(IN_COMMENT);
						info.token = COMMENT;
						hl_info_list.append(info);
						return;
					} else {
						info.offset = index - info.begin + 2;
						info.token = COMMENT;
						hl_info_list.append(info);
						pos = index + 1;
					}
				}
			}
		}
		break;

		case '"':		// literal
			info.begin = pos++;
			
			while (pos < size) {
				if (data[pos] == '"' && data[pos - 1] != '\\') {
					info.offset = pos - info.begin + 1;
					info.token = LITERAL;
					hl_info_list.append(info);
					break;
				} else {
					pos++;
				}
			}
		
			break;
		
		case '#':		// pre processor
			if (pos == 0 || data[pos - 1] == ' ' || (data[pos - 1] == '\t')) {
				info.begin = pos;
				
				// skip spaces
				do {
					pos++;
				} while (pos < size && (data[pos] == ' ' || (data[pos] == '\t')));
				
				while (pos < size && data[pos].isLetter())
					pos++;

				info.offset = pos - info.begin + 1;
				info.token = PRE_PROC;
				hl_info_list.append(info);
			}

			break;

		default:
			match = false;
			break;
	}
	
	if (!match) {
		// number
		if (data[pos].isDigit()) {
			info.begin = pos;
			
			do {
				pos++;
			} while (pos < size && (data[pos].isDigit() || data[pos] == '.'|| data[pos].isLetter()));
			
			info.offset = pos - info.begin;
			//pos--;
			info.token = NUMBER;
			hl_info_list.append(info);
		} else if (data[pos].isLower()) {
			info.begin = pos;
			
			do {
				pos++;
			} while (pos < size && data[pos].isLower());
			
			QString word(data.mid(info.begin, pos - info.begin));
			cout << "word: " << word.toStdString() << endl;

			if (keywords_.find(word) != keywords_.end()) {
				cout << "found word!!!!" << endl;
				info.offset = pos - info.begin;
				info.token = KEYWORD;
				hl_info_list.append(info);
			}
		} else {
			cout << "no match at all" << endl;
		}
	}
	
	pos++;
	}
	
	set_current_block_state(HIGHLIGHTED);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


