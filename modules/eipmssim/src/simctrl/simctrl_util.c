#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include "simctrl_def.h"
#include "simctrl_util.h"


/** @brief display column size */
#define _SIMCTRL_COLUMN_SIZE 100	


/////////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - utilities 
/////////////////////////////////////////////////////////////////////////////////

/**
 * @brief ���ڿ��� �����ϴµ�, ���޵� ũ�� ������ ����ȴ�.
 * @param dst ���ڿ��� ����� ����
 * @param src ������ ���ڿ�
 * @param dstsize ����� ������ ũ��
 * @return ������ ���ڿ��� ����
*/
size_t simctrl_strlcpy(char* dst, const char* src, size_t dstsize)
{
	size_t srcLen, size;
	srcLen = strlen(src);
	size = srcLen;
	if( size >= dstsize ) size = dstsize-1;
	strncpy(dst, src, size);
	dst[size] = '\0';
	return srcLen;
}

/**
 * @brief ���ڿ��� ���Ե� Ư�� ���ڸ� �������ش�. 
 *         system call ����� SYSTEM("cmd") ���� �� system call ���� ����� ��ȯ���ش�.
 * @param src ���� ���ڿ� 
 * @param search �����ϰ��� �ϴ� ����
 * @param change ����� ����
 * @param dst ���ڿ��� ����� ����
 * @param len ����� ������ ũ��
 * @return ���ڿ��� ����� ������ �ּ�
 */
char* simctrl_replace_str( char *src, char *search, char *changed, char *dst, int len )
{
	int wlen = 0, size = 0;
	char buff[256], *p1, *p2, *p_chg;
	FILE *_fp;

	dst[0] = '\0';

	// system call ����� SYSTEM("cmd") ���� ���θ� Ȯ���Ѵ�.
	// ������ ��� system call �������� replace string ó�����ش�.
	p1 = strstr( changed, "SYSTEM(" );
	if( p1 && (p2 = strrchr( p1, ')')) ) { 
		p1 = p1 + strlen("SYSTEM(");
		p2[0] = '\0';
		_fp = popen( p1, "r" );
		p2[0] = ')';
		if( !_fp ) {
			printf( "system call for replace_str failed. err=%d(%s)\n", errno, strerror(errno)); 
			return dst; 
		}
		while( fgets(buff, sizeof(buff), _fp) ) {
			// check carriage return 
			if( buff[0] == '\n' && strlen(buff) == 1) continue;
			simctrl_decode_ascii(buff);
			if( buff[strlen(buff)-1] == '\n' ) buff[strlen(buff)-1] = '\0';
			break; // ���ٸ� �޴´�.
		}
		fflush( _fp );
		pclose( _fp );

		p_chg = buff;
	} else {
		p_chg = changed;
	}

	p1 = (char*)src;
	while( (p2 = strstr(p1, search)) ) {
		size = len <= wlen ? 0 : len-wlen > p2-p1 ? p2-p1 : len-wlen-1;
		if( size > 0 ) {
			strncpy(dst+wlen, p1, size);
			wlen += size;
			dst[wlen] = '\0';
		}
		if( len > wlen ) wlen += simctrl_strlcpy(dst+wlen, p_chg, len-wlen);
		p1 = p2 + strlen(search);
	}
	if( len > wlen ) wlen += simctrl_strlcpy(dst+wlen, p1, len-wlen);
	return dst;
}


/**
 * brief time_t �� ���� format�� �°� string���� ��ȯ���ش�.
 * param time time_t value
 * param fmt string time format
 * param buf string time buffer
 * param buf string time buffer length
 * return buf pointer
*/
char* simctrl_cvt_time_to_str( time_t t, char *fmt, char *buf, int buflen)
{
	int len;
	struct tm ltime;

	if( !buf || buflen <= 0 ) return NULL;

	if( !fmt || fmt[0] == '\0' ) {
		len = strftime( buf, buflen, "%Y/%m/%d %H:%M:%S", localtime_r(&t, &ltime));
	} else {
		len = strftime( buf, buflen, fmt, localtime_r(&t, &ltime));
	}
	return buf;
}


