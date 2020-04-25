
#include "ipmssim_msg.h"


static void _ipmssim_msg_display_header( ipmssim_msg_t *msg );
static void _ipmssim_msg_display_usgrep( ipmssim_msg_t *msg );
static void _ipmssim_msg_display_balrep( ipmssim_msg_t *msg );


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

void ipmssim_msg_cvt_order_ntoh( ipmssim_msg_t *msg )
{
	ipmssim_msg_hdr_t *header;

	if( !msg ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p", msg );
		return ;
	}

	header = &msg->header;
	header->version = ntohl( header->version );
	header->msgid = ntohl( header->msgid );
	header->dummy = ntohl( header->dummy );
	header->time = ntohll( header->time );
	header->usgtype = ntohl( header->usgtype );
	header->result = ntohl( header->result );
	header->reserved = ntohl( header->reserved );
	header->length = ntohl( header->length );
}

void ipmssim_msg_cvt_order_hton( ipmssim_msg_t *msg )
{
	ipmssim_msg_hdr_t *header;

	if( !msg ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p", msg );
		return ;
	}

	header = &msg->header;
	header->version = htonl( header->version );
	header->msgid = htonl( header->msgid );
	header->dummy = htonl( header->dummy );
	header->time = htonll( header->time );
	header->usgtype = htonl( header->usgtype );
	header->result = htonl( header->result );
	header->reserved = htonl( header->reserved );
	header->length = htonl( header->length );
}


int ipmssim_msg_enc_header( ipmssim_msg_t *msg, char *trcid, unsigned int msgid, time_t timeval, 
		unsigned int usgtype, unsigned int result, unsigned int reserved, unsigned int length )
{
	ipmssim_msg_hdr_t *header;

	if( !msg || !trcid ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p, trcid=%p", msg, trcid );
		return -1;
	}

	header = &msg->header;
	header->version = IPMSSIM_VERSION;
	if( strlen(IPMSSIM_SYSID) >= sizeof(header->sysid) ) {
		strncpy( header->sysid, IPMSSIM_SYSID, sizeof(header->sysid) );
	} else {
		sprintf( header->sysid, "%-*s", (int)sizeof(header->sysid), IPMSSIM_SYSID );
	}
	if( strlen(trcid) >= sizeof(header->trcid) ) {
		strncpy( header->trcid, trcid, sizeof(header->trcid) );
	} else {
		sprintf( header->trcid, "%-*s", (int)sizeof(header->trcid), trcid );
	}
	header->msgid = msgid;
	header->dummy = 0;
	header->time = timeval ? timeval : time(0);
	header->usgtype = usgtype;
	header->result = result;
	header->reserved = reserved;
	header->length = length;

	return 0;
}

int ipmssim_msg_enc_status( ipmssim_msg_t *msg, char *trcid, unsigned int usgtype, 
		unsigned int length )
{
	if( !msg || !trcid ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p, trcid=%p", msg, trcid );
		return -1;
	}
	memset( msg->data, 0, length );
	return ipmssim_msg_enc_header( msg, trcid, IPMSSIM_MSGID_STS_INQ, 0/*time*/,
			usgtype, eIPMSSIM_NO_ERROR, 0/*reserved*/, length );
}

int ipmssim_msg_enc_status_ack( ipmssim_msg_t *msg, ipmssim_msg_t *rcv, unsigned int result )
{
	if( !msg || !rcv ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p, rcv=%p", msg, rcv );
		return -1;
	}
	return ipmssim_msg_enc_header( msg, rcv->header.trcid, IPMSSIM_MSGID_STS_INQ_ACK, 0/*time*/, 
			rcv->header.usgtype, result, rcv->header.reserved, 0/*length*/ );
}

int ipmssim_msg_enc_usgrep_ack( ipmssim_msg_t *msg, ipmssim_msg_t *rcv, unsigned int result )
{
	if( !msg || !rcv ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p, rcv=%p", msg, rcv );
		return -1;
	}
	return ipmssim_msg_enc_header( msg, rcv->header.trcid, IPMSSIM_MSGID_USG_REP_ACK, 0/*time*/, 
			rcv->header.usgtype, result, rcv->header.reserved, 0/*length*/ );
}

int ipmssim_msg_enc_balrep( ipmssim_msg_t *msg, char *trcid, unsigned int usgtype, 
		unsigned int length, const char *data )
{
	if( !msg || !trcid || !length || !data ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p, trcid=%p, length=%u, data=%p", 
				msg, trcid, length, data );
		return -1;
	}
	if( length >= IPMSSIM_DATA_SIZE ) {
		uaLog( L_MAJ, "Too large balance report data. data length=%d, max=%d",
				length, IPMSSIM_DATA_SIZE );
		return -1;
	}
	memcpy( msg->data, data, length );
	msg->data[length] = '\0';
	return ipmssim_msg_enc_header( msg, trcid, IPMSSIM_MSGID_BAL_REP, 0/*time*/,
			usgtype, eIPMSSIM_NO_ERROR, 0/*reserved*/, length );
}

