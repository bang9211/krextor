#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "simctrl_report.h"
#include "simctrl_util.h"


simctrl_report_chk_f g_report_chk_f = NULL;
simctrl_report_cmp_f g_report_cmp_f = NULL;


static int _simctrl_report_check( char *ebuf, char *rbuf, int type, char *result, int reslen );
static int _simctrl_report_chkmsg( char *ebuf, char *rbuf, int type, char *result, int reslen );

static int _simctrl_diff_cmp( char *a, char *b );
static int _simctrl_diff_print1( char *pos, int line, char *result, int reslen );
static int _simctrl_diff_print2( char *pos1, char *pos2, int type, char *result, int reslen );

static int _simctrl_diff_is_print_seagull( char *pos );
static int _simctrl_diff_is_print_sipp( char *pos );
static int _simctrl_diff_is_print_default( char *pos );

static int _simctrl_get_msglist( char *msg, int type, char *buf, int buflen );
static int _simctrl_get_msglist_seagull( char *msg, char *buf, int buflen );
static int _simctrl_get_msglist_sipp( char *msg, char *buf, int buflen );
static char* _simctrl_get_end_of_message( char *msg, int type, int *extern_line );
static int _simctrl_is_end_of_message( char *msg, int type );


//////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - report
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief simulator controller report check ó�� callback �Լ� ����
 * @param func report check callback function pointer
 */
void simctrl_report_set_chk_callback( simctrl_report_chk_f func )
{
	g_report_chk_f = func;
}

/**
 * @brief simulator controller report ó�� �� �߰� line compare ó�� �Լ� ����
 * @param func report compare callback function pointer
 */
void simctrl_report_set_cmp_callback( simctrl_report_cmp_f func )
{
	g_report_cmp_f = func;
}


/**
 * @brief simulator controller report ó�� �Լ� 
 * @param expected �������� ó�� ��� �̷� ����
 * @param received �ù����̼� ��� �̷� ����
 * @type scenario type (none/seagull/sipp ...)
 * @result diff record buffer
 * @reslen diff record buffer length
 * @result result code (0:����, �׿�:����)
 */
