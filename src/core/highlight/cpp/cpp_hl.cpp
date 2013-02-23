#include "cpp_hl.h"
#include <QString>
#include <QList>
#include <iostream>

using namespace std;

extern void init(const char *input, int size);
extern void destroy();
extern int parse();
extern int yyleng;
extern char *yytext;
extern string buff;
extern int pos;
extern int cpp_start;
extern bool comment;

#define COMMENT		0
#define NUMBER		2
#define KEYWORD		3

#define IN_COMMENT	(1 << 0)

C_highlighter::C_highlighter(src_file *parent) : syntax_highlighter(parent)
{
	integerFormat.setForeground(Qt::red);
    integerFormat.setBackground(Qt::transparent);
	integerFormat.setFontWeight(QFont::Bold);
	
	keywordFormat.setForeground(Qt::green);
    keywordFormat.setBackground(Qt::transparent);
	keywordFormat.setFontWeight(QFont::Bold);
	
	singleLineCommentFormat.setForeground(Qt::gray);
    singleLineCommentFormat.setBackground(Qt::transparent);
    
    //keywords_.insert("if");
    //keywords_.insert("else");
    //keywords_.insert("while");
    //keywords_.insert("for");
}

C_highlighter::~C_highlighter()
{
}


void C_highlighter::highlight_block(const QString &text)
{
	int ret;
	int size;
	QList<hl_info> hl_info_list;
	QList<hl_info>::iterator it;

	lex(text, hl_info_list);
	
	for (it = hl_info_list.begin(); it != hl_info_list.end(); it++) {
		//cout << "begin " << it->begin << " offset "<< it->offset << endl;
		
		switch (it->token) {
		case COMMENT:
			cout << "comment: " << it->begin << " " << it->offset - 1 << endl;
			set_format(it->begin, it->offset, integerFormat);
			break;
			
		case 1:
			set_format(it->begin, it->offset, keywordFormat);
			break;
		
		case NUMBER:
			set_format(it->begin, it->offset, integerFormat);
			break;
		
		default:
			break;
		}
	}

	return;

	init ((char*)(text.toStdString().c_str()), 0);
	
	while (ret = parse()) {
			
		switch (ret) {
		case 282:
		case 278:
			//printf("Integer: len: %d start: %d - %s\n", yyleng, start, buff.c_str());
			set_format(cpp_start, yyleng, integerFormat);
			break;
			
		case 279:
			set_format(cpp_start, yyleng, keywordFormat);
			break;
			
		case 280:
			set_format(cpp_start, yyleng, singleLineCommentFormat);
			break;

		case 281:
			set_format(cpp_start, yyleng, singleLineCommentFormat);
			
			//if (comment) {
			//	this->highlightBlock(currentBlock().next().text());
			//}
			break;
		}
	}
	destroy();
	/*
	if (comment) {
		QString str(currentBlock().next().text());
		
		cout << "next block: " << str.toStdString() << endl;
		//this->highlightBlock(currentBlock().next().text());
		
		///if (currentBlock().next().isValid())
			//rehighlightBlock(currentBlock().next());
	}*/
}

void C_highlighter::lex(const QString &data, QList<hl_info> &hl_info_list)
{
	int pos = 0;
	int size = data.size();
	struct hl_info info;
	bool match;
	
	cout << "size = " << size << endl;
	
	int prev_state = get_previous_block_state();
	
	if (prev_state == IN_COMMENT) {
		info.begin = pos;

		int index = data.indexOf("*/", pos);
		
		cout << "idx: " << index << " from: " << pos << endl;
		
		if (index < 0) {
			cout << "ends in comment" << endl;
			info.offset = size - info.begin;
			set_current_block_state(IN_COMMENT);
			pos = size;
		} else {
			info.offset = index - info.begin + 2;
			pos += info.offset;
		}
		
		info.token = COMMENT;
		hl_info_list.append(info);
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
					} else {
						info.offset = index - info.begin + 2;
					}
					
					info.token = COMMENT;
					hl_info_list.append(info);
				}
			}
		}
		break;

		case '"':		// literal
			info.begin = pos++;
			
			while (pos < size) {
				if (data[pos] == '"' && data[pos - 1] != '\\') {
					info.offset = pos - info.begin + 1;
					info.token = 1;
					hl_info_list.append(info);
					break;
				} else {
					pos++;
				}
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
			pos--;
			info.token = NUMBER;
			hl_info_list.append(info);
		} else if (data[pos].isLower()) {
			info.begin = pos;
			
			do {
				pos++;
			} while (pos < size && data[pos].isLower());
			
			//if (pos < size && !data[pos].isLetter() && data[pos] != '_')
			
			info.offset = pos - info.begin;
			pos--;
			info.token = KEYWORD;
			hl_info_list.append(info);
			
		}
	}
	
	pos++;
	}
	
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