int ipmssim_msg_enc_balrep_info( ipmssim_msg_t *msg, char *trcid, unsigned int usgtype, 
		ipmssim_msg_balrep_info_t *info )
{
	int i, j, wlen = 0, length = IPMSSIM_DATA_SIZE;
	char *buf, val[64];

	if( !msg || !trcid || !length || !info ) {
		uaLog( L_MAJ, "Invalid parameters. msg=%p, trcid=%p, length=%u, info=%p", 
				msg, trcid, length, info );
		return -1;
	}

	buf = (char*)msg->data;
	
	wlen += snprintf( buf+wlen, (length > wlen) ? length-wlen : 0, 
			"<msgbody month='%s'>\n", info->month );

	for( i = 0 ; i < info->nuser ; i ++ ) {
		wlen += snprintf( buf+wlen, (length > wlen) ? length-wlen : 0, 
				"  <svcMgmtID id='%s' mdn='%s' svc='%s'>\n",
				info->users[i].id, info->users[i].mdn, info->users[i].svc );

		for( j = 0 ; j < info->users[i].ncounter ; j ++ ) {
			ipmssim_cvt_number_to_str( info->users[i].counters[j].value, 
					info->users[i].counters[j].scale, val, sizeof(val) );
			wlen += snprintf( buf+wlen, (length > wlen) ? length-wlen : 0, 
					"    <changeItem fieldname='%s' value='%s' type='int'>\n",
					info->users[i].counters[j].name, val );
			if( info->users[i].counters[j].pps_item_flag ) {
				ipmssim_cvt_number_to_str( info->users[i].counters[j].pps_item.chrg_amt, 
						info->users[i].counters[j].pps_item.scale, val, sizeof(val) );
				wlen += snprintf( buf+wlen, (length > wlen) ? length-wlen : 0,
						"      <ppsItem chargeTime='%s' cdrType='%02d' chargeAmt='%s'>\n"
						"      </ppsItem>\n",
						info->users[i].counters[j].pps_item.chrg_time,
						info->users[i].counters[j].pps_item.cdr_type, val );
			}
			wlen += snprintf( buf+wlen, (length > wlen) ? length-wlen : 0, "    </changeItem>\n");
		} // end of inner for loop

		wlen += snprintf( buf+wlen, (length > wlen) ? length-wlen : 0, "  </svcMgmtID>\n" );
	} // end of outer for loop

	wlen += snprintf( buf+wlen, length > wlen ? length-wlen : 0, "</msgbody>\n" );

	return ipmssim_msg_enc_header( msg, trcid, IPMSSIM_MSGID_BAL_REP, 0/*time*/,
			usgtype, eIPMSSIM_NO_ERROR, 0/*reserved*/, wlen );
}


char* ipmssim_msg_dec_header_sysid( ipmssim_msg_t *msg, char *sysid, size_t len )
{
	int i;

	if( !msg || !sysid || len <= IPMSSIM_SYSID_SIZE ) {
		if( sysid ) sysid[0] = '\0';
		return NULL;
	}

	strncpy( sysid, msg->header.sysid, IPMSSIM_SYSID_SIZE );
	sysid[IPMSSIM_SYSID_SIZE] = '\0';
	for( i = IPMSSIM_SYSID_SIZE-1 ; i >= 0 ; i-- ) if( sysid[i] <= ' ' ) sysid[i] = '\0';
	return sysid; 
}

char* ipmssim_msg_dec_header_trcid( skb_msg_t *msg, char *trcid, size_t len )
{
	if( !msg || !trcid || len <= IPMSSIM_TRCID_SIZE ) {
		if( trcid ) trcid[0] = '\0';
		return NULL;
	}

	// strncpy( trcid, msg->header.trcid, IPMSSIM_TRCID_SIZE );
	trcid[IPMSSIM_TRCID_SIZE] = '\0';
	return trcid;
}

char* ipmssim_msg_dec_header_time( skb_msg_t *msg, const char *fmt, char *timestr, size_t len )
{
	// time_t now = time(0);

	if( !msg || !timestr || !len ) {
		if( timestr ) timestr[0] = '\0';
		return NULL;
	}

	// if( msg->header.time > now - 86400 && msg->header.time < now + 86400 ) {
	// 	ipmssim_cvt_time_to_str( msg->header.time, 1, 
	// 			(!fmt || !fmt[0]) ? "%Y/%m/%d %H:%M:%S" : fmt, timestr, len );
	// } else {
	// 	snprintf( timestr, len, "%-19lu", msg->header.time );
	// }
	return timestr;
}