int simctrl_report_check( char *expected, char *received, int type, char *result, int reslen )
{
	int rv, wlen=0;
	char *ebuf = NULL, *rbuf = NULL;
	FILE *fp;

	if( !expected || !received || (result && reslen <= 1 ) ) {
		printf("Invalid parameters. expected=%p, received=%p, result=%p, reslen=%d\n",
				expected, received, result, reslen);
		return eSIMCTRL_INVALID_PARAM;
	}
	if( result ) result[0] = '\0';

	// file�� ���� ��ü �����͸� dump�ؿ´�.
	rbuf = simctrl_fdump( received );
	if( !rbuf ) {
		printf("file dump failed. fname=%s\n", received);
		if( result ) {
			wlen += snprintf(result+wlen, (reslen > wlen) ? reslen-wlen : 0,
					"file dump failed. fname=%s\n", received);
		}
		return eSIMCTRL_FILE_OPEN_FAIL;
	}
	ebuf = simctrl_fdump( expected );
	if( !ebuf ) {
		printf("file dump failed. fname=%s\n", expected);
		if( result ) {
			wlen += snprintf(result+wlen, (reslen > wlen) ? reslen-wlen : 0, 
					"file dump failed. fname=%s\n", expected);
		}

		// file open �����̸� �����ϰ� received �̷��� ������ش�. 
		fp = simctrl_fopen( expected, "r" );
		if( !fp ) {
			fp = simctrl_fopen( expected, "w" );
			if( fp ) {
				fwrite( rbuf, 1, strlen(rbuf), fp);
				fclose(fp);
				printf("make expected file and write received trace.\n");
				if( result ) {
					wlen += snprintf(result+wlen, (reslen > wlen) ? reslen-wlen : 0,
							"make expected file and write received trace.\n");
				}
			}
		} else fclose(fp); 

		free(rbuf);
		return eSIMCTRL_FILE_OPEN_FAIL;
	}

	// expected - received ���� ��ü�� ���Ѵ�. plugin�� ��ϵǾ������� plugin���� ���Ѵ�.
	if( g_report_chk_f ) rv = g_report_chk_f( ebuf, rbuf, type, result+wlen, reslen-wlen); 
	else rv = _simctrl_report_check( ebuf, rbuf, type, result+wlen, reslen-wlen); 

	if( result[wlen] ) { // ���� result ������ �߰��� ���̴�. �̷� ��� ���� ���� ��������.
		wlen = strlen(result);
		snprintf(result+wlen, reslen-wlen > 0 ? reslen-wlen : 0, 
				"## expected report file is '%s'\n", expected);
	}

	free(ebuf);
	free(rbuf);
	return rv;
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

/** @internal report check function. expected - received ��ü ������ ���Ѵ�. */
static int _simctrl_report_check( char *ebuf, char *rbuf, int type, char *result, int reslen )
{
	int rv, wlen=0, is_fail=0, line=0, extern_line=0;
	char *epos, *epos1, *rpos, *rpos1, *rpos2;

	epos = ebuf;
	rpos = rbuf;

	// expected�� receive�� recv/send message list�� ������ ���Ѵ�.
	rv = _simctrl_report_chkmsg( ebuf, rbuf, type, result, reslen );
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	// expected�� �Ѷ��ξ� �о� receive�� ���Ѵ�.
	while( epos[0] != '\0' ) {
		epos1 = _simctrl_get_end_of_message( epos, type, &extern_line );
		if( epos1 ) epos1[0] = '\0';
		rv = -1;
		line += 1;

		// expected line�� ��ġ�ϴ� received line�� ã�´�.
		rpos1 = rpos;
		while( rpos1[0] != '\0' ) {
			rpos2 = _simctrl_get_end_of_message( rpos1, type, NULL );
			if( rpos2 ) rpos2[0] = '\0';
			rv = _simctrl_diff_cmp(epos, rpos1);
			if( rpos2 ) rpos2[0] = '\n';

			if( rv == 0 ) { // expected�� ��ġ�ϱ� ������ received line ���
				wlen += _simctrl_diff_print2( rpos, rpos2, type, 
							result ? result+wlen : NULL, reslen-wlen);
				if( rpos2 ) rpos = rpos2 + 1;
				else rpos = rpos + strlen(rpos);
				break;
			} else {
				if( _simctrl_is_end_of_message(rpos1, type) ) { // �޽��� ������ ��ã���� ����.. 
					rv = -1;
					break;
				}
			}

			if( rpos2 ) rpos1 = rpos2 + 1;
			else break; // last line case
		}

		// �� ��� expected line�� ��ġ�ϴ� received line�� ����. expected line ���
		if( rv != 0 ) {
			wlen += _simctrl_diff_print1( epos, line, result ? result+wlen : NULL, reslen-wlen );
			is_fail = 1;
		}

		if( epos1 ) {
			epos1[0] = '\n';
			epos = epos1 + 1;
		} else break; // last line case
		if( extern_line > 0 ) line += extern_line;
	}

	// expected line�� ��� ���ϰ� ���� received line�� ���
	wlen += _simctrl_diff_print2( rpos, NULL, type, result ? result+wlen : NULL, reslen-wlen);

	return is_fail ? eSIMCTRL_ERROR : eSIMCTRL_SUCCESS;
}

/** @internal report check function. expected - received �޽��� ������ ���Ѵ�. */ 
static int _simctrl_report_chkmsg( char *ebuf, char *rbuf, int type, char *result, int reslen )
{ 
	int rv, len;
	char *emsg, *rmsg;

	len = strlen(ebuf);
	emsg = malloc(len);
	if( !emsg ) {
		return eSIMCTRL_ERROR;
	}
	_simctrl_get_msglist( ebuf, type, emsg, len );

	len = strlen(rbuf);
	rmsg = malloc(len);
	if( !rmsg ) {
		free(emsg);
		return eSIMCTRL_ERROR;
	}
	_simctrl_get_msglist( rbuf, type, rmsg, len );

	if( strcmp(emsg, rmsg) ) {
		printf("## Check! diff message result.\n < %s\n > %s\n", emsg, rmsg);
		fflush( stdout );
		if( result ) {
			snprintf(result, reslen > 0 ? reslen : 0, 
					"## Check! diff message result.\n < %s\n > %s\n", emsg, rmsg);
		}
		rv = eSIMCTRL_ERROR;
	} else rv = eSIMCTRL_SUCCESS;

	free(emsg);
	free(rmsg);
	return rv;
}


/** @internal ���� �� �Լ�. ignore keyword �� extension compare ó�� ��� ���� */
static int _simctrl_diff_cmp( char *a, char *b )
{
	int rv, alen, blen, ilen;
	char *pos, *pos2, *aa, *bb;

	if( !a || !b ) return -1;

	// ABC/?/DEF/?/GH -> ABC DEF GH	
	// ABC1DEF23456GH -> ABC 1DEF 23456GH

	// check ignore keyword 
	pos = strstr(a, "/?/");
	if( pos ) {
		// pos ������ ���Ѵ�.
		*pos = '\0';
		pos2 = strstr(b, a);
		rv = pos2 ? 0 : -1;
		*pos = '/';
		if( rv == 0 && pos[3] != '\0' ) {
			pos += 3;
			pos2 = strstr(pos, "/?/");
			if( pos2 ) {
				ilen = pos - a;
				aa = a + ilen;
				bb = b + ilen - 3; // pos�� /?/ 3�ڸ� ���ԵǾ��־� b���� ���ش�.
				rv = _simctrl_diff_cmp(aa, bb);
			} else {
				ilen = strlen(pos);
				alen = strlen(a);
				blen = strlen(b);
				aa = a + alen - ilen;
				if( blen > ilen ) bb = b + blen - ilen;
				else bb = b + blen;
				rv = strcmp(aa, bb);
			}
		}
	} else {
		// call extension compare function 
		if( !g_report_cmp_f ) rv = strcmp(a, b);
		else rv = g_report_cmp_f(a, b);
	}
	return rv;
}

/** @internal expected �κ� diff ���� ��� �Լ� */
static int _simctrl_diff_print1( char *pos, int line, char *result, int reslen )
{
	int len, wlen = 0;
	char *pos2, *pos3, *buff;

	if( !pos ) return 0;

	if( line >= 0 ) {
		printf("## Check! expected line : %d\n", line);
		if( result ) {
			wlen += snprintf(result+wlen, reslen > wlen ? reslen-wlen : 0,
					"## Check! expected line : %d\n", line);
		}
	}

	pos2 = pos;
	while( pos2[0] != '\0' ) {
		pos3 = strchr(pos2, '\n');
		if( pos3 ) pos3[0] = '\0';
		
		len = strlen(pos2) * 4;
		buff = malloc( len );
		simctrl_replace_str( pos2, "\t", "\\t\\\\", buff, len );
		if( buff[strlen(buff)-1] == '\r' ) buff[strlen(buff)-1] = '\0';

		printf("%s\'%s\'\n", pos2 == pos ? " < " : "   ", buff);
		if( result ) {
			wlen += snprintf(result+wlen, reslen > wlen ? reslen-wlen : 0,
					"%s\'%s\'\n", pos2 == pos ? " < " : "   ", buff);
		}

		free( buff );

		if( pos3 ) {
			pos3[0] = '\n';
			pos2 = pos3 + 1;
		} else break; 
	}
	fflush( stdout );
	return wlen;
}

/** @internal received�κ� diff ���� ��� �Լ� */
static int _simctrl_diff_print2( char *pos1, char *pos2, int type, char *result, int reslen )
{
	int len, wlen = 0, is_print = 0;
	char *pos3, *tmp1, *tmp2, *buff;

	if( !pos1 ) return 0;

	while( pos1[0] != '\0' ) {
		pos3 = _simctrl_get_end_of_message( pos1, type, NULL );
		if( pos3 == pos2 ) break; // strcmp == 0 line
		if( pos3 ) pos3[0] = '\0';

		if( type == SIMCTRL_SCEN_TYPE_SEAGULL ) {
			is_print = _simctrl_diff_is_print_seagull(pos1); 
		} else if( type == SIMCTRL_SCEN_TYPE_SIPP ) {
			is_print = _simctrl_diff_is_print_sipp(pos1);
		} else {
			is_print = _simctrl_diff_is_print_default(pos1);
		}
		if( is_print ) {
			tmp1 = pos1;
			while( tmp1[0] != '\0' ) {
				tmp2 = strchr(tmp1, '\n');
				if( tmp2 ) tmp2[0] = '\0';

				len = strlen(tmp1) * 4;
				buff = malloc( len );
				simctrl_replace_str( tmp1, "\t", "\\t\\\\", buff, len );
				if( buff[strlen(buff)-1] == '\r' ) buff[strlen(buff)-1] = '\0';

				printf("%s\'%s\'\n", tmp1 == pos1 ? " > " : "   ", buff);
				if( result ) {
					wlen += snprintf(result+wlen, reslen > wlen ? reslen-wlen : 0,
							"%s\'%s\'\n", tmp1 == pos1 ? " > " : "   ", buff);
				}

				free( buff );

				if( tmp2 ) {
					tmp2[0] = '\n';
					tmp1 = tmp2 + 1;
				} else break;
			}
		}

		if( pos3 ) {
			pos3[0] = '\n';
			pos1 = pos3+1;
		} else break; // last line case
	}
	fflush( stdout );
	return wlen;
}


/** @internal seagull type�� ��� diff ������ print���� ���� Ȯ�� */
static int _simctrl_diff_is_print_seagull( char *pos )
{
	if( !pos || pos[0] == '\0' ) return 0;
	if( !strncmp(pos, "|A|", 3) ) return 1; 
	if( !strncmp(pos, "|E|", 3) ) return 1; 
	if( !strncmp(pos, "|M|", 3) ) return 1; 
	if( !strncmp(pos, "|T|", 3) ) return 1; 
	if( !strncmp(pos, "| |", 3) ) return 1; 
	return 0;
}

/** @internal sipp type�� ��� diff ������ print���� ���� Ȯ�� */
static int _simctrl_diff_is_print_sipp( char *pos )
{
	if( !pos || pos[0] == '\0' ) return 0;
	return 1;
}

/** @internal default type�� ��� diff ������ print���� ���� Ȯ�� */
static int _simctrl_diff_is_print_default( char *pos )
{
	if( !pos || pos[0] == '\0' ) return 0;
	return 1;
}


/** @internal message list ��ȸ �Լ� */
static int _simctrl_get_msglist( char *msg, int type, char *buf, int buflen )
{
	int rv = 0;

	buf[0] = '\0';

	if( type == SIMCTRL_SCEN_TYPE_SEAGULL ) {
		rv = _simctrl_get_msglist_seagull( msg, buf, buflen );
	} else if( type == SIMCTRL_SCEN_TYPE_SIPP ) {
		rv = _simctrl_get_msglist_sipp( msg, buf, buflen );
	}
	return 0;
}

/** @internal seagull message list ��ȸ �Լ� */
static int _simctrl_get_msglist_seagull( char *msg, char *buf, int buflen )
{
	int wlen = 0, val_i;
	char *pos = msg;
	char *pos2;

	while( pos[0] != '\0' ) {
		pos2 = strchr(pos, '\n');
		if( pos2 ) pos2[0] = '\0';

		if( !strncmp(pos, "|M|Received [[", strlen("|M|Received [[")) ) { // receive message
			wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%sRcv %s",
					wlen ? ", " : "", pos + strlen("|M|Received [["));
		} else if( !strncmp(pos, "|M|Send [[", strlen("|M|Send [[")) ) { // send message
			wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%sSnd %s",
					wlen ? ", " : "", pos + strlen("|M|Send [["));
		} else if( !strncmp(pos, "|M|[flags] = [ ", strlen("|M|[flags] = [ ")) ) { // command flag
			val_i = strtol(pos+strlen("|M|[flags] = [ "), 0, 0);
			buf[wlen-1] = val_i >= 128 ? 'R' : 'A'; // request/answer
		} else if( !strncmp(pos, "|M| (avp-code = 416)(flags = 64)] = [",
					strlen("|M| (avp-code = 416)(flags = 64)] = [")) ) { // CC-Request-Type
			val_i = strtol(pos+strlen("|M| (avp-code = 416)(flags = 64)] = ["), 0, 0);
			wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%s", 
					val_i == 1 ? "-I" : val_i == 2 ? "-U" : val_i == 3 ? "-T" : "-E");
		}
		
		if( pos2 ) {
			pos2[0] = '\n';
			pos = pos2 + 1;
		} else break;
	}
	return wlen;
}

