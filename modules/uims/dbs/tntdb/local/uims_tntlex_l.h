#ifndef __UIMS_TNTLEX_L_H__
#define __UIMS_TNTLEX_L_H__

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// UIMS TARANTOOL SQL lexer internal types & definitions
////////////////////////////////////////////////////////////////////////////////

#define UIMS_TNTTK_ID      257
//#define UIMS_TNTTK_DIGIT   258
//#define UIMS_TNTTK_HEX     259
//#define UIMS_TNTTK_HEXD2   260
#define UIMS_TNTTK_STRING  261
#define UIMS_TNTTK_OCTSTR  262
#define UIMS_TNTTK_NUMBER  263
#define UIMS_TNTTK_FLOAT   264
#define UIMS_TNTTK_EQ      267
#define UIMS_TNTTK_GT      268
#define UIMS_TNTTK_LT      269
#define UIMS_TNTTK_GE      270
#define UIMS_TNTTK_LE      271
#define UIMS_TNTTK_NE      272

#define UIMS_TNTTK_SELECT  280
#define UIMS_TNTTK_INSERT  281
#define UIMS_TNTTK_REPLACE 282
#define UIMS_TNTTK_UPDATE  283
#define UIMS_TNTTK_DELETE  284
#define UIMS_TNTTK_UPSERT  285
#define UIMS_TNTTK_CALL    286
#define UIMS_TNTTK_EVAL    287
#define UIMS_TNTTK_PING    288
#define UIMS_TNTTK_FROM    289
#define UIMS_TNTTK_WHERE   290
#define UIMS_TNTTK_INTO    291
#define UIMS_TNTTK_VALUES  292
#define UIMS_TNTTK_SET     293
#define UIMS_TNTTK_AND     294
#define UIMS_TNTTK_OR      295
#define UIMS_TNTTK_LIMIT   296
#define UIMS_TNTTK_SPLICE  297

/** @brief SQL LEXER 타입 */
typedef struct uims_tntlex_s uims_tntlex_t; 
/** @brief SQL LEXER 구조체 */
struct uims_tntlex_s {
	void *scanner; /**< SQL scanner instance */
	/**< error callback function */
	void (*errorf)( uims_tntlex_t* lexer, const char *err, const char *near, int line, int col);
};

////////////////////////////////////////////////////////////////////////////////
// local functions for uims_tntlex_t
////////////////////////////////////////////////////////////////////////////////
int uims_tntlex_init( uims_tntlex_t *lexer);
void uims_tntlex_final( uims_tntlex_t *lexer);
void* uims_tntlex_set_scan_str( uims_tntlex_t *lexer, char *str);
void uims_tntlex_clear_scan_str( uims_tntlex_t *lexer, void *state);
FILE* uims_tntlex_get_in( uims_tntlex_t *lexer);
void uims_tntlex_set_in( uims_tntlex_t *lexer, FILE *fp);
FILE* uims_tntlex_get_out( uims_tntlex_t *lexer);
void uims_tntlex_set_out( uims_tntlex_t *lexer, FILE *fp);
int uims_tntlex_scan( uims_tntlex_t *lexer);
int uims_tntlex_get_leng( uims_tntlex_t *lexer);
char* uims_tntlex_get_text( uims_tntlex_t *lexer);
int uims_tntlex_get_lineno( uims_tntlex_t *lexer);
int uims_tntlex_get_column( uims_tntlex_t *lexer);

#endif