void ipmssim_msg_display( ipmssim_msg_t *msg )
{
	if( !msg ) return ;

	_ipmssim_msg_display_header( msg );

	switch( msg->header.msgid ) {
		case IPMSSIM_MSGID_STS_INQ: break; // body ����
		case IPMSSIM_MSGID_STS_INQ_ACK: break; // body ����
		case IPMSSIM_MSGID_USG_REP: _ipmssim_msg_display_usgrep( msg ); break;
		case IPMSSIM_MSGID_USG_REP_ACK: break; // body ����
		case IPMSSIM_MSGID_BAL_REP: _ipmssim_msg_display_balrep( msg ); break;
		case IPMSSIM_MSGID_BAL_REP_ACK: break; // body ����
		default: break;
	}
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

static void _ipmssim_msg_display_header( ipmssim_msg_t *msg )
{
	// char linestr[128], timestr[64];
	// char sysid[IPMSSIM_SYSID_SIZE+8], trcid[IPMSSIM_TRCID_SIZE+8];
	// ipmssim_msg_hdr_t *header = &msg->header;

	// uaLog( L_INFO, "%s", ipmssim_make_line_str("Message Header", '=', linestr, 64) );
	// uaLog( L_INFO, " %-*s = 0x%08x", 20, "Version", header->version ); 
	// uaLog( L_INFO, " %-*s = %s", 20, "System ID", 
	// 		ipmssim_msg_dec_header_sysid(msg, sysid, sizeof(sysid)) );
	// uaLog( L_INFO, " %-*s = %s", 20, "Transaction ID", 
	// 		ipmssim_msg_dec_header_trcid(msg, trcid, sizeof(trcid)) );
	// uaLog( L_INFO, " %-*s = %d (%s)", 20, "Message ID", 
	// 		header->msgid, ipmssim_msgid_str(header->msgid) );
	// uaLog( L_DBG, " %-*s = 0x%08x", 20, "Dummy", header->dummy );
	// uaLog( L_INFO, " %-*s = %s", 20, "Time", 
	// 		ipmssim_msg_dec_header_time(msg, "", timestr, sizeof(timestr)) );
	// if( header->msgid == IPMSSIM_MSGID_USG_REP || header->msgid == IPMSSIM_MSGID_USG_REP_ACK ) {
	// 	uaLog( L_INFO, " %-*s = %d (%s)", 20, "Usage type", 
	// 			header->usgtype, ipmssim_usgtype_str(header->usgtype) );
	// } else {
	// 	uaLog( L_DBG, " %-*s = %d", 20, "Usage type", header->usgtype );
	// }
	// if( header->msgid == IPMSSIM_MSGID_STS_INQ_ACK || header->msgid == IPMSSIM_MSGID_USG_REP_ACK 
	// 		|| header->msgid == IPMSSIM_MSGID_BAL_REP_ACK ) {
	// 	uaLog( L_INFO, " %-*s = %d (%s)", 20, "Result code", 
	// 			header->result, ipmssim_result_str(header->result) );
	// } else {
	// 	uaLog( L_DBG, " %-*s = %d", 20, "Result code", header->result );
	// }
	// uaLog( L_DBG, " %-*s = %d", 20, "Reserved", header->reserved );
	// uaLog( L_INFO, " %-*s = %d", 20, "Message length", header->length );
	// uaLog( L_INFO, "%s", ipmssim_make_line_str("", '=', linestr, 64) );
}

static void _ipmssim_msg_display_usgrep( ipmssim_msg_t *msg )
{
	char linestr[128], decode[IPMSSIM_DATA_SIZE*2+8], *body;

	if( msg->header.usgtype == IPMSSIM_USGTYPE_SMS ||
			ipmssim_usgtype_is_ltedata(msg->header.usgtype) ) {
		body = (char*)msg->data;	
	} else {
		ipmssim_encode_base64( (const char*)msg->data, msg->header.length, decode, sizeof(decode) );
		body = decode;
	}

	uaLog( L_INFO, "%s", ipmssim_make_line_str("Message Body (USG_REP)", '=', linestr, 64) );
	uaLog( L_INFO, "Body = %s", body );
	uaLog( L_INFO, "%s", ipmssim_make_line_str("", '=', linestr, 64) );
}

static void _ipmssim_msg_display_balrep( ipmssim_msg_t *msg )
{
	char linestr[128];

	uaLog( L_INFO, "%s", ipmssim_make_line_str("Message Body (BAL_REP)", '=', linestr, 64) );
	uaLog( L_INFO, "Body = \n%s", msg->data );
	uaLog( L_INFO, "%s", ipmssim_make_line_str("", '=', linestr, 64) );
}