/** @internal sipp message list ��ȸ �Լ� */
static int _simctrl_get_msglist_sipp( char *msg, char *buf, int buflen )
{
	int wlen = 0, is_unexpected = 0, is_request = 0;
	char *pos = msg;
	char *pos2, *pos3, *pos4;

	while( pos[0] != '\0' ) {
		pos2 = strchr(pos, '\n');
		if( pos2 ) pos2[0] = '\0';

		if( !strncmp(pos, "Unexpected", 10) && strstr(pos, "message ") ) { // Unexpected message
			is_unexpected = 1;
		} else if( strstr(pos, " message received ") ) { // receive keyword
			wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%sRcv ", 
					wlen ? ", " : "");
			is_unexpected = 0;
		} else if( strstr(pos, " message sent ") ) { // send keyword
			wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%sSnd ", 
					wlen ? ", " : "");
			is_unexpected = 0;
		} else if( !is_unexpected &&
				(!strncmp(pos, "INVITE ", 7) || !strncmp(pos, "ACK ", 4) 
				|| !strncmp(pos, "OPTIONS ", 8) || !strncmp(pos, "CANCEL ", 7) 
				|| !strncmp(pos, "BYE ", 4) || !strncmp(pos, "UPDATE ", 7) 
				|| !strncmp(pos, "INFO ", 5) || !strncmp(pos, "REFER ", 6) 
				|| !strncmp(pos, "NOTIFY ", 7) || !strncmp(pos, "MESSAGE ", 8) 
				|| !strncmp(pos, "PRACK ", 6)) ) { // request message
			pos3 = strchr(pos, ' ');
			pos3[0] = '\0';
			wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%s", pos);
			pos3[0] = ' ';
			is_request = 1;
		} else if( !is_unexpected && !strncmp(pos, "SIP/", 4) ) {
			pos3 = strchr(pos, ' ');
			if( pos3 ) {
				pos3 = pos3 + 1; // "SIP/2.0 200 OK" �����̴�. �ι�° ���ڰ��� ��� 
				wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "%c%c%c",
						pos3[0], pos3[1], pos3[2]);
				is_request = 0;
			}
		} else if( !is_unexpected && !is_request && !strncmp(pos, "CSeq: ", 6) ) { // request or response message
			pos3 = strrchr(pos, ' ');
			if( pos3 ) {
				pos3 = pos3 + 1; // "CSeq: 1 INVITE" �����̴�. ������ Request ������ ���
				pos4 = strchr(pos3, '\r');
				if( pos4 ) pos4[0] = '\0'; // \r\n �� ���� ��ϵǹǷ� \r�� �������־���Ѵ�.
				wlen += snprintf(buf+wlen, buflen > wlen ? buflen-wlen : 0, "(%s)", pos3);
				if( pos4 ) pos4[0] = '\r';
			}
		}

		if( pos2 ) {
			pos2[0] = '\n';
			pos = pos2 + 1;
		} else break;
	}

	return 0;
}

