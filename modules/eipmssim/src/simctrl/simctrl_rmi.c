#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "simctrl_rmi.h"


//////////////////////////////////////////////////////////////////////////////
// private defines, predefine functions
//////////////////////////////////////////////////////////////////////////////

#define _RMI_COMID_CONNECT_SCHEDSVR       	190
#define _RMI_COMID_MMC_CMD					2001
#define _RMI_REQ_TYPE_TEXT					8312	

#define _RMI_RESULT_CONTINUE				1

/** @brief private simulator controller rmi message header type */
typedef struct _simctrl_rmi_msghdr_s _simctrl_rmi_msghdr_t;
/** @brief private simulator controller rmi message type */
typedef struct _simctrl_rmi_msg_s _simctrl_rmi_msg_t;
/** @brief private simulator controller rmi mml header type */
typedef struct _simctrl_rmi_mmlhdr_s _simctrl_rmi_mmlhdr_t;
/** @brief private simulator controller rmi mml type */
typedef struct _simctrl_rmi_mml_s _simctrl_rmi_mml_t;

/** @brief private simulator controller rmi message header struct */
struct _simctrl_rmi_msghdr_s {
	int msgLen;   	/**< message length */
	int tId;      	/**< transaction Id */
	short tType;	/**< transaction Id type */
	short comId;	/**< command Id */
	int dataLen;    /**< data length (without message header length) */
}; // ~/UALIB/include/ua_ompib.h 

/** @brief private simulator controller rmi message struct */
struct _simctrl_rmi_msg_s {
	_simctrl_rmi_msghdr_t header; /**< message header instance */
	char data[40960]; /**< message data buffer */
};

/** @brief private simulator controller rmi mml header struct */
struct _simctrl_rmi_mmlhdr_s {
	short msgLen; 				/**< message length */
	short srcProc;				/**< source process id */
	char userName[20];			/**< user name (login user) */
	struct in_addr clientIp; 	/**< rmi client host ip */
	int clientPid;				/**< client process id */
	int clientFd;               /**< bhtak 2004/4/22 peer client fd value */
	char destHostName[12];   	/**< ���ɾ ���� �뱹 ȣ��Ʈ �̸� */
	char cdsTid[8];          	/**< CDS IB transaction id */
	char reserved[4];			/**< reserved field */
	short tid;					/**< transaction Id */
	short result;				/**< result code */
}; // ~/UALIB/include/ua_socklib.h

/** @brief private simulator controller rmi mml struct */
struct _simctrl_rmi_mml_s {
	_simctrl_rmi_mmlhdr_t header; /**< mml header instance */
	char data[10240]; /**< mml data buffer */
};


static int _simctrl_rmi_connect( simctrl_rmi_t *rmi );
static int _simctrl_rmi_watch_msg( simctrl_rmi_t *rmi );
static int _simctrl_rmi_read_msg( simctrl_rmi_t *rmi, _simctrl_rmi_msg_t *msg );
static int _simctrl_rmi_send_msg( simctrl_rmi_t *rmi, _simctrl_rmi_msg_t *msg );
static int _simctrl_rmi_send_login( simctrl_rmi_t *rmi );
static int _simctrl_rmi_send_request( simctrl_rmi_t *rmi, char *cmd );


//////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - rmi 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief simulator controller rmi instance
 * @return rmi instance
 */
simctrl_rmi_t* simctrl_rmi_instance()
{
	static simctrl_rmi_t _singleton;
	return &_singleton;
}


/**
 * @brief simulator controller rmi instance �ʱ�ȭ
 * @param rmi rmi instance
 * @param host hostname or ip address
 * @param port port number
 * @param msec socket channel select timeout(miliseconds)
 * @return result code
 * @see simctrl_rmi_final()
 */
