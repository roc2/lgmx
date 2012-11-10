#include "cpp_hl.h"
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
}

C_highlighter::~C_highlighter()
{
}


void C_highlighter::highlightBlock(const QString &text)
{
	int ret;
	int size;

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


