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

#define NUMBER		2
#define KEYWORD		3


C_highlighter::C_highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
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


void C_highlighter::highlightBlock(const QString &text)
{
	int ret;
	int size;
	QList<hl_info> hl_info_list;
	QList<hl_info>::iterator it;

	lex(text, hl_info_list);
	
	for (it = hl_info_list.begin(); it != hl_info_list.end(); it++) {
		//cout << "begin " << it->begin << " offset "<< it->offset << endl;
		
		switch (it->token) {
		case 0:
			setFormat(it->begin, it->offset, integerFormat);
			break;
			
		case 1:
			setFormat(it->begin, it->offset, keywordFormat);
			break;
		
		case NUMBER:
			setFormat(it->begin, it->offset, integerFormat);
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
			setFormat(cpp_start, yyleng, integerFormat);
			break;
			
		case 279:
			setFormat(cpp_start, yyleng, keywordFormat);
			break;
			
		case 280:
			setFormat(cpp_start, yyleng, singleLineCommentFormat);
			break;

		case 281:
			setFormat(cpp_start, yyleng, singleLineCommentFormat);
			
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
	
	while (pos < size) {
		match = true;
		
		switch (data[pos].unicode()) {
		
		case '/':	// match comments
		{
			if (pos + 1 < size) {
				if (data[pos + 1] == '/') {		// single line comment
					info.begin = pos;
					info.offset = data.length() - info.begin;
					info.token = 0;
					pos = size;
					hl_info_list.append(info);
					break;
					
				} else if (data[pos+1] == '*') {
					info.begin = pos;

					while (data[pos] != '*' || data[pos + 1] != '/') {
						pos++;
						if (pos + 1 == size)
							break;
					}
					
					//end = pos++;
					
					//cout << "comment: " << begin << " " << end << endl;
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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