int simctrl_rmi_init(simctrl_rmi_t *rmi, char *host, short port, int msec )
{
	if( !rmi || !host || port <= 0 ) {
		printf("Invalid parameters. rmi=%p, host=%p, port=%d\n", rmi, host, port);
		return eSIMCTRL_INVALID_PARAM;
	}
//	if( rmi->init_flag ) {
//		printf("Already init rmi instance.\n");
//		return eSIMCTRL_ALREADY_EXIST;
//	}

	// set host, port
	if( strlen(host) >= sizeof(rmi->host) ) {
		printf("Too big rmi host. host=%s, buffer size=%lu\n", host, sizeof(rmi->host));
		return eSIMCTRL_INVALID_PARAM;
	}
	strcpy(rmi->host, host);
	rmi->port = port;

	// set select timeout
	if( msec > 0 ) {
		rmi->timeout.tv_sec = msec / 1000;
		rmi->timeout.tv_usec= (msec % 1000) * 1000;
	} else {
		rmi->timeout.tv_sec = 0;
		rmi->timeout.tv_usec= 100000;
	}

	// clear variables
	if( rmi->init_flag && rmi->sd >= 0 ) {
		simctrl_rmi_close(rmi); // ������ ������ �Ǿ��־��� ���� �����Ƿ� �������ش�.
	} else {
		rmi->sd = -1;
	}
	rmi->login_result_flag = 0;
	rmi->init_flag = 1;

	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller rmi final
 * @param rmi rmi instance
 * @see simctrl_rmi_init()
 */
void simctrl_rmi_final(simctrl_rmi_t *rmi)
{
	if( !rmi && !rmi->init_flag ) return ;
	simctrl_rmi_close(rmi);
	memset(rmi, 0, sizeof(simctrl_rmi_t));
	rmi->sd = -1;
}


/**
 * @brief simulator controller rmi mmc�� ���� �õ�
 * @param rmi rmi instance
 * @return result code
 * @see simctrl_rmi_close()
 */
int simctrl_rmi_connect(simctrl_rmi_t *rmi)
{
	int rv;

	if( !rmi ) {
		printf("Invalid parameters. rmi=%p\n", rmi);
		return eSIMCTRL_INVALID_PARAM;
	}
	if( !rmi->init_flag ) {
		printf("Do not init rmi insstance.\n");
		return eSIMCTRL_INVALID_PARAM;
	}
	if( rmi->sd >= 0 ) {
		printf("Already exist rmi channel.\n");
		return eSIMCTRL_ALREADY_EXIST;
	}
		
	// connect
	rv = _simctrl_rmi_connect(rmi);
	if( rv < eSIMCTRL_SUCCESS ) return rv;
	rmi->sd = rv;

	// send login 
	rv = _simctrl_rmi_send_login( rmi );
	if( rv < eSIMCTRL_SUCCESS ) {
		simctrl_rmi_close(rmi);
		return rv;
	}
	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller rmi mmc�� ���� ���� �õ�
 * @param rmi rmi instance
 * @return result code
 * @see simctrl_rmi_connect()
 */
int simctrl_rmi_close(simctrl_rmi_t *rmi)
{
	if( !rmi ) {
		printf("Invalid parameters. rmi=%p\n", rmi);
		return eSIMCTRL_INVALID_PARAM;
	}
	if( !rmi->init_flag ) {
		printf("Do not init rmi instance.\n");
		return eSIMCTRL_INVALID_PARAM;
	}
	if( rmi->sd < 0 ) {
		printf("Do not connect rmi channel.\n");
		return eSIMCTRL_INVALID_PARAM;
	}

	// send logout

	// close socket descriptor
	close(rmi->sd);
	rmi->sd = -1;
	return eSIMCTRL_SUCCESS;
}


/**
 * @brief simulator controller rmi ���� ȭ�鿡 ���
 * @param rmi rmi instance
 */
void simctrl_rmi_display(simctrl_rmi_t *rmi)
{
	if( !rmi ) return ;
	printf("init_flag = %s\n", rmi->init_flag ? "TRUE" : "FALSE");
	printf("host      = %s\n", rmi->init_flag ? rmi->host : "");
	printf("port      = %d\n", rmi->init_flag ? rmi->port : 0);
	printf("connect   = %s", rmi->init_flag ? rmi->sd >= 0 ? "Connected" : "Disconnected" : "");
	if( rmi->init_flag ) printf(" sd(%d)\n", rmi->sd);
	else printf("\n");
	fflush( stdout );
}


/**
 * @brief simulator controller rmi mmc���ɾ mmcd�� ���� 
 * @param rmi rmi instance
 * @param cmd mmc command string
 * @return result code (eSIMCTRL_SUCCESS : ���� ����, �׿� ����)
 * @see simctrl_rmi_recv_result() 
 */
int simctrl_rmi_send_request(simctrl_rmi_t *rmi, char *cmd)
{
	if( !rmi || !cmd ) {
		printf("Invalid parameters. rmi=%p, cmd=%p\n", rmi, cmd);
		return eSIMCTRL_INVALID_PARAM;
	}
	if( !rmi->init_flag || rmi->sd < 0 ) {
		printf("Do not init rmi instance or connect rmi channel.\n");
		return eSIMCTRL_INVALID_PARAM;
	}
	return _simctrl_rmi_send_request(rmi, cmd);
}

/**
 * @brief simulator controller rmi mmc���ɾ� ó�� ����� mmcd�� ���� ����
 * @param rmi rmi instance
 * @param display_type display type (0:not display, 1:normal display, 2:report display)
 * @return result code
 *         eSIMCTRL_SUCCESS       : ��� ���� ����(�Ϸ�)
 *         eSIMCTRL_SUCC_CONTINUE : ��� ���� ����(������)
 *         eSIMCTRL_MSG_CONTINUE  : ��� ���� ����(������ - ���� �޽����� ���� ���� ��)
 *         eSIMCTRL_NOT_FOUND     : ��� �� ����(select timeout)
 *         �׿� ���� 
 * @see simctrl_rmi_send_request()
 */
int simctrl_rmi_recv_result(simctrl_rmi_t *rmi, int display_type)
{
	int rv, first_flag = 1;
	char *pos1, *pos2;
	_simctrl_rmi_msg_t msg;
	_simctrl_rmi_mml_t *mml;

	if( !rmi ) {
		printf("Invalid parameters. rmi=%p\n", rmi);
		return eSIMCTRL_INVALID_PARAM;
	}
	if( !rmi->init_flag || rmi->sd < 0 ) {
		printf("Do not init rmi instance or connect rmi channel.\n");
		return eSIMCTRL_INVALID_PARAM;
	}

	rv = _simctrl_rmi_watch_msg(rmi);
	if( rv < eSIMCTRL_SUCCESS ) return rv; 

	rv = _simctrl_rmi_read_msg(rmi, &msg);
	if( rv < eSIMCTRL_SUCCESS ) return rv;

	if( !rmi->login_result_flag ) { // �α��� ��û ����� ȭ�鿡 ������� �ʵ���~
		rmi->login_result_flag = 1;
		return eSIMCTRL_SUCCESS;
	} else {
		mml = (_simctrl_rmi_mml_t*)msg.data;
		if( display_type == 1 ) {
			printf("%s", mml->data); 
		} else if( display_type >= 2 ) {
			pos1 = pos2 = mml->data;
			while( (pos2 = strchr(pos1, '\n')) ) {
				pos2[0] = '\0';
				if( first_flag && display_type == 3 ) {
					printf("%s\n", pos1);
					first_flag = 0;
				} else {
					printf("\t| %s\n", pos1);
				}
				pos2[0] = '\n';
				pos1 = pos2+1;
			}
			if( pos1[0] != '\0' ) printf("\t| %s", pos1);
		}
		fflush( stdout );
	}

	if( mml->header.result == _RMI_RESULT_CONTINUE ) {
		return eSIMCTRL_MSG_CONTINUE;
	} else if( strstr(mml->data, "ACCEPTED\n") || strstr(mml->data, "CONTINUED\n") ) {
		return eSIMCTRL_SUCC_CONTINUE;
	} else {
		return eSIMCTRL_SUCCESS;
	}
}


/**
 * @brief simulator controller rmi mmc���ɾ� ���� (connect -> send -> recv -> close)
 * @param rmi rmi instance
 * @param cmd mmc command string
 * @param display_type display type (0:not display, 1:normal display, 2:report display)
 * @return result code (eSIMCTRL_SUCCESS : ���� ����, �׿� ����)
 * @see simctrl_rmi_connect(), simctrl_rmi_close(), 
 *      simctrl_rmi_send_request(), simctrl_rmi_recv_result()
 */
int simctrl_rmi_run(simctrl_rmi_t *rmi, char *cmd, int display_type)
{
	int rv, cnt = 0, connect_flag = 0;

	if( !rmi || !cmd || !cmd[0] ) {
		printf("Invalid parameters. rmi=%p, cmd=%p(%s)\n", rmi, cmd, cmd ? cmd : "");
		return eSIMCTRL_INVALID_PARAM;
	}
	if( !rmi->init_flag ) {
		printf("Do not init rmi instance.\n");
		return eSIMCTRL_INVALID_PARAM;
	}
	if( rmi->sd < 0 ) {
		rv = simctrl_rmi_connect( rmi );
		if( rv != eSIMCTRL_SUCCESS ) return rv; 
		connect_flag = 1;
	}

	rv = simctrl_rmi_send_request( rmi, cmd );
	if( rv != eSIMCTRL_SUCCESS ) goto final; 

	while( (rv = simctrl_rmi_recv_result(rmi, display_type)) != eSIMCTRL_SUCCESS ) {
		if( rv == eSIMCTRL_SUCC_CONTINUE || rv == eSIMCTRL_MSG_CONTINUE ) {
			cnt = 0;
			continue;
		} else if( rv == eSIMCTRL_NOT_FOUND ) { 
			cnt += 1;
			if( cnt >= 100 ) {
				printf("Too many receive rmi command result...\n");
				rv = eSIMCTRL_ERROR;
				goto final;
			}
			usleep(10000);
		} else goto final; 
	}

	rv = eSIMCTRL_SUCCESS;

final:
	if( connect_flag ) simctrl_rmi_close( rmi );
	return rv;
}


//////////////////////////////////////////////////////////////////////////////
// private functions for simulator controller - rmi 
//////////////////////////////////////////////////////////////////////////////

/** @internal mmcd�� tcp connect �õ��Ѵ�. */
static int _simctrl_rmi_connect( simctrl_rmi_t *rmi )
{
	int rv, sd, val;
	struct hostent *hp;  
	struct sockaddr_in addr;
	struct linger lin;

	memset(&addr, 0, sizeof(addr));
	hp = gethostbyname(rmi->host);
	if( hp ) {
		addr.sin_family = hp->h_addrtype;
		addr.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;
	} else {
		addr.sin_family = AF_INET; // Internet address family
		addr.sin_addr.s_addr = inet_addr(rmi->host); 
	}
	addr.sin_port = htons(rmi->port);

	sd = socket(addr.sin_family, SOCK_STREAM, 0);
	if( sd < 0 ) {
		printf("socket() failed. err=%d(%s)\n", errno, strerror(errno));
		return eSIMCTRL_ERROR;
	}

	val = 1024*1024*10;
	rv = setsockopt(sd, SOL_SOCKET, SO_RCVBUF, (int*)&val, sizeof(val));
	if( rv < 0 ) {
		printf("setsockopt() failed. err=%d(%s)\n", errno, strerror(errno));
		close(sd);
		return eSIMCTRL_ERROR;
	}
	rv = setsockopt(sd, SOL_SOCKET, SO_SNDBUF, (int*)&val, sizeof(val));
	if( rv < 0 ) {
		printf("setsockopt() failed. err=%d(%s)\n", errno, strerror(errno));
		close(sd);
		return eSIMCTRL_ERROR;
	}

	lin.l_onoff = 1;
	lin.l_linger = 0;
	rv = setsockopt(sd, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin));
	if( rv < 0 ) {
		printf("setsockopt() failed. err=%d(%s)\n", errno, strerror(errno));
		close(sd);
		return eSIMCTRL_ERROR;
	}

	printf("Connect ip_addr=%s, port=%d\n", 
			inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	rv = connect( sd, (struct sockaddr*)&addr, sizeof(addr) );
	if( rv < 0 ) {
		printf("connect() failed. err=%d(%s)\n", errno, strerror(errno));
		close(sd);
		return eSIMCTRL_ERROR;
	}

	return sd;
}

/** @internal mmcd���� tcp channel���� ������ �����Ͱ� �ִ��� Ȯ���Ѵ�. 
              ������ ������ eSIMCTRL_SUCCESS, ������ eSIMCTRL_NOT_FOUND, �ܴ̿� ���� */
static int _simctrl_rmi_watch_msg( simctrl_rmi_t *rmi )
{
	int rv, max_fds;
	struct timeval timeout;
	fd_set readfds;

	// check read buffer
	FD_ZERO(&readfds);
	FD_SET(rmi->sd, &readfds);
	max_fds = rmi->sd + 1;

	timeout.tv_sec = rmi->timeout.tv_sec; 
	timeout.tv_usec= rmi->timeout.tv_usec; 

	rv = select( max_fds, &readfds, 0, 0, &timeout );
	if( rv < 0 ) {
		if( errno == EINTR ) return eSIMCTRL_NOT_FOUND;
		else {
			printf("select() failed3. err = %d(%s)\n", errno, strerror(errno) );
			return eSIMCTRL_ERROR;
		}
	} else if( rv == 0 ) {
		return eSIMCTRL_NOT_FOUND;
	}
	if( !FD_ISSET(rmi->sd, &readfds) ) {
		return eSIMCTRL_NOT_FOUND;
	}
	return eSIMCTRL_SUCCESS; 
}

/** @internal mmcd tcp channel���� �޽����� �����ϰ�, network ordering(ntoh) �Ѵ�. */
static int _simctrl_rmi_read_msg( simctrl_rmi_t *rmi, _simctrl_rmi_msg_t *msg )
{
	int rv, len, rlen, eagain_cnt; 
	char *rbuf;
	_simctrl_rmi_msghdr_t *msghdr = &msg->header;
	_simctrl_rmi_mmlhdr_t *mmlhdr = (_simctrl_rmi_mmlhdr_t*)msg->data;

	rv = fcntl(rmi->sd, F_GETFL, 0);
	if( rv < 0 ) {
		printf("fcntl error F_GETFL val %X\n", rv);
	} else {
		if( fcntl(rmi->sd, F_SETFL, rv | O_NONBLOCK ) < 0 ) {
			printf("fcntl error F_SETFL val %X\n", rv | O_NONBLOCK);
		}
	}

	// read message from mmcd
	len = sizeof(_simctrl_rmi_msghdr_t);
	rlen = eagain_cnt = 0;
	rbuf = (char*)msg;

	// step 1. read message header
	while( rlen < len ) {
		rv = read( rmi->sd, rbuf, len-rlen );
		if( rv < 0 ) {
			if( errno == EAGAIN ) {
				eagain_cnt += 1;
				if( eagain_cnt == 5 ) {
					printf("read() failed. too many eagain... len=%d, rlen=%d\n", len, rlen);
					simctrl_rmi_close(rmi);
					return eSIMCTRL_ERROR;
				}
				usleep( 1000 * eagain_cnt );
				continue;
			} else {
				printf("read() failed. err=%d(%s)\n", errno, strerror(errno));
				simctrl_rmi_close(rmi);
				return eSIMCTRL_ERROR;
			}
		}
		rlen += rv;
		rbuf += rv;
	}
	len = ntohl(msghdr->msgLen);
	if( len >= sizeof(_simctrl_rmi_mml_t) || len < 0 ) {
		printf("Invalid msgLen... len=%d, expected len range is %d ~ %lu.\n",
				len, 0, sizeof(_simctrl_rmi_mml_t));
		simctrl_rmi_close(rmi);
	}
	// setp 2. read message body
	eagain_cnt = 0;
	while( rlen < len ) {
		rv = read( rmi->sd, rbuf, len-rlen );
		if( rv < 0 ) {
			if( errno == EAGAIN ) {
				eagain_cnt += 1;
				if( eagain_cnt == 10 ) {
					printf("read() failed. too many eagain... len=%d, rlen=%d\n", len, rlen);
					simctrl_rmi_close(rmi);
					return eSIMCTRL_ERROR;
				}
				usleep( 1000 * eagain_cnt );
				continue;
			} else {
				printf("read() failed. err=%d(%s)\n", errno, strerror(errno));
				simctrl_rmi_close(rmi);
				return eSIMCTRL_ERROR;
			}
		}
		rlen += rv;
		rbuf += rv;
	}
	rbuf[0] = '\0';
	//printf("read message success. ip=%s, port=%d, length=%d\n", rmi->host, rmi->port, rlen);

	// network oedering (msg header)
	msghdr = &msg->header;
	msghdr->msgLen = ntohl(msghdr->msgLen);
	msghdr->tId = ntohl(msghdr->tId);
	msghdr->tType = ntohs(msghdr->tType);
	msghdr->comId = ntohs(msghdr->comId);
	msghdr->dataLen = ntohl(msghdr->dataLen);

	// network ordering (mml header)
	if( len > sizeof(_simctrl_rmi_msghdr_t) ) { // data�� ������~
		mmlhdr = (_simctrl_rmi_mmlhdr_t*)msg->data;
		mmlhdr->msgLen = ntohs(mmlhdr->msgLen);
		mmlhdr->srcProc = ntohs(mmlhdr->srcProc);
		mmlhdr->clientPid = ntohl(mmlhdr->clientPid);
		mmlhdr->clientFd = ntohl(mmlhdr->clientFd);
		mmlhdr->tid = ntohs(mmlhdr->tid);
		mmlhdr->result = ntohs(mmlhdr->result);
	}

	return eSIMCTRL_SUCCESS;
}

/** @internal network ordering(hton) ó�� �� mmcd tcp channel�� �޽��� �����Ѵ�. */
static int _simctrl_rmi_send_msg( simctrl_rmi_t *rmi, _simctrl_rmi_msg_t *msg )
{
	int rv, len, wlen, eagain_cnt;
	char *wbuf;
	_simctrl_rmi_msghdr_t *msghdr;
	_simctrl_rmi_mmlhdr_t *mmlhdr;

	len = msg->header.msgLen; 

	// network ordering (msg header)
	msghdr = &msg->header;
	msghdr->msgLen = htonl(msghdr->msgLen);
	msghdr->tId = htonl(msghdr->tId);
	msghdr->tType = htons(msghdr->tType);
	msghdr->comId = htons(msghdr->comId);
	msghdr->dataLen = htonl(msghdr->dataLen);

	// network ordering (mml header)
	if( len > sizeof(_simctrl_rmi_msghdr_t) ) { // data�� ������~
		mmlhdr = (_simctrl_rmi_mmlhdr_t*)msg->data;
		mmlhdr->msgLen = htons(mmlhdr->msgLen);
		mmlhdr->srcProc = htons(mmlhdr->srcProc);
		mmlhdr->clientPid = htonl(mmlhdr->clientPid);
		mmlhdr->clientFd = htonl(mmlhdr->clientFd);
		mmlhdr->tid = htons(mmlhdr->tid);
		mmlhdr->result = htons(mmlhdr->result);
	}

	// write message to mmcd
	wlen = eagain_cnt = 0;
	wbuf = (char*)msg;
	while( wlen < len ) {
		rv = write( rmi->sd, wbuf, (size_t)len-wlen);
		if( rv <= 0 ) {
			if( errno == EAGAIN ) {
				eagain_cnt += 1;
				if( eagain_cnt == 5 ) {
					printf("write() failed. too many eagain... len=%d, wlen=%d\n", len, wlen);
					simctrl_rmi_close(rmi);	
					return eSIMCTRL_ERROR;
				}
				continue; 
			} else {
				printf("write() failed. err=%d(%s)\n", errno, strerror(errno));
				simctrl_rmi_close(rmi);	
				return eSIMCTRL_ERROR;
			}
		}
		wlen += rv;
		wbuf += rv;
	}

	//printf("send message success. ip=%s, port=%d, length=%d\n", rmi->host, rmi->port, wlen);
	return eSIMCTRL_SUCCESS;
}

/** @internal login �޽����� �����Ѵ�. id/pwd�� ���������� �ʴ´�. */
static int _simctrl_rmi_send_login( simctrl_rmi_t *rmi ) 
{
	int rv, cnt = 0;
	_simctrl_rmi_msg_t msg;

	memset(&msg.header, 0, sizeof(msg.header));
	msg.header.comId = _RMI_COMID_CONNECT_SCHEDSVR;
	msg.header.dataLen = 0;
	msg.header.msgLen = sizeof(msg.header);

	rv = _simctrl_rmi_send_msg( rmi, &msg );
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	while( (rv = simctrl_rmi_recv_result(rmi, 0)) != eSIMCTRL_SUCCESS ) {
		if( rv == eSIMCTRL_SUCC_CONTINUE || rv == eSIMCTRL_MSG_CONTINUE ) {
			cnt = 0;
			continue;
		} else if( rv == eSIMCTRL_NOT_FOUND ) {
			cnt += 1;
			if( cnt >= 30 ) {
				//printf("Too many receive rmi connect result...\n");
				//rv = eSIMCTRL_ERROR;
				//goto final;
				rmi->login_result_flag = 1; // omsib ������ ���� ������ ���� ���� �� ���� 
				break;
			}
			usleep(10000);
		} else return eSIMCTRL_ERROR;
	}
	return eSIMCTRL_SUCCESS;
}

/** @internal mmc ���ɾ� ó�� ��û �޽����� �����ϰ� �����Ѵ�. */
static int _simctrl_rmi_send_request( simctrl_rmi_t *rmi, char *cmd )
{
	_simctrl_rmi_msg_t msg;
	_simctrl_rmi_mml_t *mml = (_simctrl_rmi_mml_t*)msg.data;

	// set mml command
	if( strlen(cmd) >= sizeof(mml->data) ) {
		printf("Too long rmi command. cmd=%s, buffer size=%lu\n", cmd, sizeof(mml->data));
		return eSIMCTRL_INVALID_PARAM;
	}
	strcpy(mml->data, cmd);

	// set mml header
	memset(&mml->header, 0, sizeof(mml->header));
	mml->header.srcProc = _RMI_REQ_TYPE_TEXT;
	strcpy(mml->header.userName, "admin");
	mml->header.msgLen = strlen(mml->data) + sizeof(mml->header);

	// set rmi msg header
	memset(&msg.header, 0, sizeof(msg.header));
	msg.header.comId = _RMI_COMID_MMC_CMD;
	msg.header.dataLen = mml->header.msgLen;
	msg.header.msgLen = msg.header.dataLen + sizeof(msg.header);

	return _simctrl_rmi_send_msg( rmi, &msg );
}