/** @internal ���� �޽����� ������ ��ġ�� ��ȸ�Ѵ�. */
static char* _simctrl_get_end_of_message( char *msg, int type, int *extern_line )
{
	int line = 0;
	char *pos, *tmp;

	pos = strchr( msg, '\n' );

	if( type == SIMCTRL_SCEN_TYPE_SEAGULL ) {
		if( pos && !strncmp(msg, "|M| [", 5) ) { // �ֻ��� AVP ���� ��ġ
			tmp = pos + 1;
			while( tmp[0] != '\0' && (pos = strchr(tmp, '\n')) ) {
				if( !strncmp(tmp, "|M| [", 5) // ���� line���� AVP�� ������, ���� AVP�� ��� 
						|| !strncmp(tmp, "|M|]", 4) ) { // �޽����� ���� ���
					pos = tmp - 1;
					break;
				}
				line += 1;
				if( !strncmp(tmp, "|M| ]", 5) ) break; // �ֻ��� AVP ���� ��ġ
				tmp = pos + 1;
			}
		}
	}

	if( extern_line ) *extern_line = line;
	return pos;
}

/** @internal �޽����� ���������� Ȯ���Ѵ�. */
static int _simctrl_is_end_of_message( char *msg, int type )
{
	if( type == SIMCTRL_SCEN_TYPE_SEAGULL ) {
		if( !strncmp(msg, "|M|]", 4) ) return 1;
	} else if( type == SIMCTRL_SCEN_TYPE_SIPP ) {
		if( msg[0] == '\r' && msg[1] == '\n' ) return 1;
		if( msg[0] == '\n' ) return 1;
	}
	return 0;
}
