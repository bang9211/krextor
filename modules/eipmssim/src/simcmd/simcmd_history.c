#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "simcmd_errno.h"
#include "simcmd_history.h"


/////////////////////////////////////////////////////////////////////////////////////////
// public functions 
/////////////////////////////////////////////////////////////////////////////////////////

/** 
 * @breif �Է��� ���ɾ �ϼ���Ų��.
 * @param line readline string
 * @return result code : 0 ����, -1 : ���� Ȥ�� just print 
 */
int simcmd_history_expand( char **line )
{
	int rv;
	char *hisexp = NULL;

	/* Expand the string STRING, placing the result into OUTPUT, a pointer
	   to a string.  Returns:

	   0) If no expansions took place (or, if the only change in
		  the text was the de-slashifying of the history expansion
		  character)
	   1) If expansions did take place
	  -1) If there was an error in expansion.
	   2) If the returned line should just be printed.

	  If an error ocurred in expansion, then OUTPUT contains a descriptive
	  error message. */
	rv = history_expand(*line, &hisexp);
	if( rv == 1 || rv == 2 ) {
		free(*line);
		*line = hisexp;
		printf("%s\n", *line);
	} else {
		if( hisexp ) free(hisexp);
	}

	if( rv == 0 || rv == 1 ) return eSIMCMD_SUCCESS;
	else return eSIMCMD_ERROR;
}

/**
 * @breif ���ɾ history ����Ʈ�� �߰��ϴ� �Լ�
 * @param line readline string
 */
void simcmd_history_add( char *line )
{
	int i;
	time_t t = time(0);
	struct tm ltime;
	HIST_ENTRY **hlist;
	HIST_ENTRY *hist;

	/* Return a NULL terminated array of HIST_ENTRY which is the current input
	   history.  Element 0 of this list is the beginning of time.  If there
	   is no history, return NULL. */
	hlist = history_list();

	for( i = 0 ; i < history_length ; i ++ ) {
		if( !strcmp(line, hlist[i]->line) ) break;
	}
	if( i < history_length ) {
		/* A reasonably useless function, only here for completeness.  WHICH
		   is the magic number that tells us which element to delete.  The
		   elements are numbered from 0. */
		hist = remove_history( i );
		if( hist ) {
			if( hist->data ) free(hist->data);
			free(hist);
		}
	}

	/* Place STRING at the end of the history list.
	   The associated data field (if any) is set to NULL. */
	add_history(line);

	/* Return the history entry which is logically at OFFSET in the history
	   array.  OFFSET is relative to history_base. */
	hist = history_get(history_length); 
	if( hist ) {
		// ���ɾ� ó�� �ð��� �߰� ���
		hist->data = (histdata_t*)malloc(32);
		strftime(hist->data, 32, "%H:%M", localtime_r(&t, &ltime)); 
	}
}

/**
 * @brief �Է��� ������ŭ�� history�� �����ϰ� �����ϴ� �Լ�
 * @param max the number of max remains history
 */ 
void simcmd_history_truncate( int max )
{
	HIST_ENTRY *hist;

	if( max <= 0 ) return ;

	while(  history_length > max ) { 
		/* A reasonably useless function, only here for completeness.  WHICH
		   is the magic number that tells us which element to delete.  The
		   elements are numbered from 0. */
		hist = remove_history( history_base-1 );
		if( hist ) {
			if( hist->data ) free(hist->data);
			free(hist);
		}
	}
}

/**
 * @brief history�̷��� ȭ�鿡 ����Ѵ�.
 */
void simcmd_history_print()
{
	int i;
	HIST_ENTRY **hlist;
	hlist = history_list();

	printf( "-------------------------------------------------\n");
    for( i = 0 ; i < history_length ; i++ ) {
		printf ("%4d\t%s\t%-s\n", history_base+i, 
				hlist[i]->data ? (char*)hlist[i]->data : "null", hlist[i]->line);
	}
    printf ("-------------------------------------------------\n");
}	