/**
 * @brief ���޵� �Ķ���Ϳ� environment ���� ���� �� ���� �н��� ������ ��ȯ�Ѵ�.
 * @param path ���� �н��� ������ϴ� path�� �Է°�.
 * @param cpath ���� path�� ������ ����
 * @param size cpath�� buffer size
 * @return ���� �н��� ���ڿ� ����
 */
int simctrl_get_conf_path( const char *path, char *cpath, int size)
{
    char *pEnv, *ptr, env[64];

    if ( path[0] == '$' && path[1] == '(' && (ptr = strchr( path,')')) != NULL) {
        strncpy(env, path + 2, ptr - path - 2);
        env[ptr - path - 2] = '\0';
        pEnv = getenv( env);
        if ( pEnv == NULL) return eSIMCTRL_INVALID_CONF;
        snprintf( cpath, size-1, "%s%s", pEnv, ptr + 1);
        return snprintf( cpath, size-1, "%s%s", pEnv, ptr + 1);
    }
    else if ( path[0] == '$' && (ptr = strchr( path,'/')) != NULL) {
        strncpy(env, path + 1, ptr - path - 1);
        env[ptr - path - 1] = '\0';
        pEnv = getenv( env);
        if ( pEnv == NULL) return eSIMCTRL_INVALID_CONF;
        return snprintf( cpath, size-1, "%s%s", pEnv, ptr);
    }
    else if( path[0] == '/' ) {
        return simctrl_strlcpy( cpath, path, size-1);
    }
    else {
        //pEnv = getenv("UASYS_HOME");
        //if ( pEnv == NULL) return eSIMCTRL_INVALID_CONF;
        //return snprintf( cpath, size-1, "%s/%s", pEnv, path);
        return snprintf( cpath, size-1, "./%s", path);
    }
}


/**
 * @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� file open�Ѵ�. 
 * @param fname ���� �н��� ������ϴ� path�� �Է°�.
 * @param mode file open mode (fopen�� mode ����)
 * @return open file pointer
 * @see simctrl_get_conf_path(), fopen()
 */
FILE *simctrl_fopen( char *fname, char *mode )
{
	int len;
	char *path = NULL;
	FILE *fp = NULL;

	if( !fname ) {
		printf("Invalid parameters. fname=%p\n", fname);
		return NULL;
	}

	len = strlen(fname) + 128;
	path = malloc(len);
	if( !path ) {
		printf("Memory alloc failed.\n");
		return NULL;
	}
	simctrl_get_conf_path( fname, path, len);

	fp = fopen(path, mode);
	if( path ) free(path);
	return fp;
}

/**
 * @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� file�� ���� ������ dump�Ѵ�.
 * @param fname ���� �н��� ������ϴ� path�� �Է°�.
 * @return dump buffer pointer 
 * @see simctrl_get_conf_path(), malloc()
 */
char* simctrl_fdump( char *fname )
{
	int fd, len, rcv, sum;
	char *path = NULL, *buf = NULL;
	struct stat fnstat;

	if( !fname ) {
		printf("Invalid parameters. fname=%p\n", fname);
		return NULL;
	}

	len = strlen(fname) + 128;
	path = malloc(len);
	if( !path ) {
		printf("Memory alloc failed.\n");
		return NULL;
	}
	simctrl_get_conf_path( fname, path, len);

	fd = open( path, O_RDONLY, 0);
	if( fd < 0 ) {
		printf("file open failed. file=%s, err=%d(%s)\n",
				path, errno, strerror(errno));
		free(path);
		return NULL;
	}
	if( stat(path, &fnstat) < 0 ) {
		printf("file stat failed. file=%s, err=%d(%s)\n",
				path, errno, strerror(errno));
		free(path);
		close(fd);
		return NULL;
	}
	len = fnstat.st_size + 1024;
	buf = malloc(len);
	if( !buf ) {
		printf("memory alloc failed. err=%d(%s)\n", errno, strerror(errno));
		free(path);
		close(fd);
		return NULL;
	}
	memset(buf, 0, len);

	sum = 0;
	while( (rcv = read(fd, buf+sum, len-sum) ) > 0 ) {
		sum += rcv;
	}

	free(path);
	close(fd);
	return buf;
}

