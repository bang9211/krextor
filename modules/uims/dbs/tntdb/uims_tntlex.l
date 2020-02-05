%{
#include "local/uims_tntlex_l.h"
extern int yyerror(char *s, yyscan_t yyscanner);
%}

%option yylineno

HEX		[0-9a-f]
DIGIT	[0-9]
ID		[a-zA-Z_][a-zA-Z0-9_:.]*

%%

[']([^']|'')*[']				{ return UIMS_TNTTK_STRING; }
[0][xX][']{HEX}*[']				{ return UIMS_TNTTK_OCTSTR; }
[0][xX]{HEX}*					{ return UIMS_TNTTK_NUMBER; }	
{DIGIT}+[.]{DIGIT}+				{ return UIMS_TNTTK_FLOAT; }
[\-]{DIGIT}+[.]{DIGIT}+			{ return UIMS_TNTTK_FLOAT; }
{DIGIT}+						{ return UIMS_TNTTK_NUMBER; }
[\-]{DIGIT}+					{ return UIMS_TNTTK_NUMBER; }

"="								{ return UIMS_TNTTK_EQ; }
">"								{ return UIMS_TNTTK_GT; }
"<"								{ return UIMS_TNTTK_LT; }
">="							{ return UIMS_TNTTK_GE; }
"<="							{ return UIMS_TNTTK_LE; }
"!="							{ return UIMS_TNTTK_NE; }

"{"								{ return yytext[0]; }
"}"								{ return yytext[0]; }
";"								{ return yytext[0]; }
"["								{ return yytext[0]; }
"]"								{ return yytext[0]; }
"?"								{ return yytext[0]; }
"+"								{ return yytext[0]; }
"-"								{ return yytext[0]; }
"|"								{ return yytext[0]; }
"&"								{ return yytext[0]; }
"^"								{ return yytext[0]; }
[(),*@!#/]						{ return yytext[0]; }

[Ss][Ee][Ll][Ee][Cc][Tt]		{ return UIMS_TNTTK_SELECT; }
[Ii][Nn][Ss][Ee][Rr][Tt]		{ return UIMS_TNTTK_INSERT; }
[Rr][Ee][Pp][Ll][Aa][Cc][Ee]	{ return UIMS_TNTTK_REPLACE; }
[Uu][Pp][Dd][Aa][Tt][Ee]		{ return UIMS_TNTTK_UPDATE; }
[Dd][Ee][Ll][Ee][Tt][Ee]		{ return UIMS_TNTTK_DELETE; }
[Uu][Pp][Ss][Ee][Rr][Tt]		{ return UIMS_TNTTK_UPSERT; }
[Cc][Aa][Ll][Ll]				{ return UIMS_TNTTK_CALL; }
[Ee][Vv][Aa][Ll]				{ return UIMS_TNTTK_EVAL; }
[Pp][Ii][Nn][Gg]				{ return UIMS_TNTTK_PING; }
[Ff][Rr][Oo][Mm]				{ return UIMS_TNTTK_FROM; }
[Ww][Hh][Ee][Rr][Ee]			{ return UIMS_TNTTK_WHERE; }
[Ii][Nn][Tt][Oo]				{ return UIMS_TNTTK_INTO; }
[Vv][Aa][Ll][Uu][Ee][Ss]		{ return UIMS_TNTTK_VALUES; }
[Ss][Ee][Tt]					{ return UIMS_TNTTK_SET; }
[Aa][Nn][Dd]					{ return UIMS_TNTTK_AND; }
[Oo][Rr]						{ return UIMS_TNTTK_OR; }
[Ll][Ii][Mm][Ii][Tt]			{ return UIMS_TNTTK_LIMIT; }
{ID}							{ return UIMS_TNTTK_ID; }

\n								;
[ \t]+							;
.								{ return yyerror("Invalid character", yyscanner);}
%%

int yyerror(char *s, yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
	uims_tntlex_t *lexer = (uims_tntlex_t*)yyg->yyextra_r;
	if( lexer == NULL || lexer->errorf == NULL ) { 
		fprintf(stderr,"%s near %s (line=%d, col=%d)\n",
				s, yyget_text(yyscanner),  yyget_lineno(yyscanner), yyget_column(yyscanner));
	} else {
		lexer->errorf( lexer, s, yyget_text(yyscanner), yyget_lineno(yyscanner), yyget_column(yyscanner));
	}
	return -1;
}

int yywrap( yyscan_t yyscanner)
{
	return 1;
}

int uims_tntlex_init( uims_tntlex_t *lexer)
{
	memset( lexer, 0, sizeof(uims_tntlex_t));
	return yylex_init_extra( lexer, &lexer->scanner);
}

void uims_tntlex_final( uims_tntlex_t *lexer)
{
	if( lexer->scanner) yylex_destroy( lexer->scanner);
	memset( lexer, 0, sizeof(uims_tntlex_t));
}

void* uims_tntlex_set_scan_str( uims_tntlex_t *lexer, char *str)
{
	return yy_scan_string( str, lexer->scanner);
}

void uims_tntlex_clear_scan_str( uims_tntlex_t *lexer, void *state)
{
	return yy_delete_buffer( state, lexer->scanner);
}

FILE* uims_tntlex_get_in( uims_tntlex_t *lexer)
{
	return yyget_in( lexer->scanner);
}

void uims_tntlex_set_in( uims_tntlex_t *lexer, FILE *fp)
{
	yyset_in( fp, lexer->scanner);
}

FILE* uims_tntlex_get_out( uims_tntlex_t *lexer)
{
	return yyget_out( lexer->scanner);
}

void uims_tntlex_set_out( uims_tntlex_t *lexer, FILE *fp)
{
	yyset_out( fp, lexer->scanner);
}

int uims_tntlex_scan( uims_tntlex_t *lexer)
{
	return yylex( lexer->scanner);
}

int uims_tntlex_get_leng( uims_tntlex_t *lexer)
{
	return yyget_leng( lexer->scanner);
}

char* uims_tntlex_get_text( uims_tntlex_t *lexer)
{
	return yyget_text( lexer->scanner);
}

int uims_tntlex_get_lineno( uims_tntlex_t *lexer)
{
	return yyget_lineno( lexer->scanner);
}

int uims_tntlex_get_column( uims_tntlex_t *lexer)
{
	return yyget_lineno( lexer->scanner);
}
