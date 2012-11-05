%{
#include <iostream>
#include <string>

using namespace std;
#define YY_DECL extern "C" int yylex()

string buff;

extern char *input;
int pos = 0;
int cpp_start = 0;
bool comment = false;

%}

string  		\"[^\n"]+\"
dig			[0-9]
alpha   [A-Za-z]

EXP 	([Ee][-+]?[0-9]+)
ILEN	([Uu](L|l|LL|ll)?|(L|l|LL|ll)[Uu]?)
UCN 	(\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})

%x IN_COMMENT

%%

0[0-7]*{ILEN}?				{cpp_start = pos; pos += yyleng; return 278;}
[1-9][0-9]*{ILEN}?			{cpp_start = pos; pos += yyleng; return 278;}
0[Xx][0-9a-fA-F]+{ILEN}?	{cpp_start = pos; pos += yyleng; return 278;}

([0-9]*\.[0-9]+|[0-9]+\.){EXP}?[flFL]?		{cpp_start = pos; pos += yyleng; return 278;}
[0-9]+{EXP}[flFL]?							{cpp_start = pos; pos += yyleng; return 278;}

int|void|return|if|else|for|while|static|struct|enum   {cpp_start = pos; pos += yyleng; return 279;}

"#"" "*(if(n?def)?|els(e|if)|endif|define|undef|line|pragma)			 {cpp_start = pos; pos += yyleng; return 282;}

0[Xx]([0-9a-fA-F]*\.[0-9a-fA-F]+|[0-9a-fA-F]+\.?)[Pp][-+]?[0-9]+[flFL]?		{cpp_start = pos; pos += yyleng; return 278;}

([_a-zA-Z]|{UCN})([_a-zA-Z0-9]|{UCN})*		{cpp_start = pos; pos += yyleng; return 1;}

[ \t\n]+        {pos += yyleng; return 1;}
{string}		{cpp_start = pos; pos += yyleng; return 280;}
"#"include		{cpp_start = pos; pos += yyleng; return 278;}



"//".*	{cpp_start = pos; pos += yyleng; return 281;}

<INITIAL>{
"/*"   {cpp_start = pos; pos += yyleng; BEGIN(IN_COMMENT); comment = true; return 281;}
}

<IN_COMMENT>{
"*/"      {cpp_start = pos; pos += yyleng; BEGIN(INITIAL); comment = false; return 281; }
([^*]|\n)+|. {cpp_start = pos; pos += yyleng; return 281;}
}


.				{pos++; /*cout << "pos++" << endl;*/ return 1;}


%%


void init(const char *input, int size)
{
	pos = cpp_start = 0;
	
	yy_scan_string (input);
	//yy_scan_buffer(input, 10);
}

void destroy()
{
	yy_delete_buffer(YY_CURRENT_BUFFER);
}

int parse()
{
	/*Analyze the string*/
	return yylex();
	
	//cout << "ret = " << ret << endl;

	/*Delete the new buffer*/
	//yy_delete_buffer(YY_CURRENT_BUFFER);
}

//\".*\"			{ cout << "Found a real string: " << yytext << endl; }
// [0-9]+\.[0-9]+  { cout << "Found a floating-point number:" << yytext << endl; buff = yytext; return yyleng;}
// [a-zA-Z0-9]+    { cout << "Found a string: " << yytext << endl; }
//[^*\n]+   {cpp_start = pos; pos += yyleng; return 281;}
//"*"       {cpp_start = pos++; return 281;}
//[\n]      {pos++;}

