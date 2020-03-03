#include "tcp_msg.h"
 
////////////////////////////////////////////////////////////////////////////////////
// functions for tcp_msg_t
////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief network dbif 메시지의 파라미터값들을 network byte ordering으로 바꾼다.
 * @param msg network DBIF message
 * @return 실행 결과 
 */
int tcp_msg_cvt_order_hton(tcp_msg_t *msg)
{
	if (msg == NULL)
		return -1;

#if !UX_BIGENDIAN
	int len, i=1;
	uxc_dbif_t *dbif;
	uxc_ixpc_t *header;

	dbif = (uxc_dbif_t *) msg->data;
	header = (uxc_ixpc_t *)&msg->header;

	len = uxc_dbif_get_length(dbif);

	/* convert data to network ordering */
	for( i = 0; i < dbif->nPara; i++) {

		switch (dbif->type[i] & 0xf) {
			case UXC_IMTYPE_INT :
				dbif->para[i].value = htonl(dbif->para[i].value);
				break;
			case UXC_IMTYPE_STRING :
			case UXC_IMTYPE_OCTET:
				dbif->para[i].data.offset = htons(dbif->para[i].data.offset);
				dbif->para[i].data.size = htons(dbif->para[i].data.size);
				break;
			default :
				ux_log( UXL_INFO, "Unknown Para[%d] Type %d\n", i, dbif->type[i]);
				break;
		}
	}

	/* data convert */
	dbif->dataLen = htons(dbif->dataLen);
	dbif->nPara = htons(dbif->nPara);

	/* ipcmsg header convert */
	header->srcSubSysId = htons(header->srcSubSysId);
	header->srcProcId= htons(header->srcProcId);
	header->dstSubSysId= htons(header->dstSubSysId);
	header->dstProcId= htons(header->dstProcId);
	header->srcQid= htonl(header->srcQid);
	header->dstQid= htonl(header->dstQid);
	header->msgId= htonl(header->msgId);
	header->cmdId= htonl(header->cmdId);
	header->userData= htonl(header->userData);
	header->fdIdx= htonl(header->fdIdx);
	header->length= htons(header->length);
	header->result= htons(header->result);
#endif

	return UX_SUCCESS;
}


/**
 * @brief network dbif 메시지의 파라미터값들을 host byte ordering으로 바꾼다.
 * @param msg network DBIF message
 * @return 실행 결과
 */
int tcp_msg_cvt_order_ntoh(tcp_msg_t *msg)
{
#if !UX_BIGENDIAN
	int i;
	uxc_dbif_t * dbif;
	uxc_ixpc_t * header;

	if (msg == NULL) return -1;

	dbif = (uxc_dbif_t *) msg->data;
	header = (uxc_ixpc_t *) &msg->header;

	dbif->dataLen = ntohs( dbif->dataLen);
	dbif->nPara = ntohs( dbif->nPara);

	for( i = 0; i < dbif->nPara; i++) {
		if( (dbif->type[i] & 0xf) == UXC_IMTYPE_INT) {
			dbif->para[i].value = ntohl(dbif->para[i].value);
		} else if( (dbif->type[i] & 0xf) == UXC_IMTYPE_STRING ||
				  (dbif->type[i] & 0xf) == UXC_IMTYPE_OCTET) 
		{
			dbif->para[i].data.offset = ntohs(dbif->para[i].data.offset);
			dbif->para[i].data.size = ntohs(dbif->para[i].data.size);
		} else {
			ux_log( UXL_INFO, "Unknown Para[%d] Type %d\n", i, dbif->type[i]);
		}
	}

	/* ipcmsg header convert */
	header->srcSubSysId = ntohs(header->srcSubSysId);
	header->srcProcId= ntohs(header->srcProcId);
	header->dstSubSysId= ntohs(header->dstSubSysId);
	header->dstProcId= ntohs(header->dstProcId);
	header->srcQid= ntohl(header->srcQid);
	header->dstQid= ntohl(header->dstQid);
	header->msgId= ntohl(header->msgId);
	header->cmdId= ntohl(header->cmdId);
	header->userData= ntohl(header->userData);
	header->fdIdx= ntohl(header->fdIdx);
	header->length= ntohs(header->length);
	header->result= ntohs(header->result);
#endif

	return UX_SUCCESS;
}


/**
 * @brief uxcutor 에서 받은 tcp_msg 로부터 dbif msg 정보를 출력한다.
 * @param msg tcp_msg message
 * @return 실행 결과 
 */
int tcp_msg_display( tcp_msg_t *msg)
{

	int rv;
	char data [1024];

	uxc_dbif_t *dbif;
	dbif = (uxc_dbif_t *)msg->data;

	ux_log(UXL_CRT, "	[msgid] %d ", msg->header.msgId);

	rv = uxc_dbif_sprint(dbif, "", data, 1024);
	ux_log(UXL_INFO, "	%s", data);

	return UX_SUCCESS;
}


/**
 * @brief encoding 한 ipcmsg 를 uxcutor 로 전송한다. 
 * @param msg ipcmsg, message
 * @param tcp tcp pa
 * @param peerkey peer key
 * @return 실행 결과 
 */
int tcp_msg_send( tcp_msg_t *msg, upa_tcp_t *tcp, upa_peerkey_t *peerkey)
{
	ux_log( UXL_INFO, "= msg_send_tcpmsg =");
	int rv, msg_size;
		
	msg_size = sizeof (uxc_ixpc_t) + msg->header.length; 

	rv = tcp_msg_cvt_order_hton(msg);
	if( rv < UX_SUCCESS) {
		ux_log(UXL_INFO, "msg data error");
		return rv;
	}

	rv = upa_tcp_send2(tcp, peerkey, msg, msg_size, 1);
	if ( rv < UX_SUCCESS) {
		ux_log( UXL_CRT, "can't send data.");
		return -1;
	} else {
		ux_log( UXL_INFO, "= sent data. length = %d\n ", msg_size);
		return eUXC_SUCCESS;
	}
}
