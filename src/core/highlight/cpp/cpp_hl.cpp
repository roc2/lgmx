#include "cpp_hl.h"
#include <QString>
#include <QList>
#include <stdexcept>
#include <debug.h>
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


cpp_highlighter::cpp_highlighter(src_file *parent, QSharedPointer<QSet<QString> > keywords, 
                             QSharedPointer<std::vector<QTextCharFormat> > formats) : syntax_highlighter(parent), 
                             keywords_(keywords), formats_(formats)
{
    cout << "cpp_highlighter constructor ok" << endl;
}

cpp_highlighter::~cpp_highlighter()
{
	cout << "~cpp_highlighter" << endl;
}


void cpp_highlighter::highlight_block(const QString &text)
{
	QList<hl_info> hl_info_list;
	QList<hl_info>::iterator it;

	formatChanges.fill(QTextCharFormat(), curr_block_.length() - 1);

	lex(text, hl_info_list);
	
	for (it = hl_info_list.begin(); it != hl_info_list.end(); it++) {
		try {
			switch (it->token) {
			case COMMENT:
				//cout << "comment: " << it->begin << " " << it->offset - 1 << endl;
				set_format(it->begin, it->offset, (*formats_).at(COMMENT_IDX));
				break;
				
			case KEYWORD:
				//cout << "keyword: " << it->begin << " " << it->offset - 1 << endl;
				set_format(it->begin, it->offset, (*formats_).at(KEYWORD_IDX));
				break;
			
			case NUMBER:
				//cout << "number: " << it->begin << " " << it->offset - 1 << endl;
				set_format(it->begin, it->offset, (*formats_).at(NUMBER_IDX));
				break;
				
			case LITERAL:
				//cout << "literal: " << it->begin << " " << it->offset - 1 << endl;
				set_format(it->begin, it->offset, (*formats_).at(LITERAL_IDX));
				break;
			
			case PRE_PROC:
				//cout << "pre proc: " << it->begin << " " << it->offset - 1 << endl;
				set_format(it->begin, it->offset, (*formats_).at(PRE_PROC_IDX));
				break;

			default:
				break;
			}
		} catch (const std::out_of_range &err) {
			debug(ERR, HIGHLIGHT, err.what() << " - " << "Invalid formats vector index");
		}
	}
}

void cpp_highlighter::lex(const QString &data, QList<hl_info> &hl_info_list)
{
	int pos = 0;
	int size = data.size();
	struct hl_info info;
	bool match;
	
	//cout << "\nsize = " << size << endl;
	
	int prev_state = get_previous_block_state();
	//cout << "prev state = " << prev_state << endl;
	
	if (prev_state == IN_COMMENT) {
		info.begin = pos;

		//cout << "first in comment!!" << endl;
		int index = data.indexOf("*/", pos);
		
		//cout << "idx: " << index << " from: " << pos << endl;
		
		if (index < 0) {
			//cout << "ends in comment" << endl;
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
					
					//cout << "idx: " << index << " from: " << pos + 2 << endl;
					
					if (index < 0) {
						//cout << "ends in comment" << endl;
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
		case '\'':
		{
			QChar match_ch(data[pos].unicode() == '"' ? '"' : '\'');
			info.begin = pos++;
			
			while (pos < size) {
				if (data[pos] == match_ch && data[pos - 1] != '\\') {
					info.offset = pos - info.begin + 1;
					info.token = LITERAL;
					hl_info_list.append(info);
					break;
				} else {
					pos++;
				}
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
		} else if (data[pos].isLetter() || (data[pos] == '_')) {
			info.begin = pos;
			
			do {
				pos++;
			} while (pos < size && (data[pos].isLetter() || data[pos].isDigit() || (data[pos] == '_')));
			
			QString word(data.mid(info.begin, pos - info.begin));
			//cout << "word: " << word.toStdString() << endl;

			if ((*keywords_).contains(word)) {
				//cout << "found word!!!!" << endl;
				info.offset = pos - info.begin;
				info.token = KEYWORD;
				hl_info_list.append(info);
			} else {
				// not a keyword, it is an identifier
			}
		}
	}
	
	pos++;
	}
	
	set_current_block_state(HIGHLIGHTED);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