/**
 * @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� file�� remove�Ѵ�. 
 * @param fname ���� �н��� ������ϴ� path�� �Է°�.
 * @return remove result code 
 * @see simctrl_get_conf_path(), remove()
 */
int simctrl_remove( char *fname )
{
	int rv, len;
	char *path = NULL;

	if( !fname ) {
		printf("Invalid parameters. fname=%p\n", fname);
		return eSIMCTRL_INVALID_PARAM; 
	}

	len = strlen(fname) + 128;
	path = malloc(len);
	if( !path ) {
		printf("Memory alloc failed.\n");
		return eSIMCTRL_NO_MEMORY;
	}
	simctrl_get_conf_path( fname, path, len);

	rv = remove(path);
	if( path ) free(path);
	return rv; 
}


/**
 * @brief ���޵� �Ķ���Ϳ� environment�� �����η� ��ȯ�� �� directory�� �����Ѵ�. 
 * @param fname ���� �н��� ������ϴ� path�� �Է°�.
 * @return mkdir result code 
 * @see simctrl_get_conf_path(), mkdir()
 */
int simctrl_mkdir( char *fname )
{
	int rv, len;
	char *path = NULL, *pos = NULL;
    struct stat sinfo;

	if( !fname ) {
		printf("Invalid parameters. fname=%p\n", fname);
		return eSIMCTRL_INVALID_PARAM; 
	} 

	len = strlen(fname) + 128;
	path = malloc(len);
	if( !path ) {
		printf("Memory alloc failed.\n");
		return eSIMCTRL_NO_MEMORY;
	}
	simctrl_get_conf_path( fname, path, len);
	
    // check if directory exist 
    if( path[0] == '\0' || stat( path, &sinfo) >= 0) {
		if( path ) free(path);
        return eSIMCTRL_SUCCESS;
	}

    // this directory doesn't exist. make it 
    if( mkdir(path, 0755) < 0 ) {
        // if 'mkdir' fails, make parent directory first 
		pos = strrchr( path, '/');
		if( pos ) {
			pos[0] = '\0';
			rv = simctrl_mkdir( path );
			if( rv < eSIMCTRL_SUCCESS ) {
				if( path ) free(path);
				return rv;
			}	
			pos[0] = '/';

            if( mkdir(path, 0755) < 0 ) {
                printf( "make directory failed. dir=%s, errno=%d(%s)\n", 
						path, errno, strerror(errno));
				if( path ) free(path);
                return eSIMCTRL_ERROR; 
            } else {
                printf( "make directory success. dir=\"%s\"\n", path);
            }
        }
    }
	if( path ) free(path);
	return eSIMCTRL_SUCCESS;
}

/**
 * @brief file name�� ������ ��ο� ���� env ��ȯ �� directory�� �����Ѵ�.
 * @param fname directory + file name
 * @return result code
 * @see simctrl_mkdir()
 */
int simctrl_mkdir_with_file( char *fname )
{
	int rv;
	char *pos;

	if( !fname ) {
		printf("Invalid parameters. fname=%p\n", fname);
		return eSIMCTRL_INVALID_PARAM; 
	}

	pos = strrchr(fname, '/');
	if( !pos ) return eSIMCTRL_SUCCESS; // file�� ������ ����̴�.

	pos[0] = '\0';
	rv = simctrl_mkdir(fname);
	pos[0] = '/';
	return rv;
}


/**
 * @brief ���޵� chracter�� ���� ���̸�ŭ ���ۿ� ����ϰ� '\n'�� �Է��Ѵ�.
 * @param buf ������ ����ϰ��� �ϴ� ����
 * @param buflen ������ ����
 * @param c ������ ����� �� ����� character
 * @return print line size
 * @see simctrl_display_line(), snprintf()
 */
int simctrl_sprintf_line( char *buf, int buflen, char c)
{
	char line[_SIMCTRL_COLUMN_SIZE];

	if( buf ) buf[0] = '\0';
	if( !buf || buflen <= 0 ) return 0;
	if( c <= ' ' ) c = '-'; 

	memset(line, c, sizeof(line)-1);
	line[sizeof(line)-1] = '\0';
	return snprintf(buf, buflen, "%s\n", line);
}

/**
 * @brief ���޵� chracter�� ���� ���̸�ŭ ȭ�鿡 ����ϰ� '\n'�� ����Ѵ�. 
 * @param c ������ ����� �� ����� character
 * @see simctrl_sprintf_line(), printf()
 */
void simctrl_display_line( char c)
{
	char line[_SIMCTRL_COLUMN_SIZE];
	if( c <= ' ' ) c = '-'; 
	memset(line, c, sizeof(line)-1);
	line[sizeof(line)-1] = '\0';
	printf("%s\n", line);
}


/**
 * @brief ���޵� �Ķ���Ϳ� Ư����ȣ�� ��ȯ�Ѵ�. skip �Ǵ� '.'���� ��ü���ش�.
 * @param buf string buffer
 * @return string buffer pointer
 */
char* simctrl_decode_ascii( char *buf )
{
	int idx = 0, ignore_idx = 0, i;
	unsigned char *pos = (unsigned char*)buf; 

	if( !buf ) return NULL;

	while( *pos ) {
		if( *pos < 0x6/*ACK*/ || *pos >= ' ' || 
				*pos == '\n' || *pos == '\t' ) { // �Ϲ� ���ڴ� �׳� ���д�.
			buf[idx++] = *pos; 
			pos += 1;
			continue;
		}
		ignore_idx = 0;
		if( *pos == 0x1B ) { // escape character
			if( !memcmp(pos+1, "(B", 2) ) ignore_idx = 2; 
			else if( !memcmp(pos+1, ")0", 2) ) ignore_idx = 2;
			else if( !memcmp(pos+1, "[1;45r", 6) ) ignore_idx = 6;
			else if( !memcmp(pos+1, "[45;1H", 6) ) ignore_idx = 6;
			else if( !memcmp(pos+1, "[m", 2) ) ignore_idx = 2;
			else if( !memcmp(pos+1, "[?7h", 4) ) ignore_idx = 4;
			else if( !memcmp(pos+1, "[?1l", 4) ) ignore_idx = 4;
			else if( !memcmp(pos+1, "[2J", 3) ) ignore_idx = 3;
		} else if( *pos == '\r' ) { 
			; // '\r'�� skip�Ѵ�.
		} else { 
			buf[idx++] = '.'; // Ư������ ��� '.'���� ����
		}
		for( i = 0 ; i < ignore_idx + 1 ; i ++ ) pos += 1;
	}
	buf[idx] = '\0';
	return buf;
}


/**
 * @brief process���� kill signal�� ���� �����Ų��.
 * @param pid process id
 * @param kill_child_flag child process�� kill ���� ���� �÷���
 */
void simctrl_kill( int pid, int kill_child_flag )
{
	int _pid;
	char buff[256];
	FILE *_fp;

	if( pid <= 1 ) {
		printf( "kill failed. invalid pid(%d)\n", pid);
		return ;
	}

	sprintf(buff, "ps -ef | grep %d | awk '{ if( $3 == %d ) print $2 }'", pid, pid); 
	_fp = popen(buff, "r");
	if( !_fp ) {
		printf( "kill failed. err=%d(%s)\n", errno, strerror(errno));
		return ;
	}

	while( fgets(buff, sizeof(buff), _fp) ) {
		_pid = strtol(buff, 0, 0);
		if( _pid >= 1 ) {
			if( kill_child_flag ) { // kill�Ϸ��� process�� child�� kill�Ѵ�.
				simctrl_kill( _pid, kill_child_flag ); 
			}
			kill( _pid, SIGKILL );
			printf("kill success. pid=%d\n", _pid); 
		}
	}

	fflush( _fp );
	pclose( _fp );
}


/////////////////////////////////////////////////////////////////////////////////
// private functions for simulator controller - utilities 
/////////////////////////////////////////////////////////////////////////////////
