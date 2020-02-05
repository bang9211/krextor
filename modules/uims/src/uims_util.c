#include "uims/uims_util.h"

int uims_util_get_aor( usip_uri_t *uri, char *buffer, int bufsize)
{
	int rv;
	
	if( uri == NULL) return UX_ENOENT;

	rv = snprintf(buffer, bufsize, "%s%s%s%s%s", (uri->scheme) ? uri->scheme : "",
					(uri->scheme) ? ":" : "", (uri->user) ? uri->user : "", 
					(uri->user && uri->host) ? "@" : "", (uri->host) ? uri->host : "");
	if( rv <= 0) {
		return strlen((uri->scheme) ? uri->scheme : "") +
			strlen((uri->scheme) ? ":" : "") +
			strlen((uri->user) ? uri->user : "") +
			strlen((uri->user && uri->host) ? "@" : "") +
			strlen((uri->host) ? uri->host : "");
	}
	return rv;
}

usip_via_hdr_t *uims_util_get_ue_via( usip_mobj_t *mobj)
{
	usip_via_hdr_t *via;

	// return top-most via for request
	if( mobj->request) return mobj->via;
	
	// return bottom-most via for response
	via = mobj->via;
	while( via) {
		if( (usip_via_hdr_t*)via->base->next == NULL) break;
		via = (usip_via_hdr_t*)via->base->next;
	}
	return via;
}

int uims_util_get_tport_str( char *protocol, char *buf, int nbuf)
{
	int n;
	char *s;

	if( protocol == NULL) {
		n = strlcpy( buf, "udp", nbuf);
		return n;
	}

	s = strrchr( protocol, '/');
	if( s) s = s+1;
	else s = protocol;
	
	n = strlcpy( buf, s, nbuf);
	return n;
}

int uims_util_sprint_str( char *buffer, int size, char *title, char *value, int dflt_len)
{
	int rv;

	rv = snprintf( buffer, ( size > 0)? size:0, title, value? value:"");
	if( rv < 0) rv = dflt_len + ( value? strlen( value):0);

	return rv;
}

int uims_util_sprint_int( char *buffer, int size, char *title, int value, int dflt_len)
{
	int rv;
	char temp[64];

	rv = snprintf( buffer, ( size > 0)? size:0, title, value);
	if( rv < 0) {
		sprintf( temp, "%d", value);
		rv = dflt_len + strlen( temp);
	}

	return rv;
}

int uims_util_sprint_u64( char *buffer, int size, char *title, uint64_t value, int dflt_len)
{
	int rv;
	char temp[64];

	rv = snprintf( buffer, ( size > 0)? size:0, title, value);
	if( rv < 0) {
		sprintf( temp, "%lld", (unsigned long long)value);
		rv = dflt_len + strlen( temp);
	}

	return rv;
}

int uims_util_sprint_ttl( char *buffer, int size, char *title)
{
	int rv;

	rv = snprintf( buffer, ( size > 0)? size:0, "%s", title);
	if( rv < 0) rv = strlen( title);

	return rv;
}

int uims_util_sprint_ctt( char *buffer, int size, char *title, usip_contact_hdr_t *hdr, int dflt_len)
{
	int rv;
	char temp[1024];

	if( hdr) rv = usip_contact_encode( hdr, temp, sizeof( temp), 0);
	else rv = 0;

	return uims_util_sprint_str( buffer, size, title, ( rv > 0)? temp : NULL, dflt_len);
}

int uims_util_sprint_hdr( char *buffer, int size, char *title, usip_hdr_t *hdr, int dflt_len)
{
	int rv;
	char temp[2048];

	if( hdr) rv = usip_hdr_encode( hdr, temp, sizeof( temp), (USIP_HFLAG_NO_NAME | USIP_HFLAG_NO_CRLF));
	else rv = 0;
	
	return uims_util_sprint_str( buffer, size, title, ( rv > 0)? temp : NULL, dflt_len);
}

int uims_util_encode_rroutes( usip_route_hdr_t *rroutes, char *buffer, int buflen, int flags)
{
	int rv, len;
	usip_route_hdr_t *src;

	if( !rroutes) return 0;

	len = 0;
	src = rroutes;
	
	while( src) {
		rv = usip_route_encode( src, buffer+len, buflen-len, flags);
		if( rv < 0) return rv;

		len += rv;
		src = (usip_route_hdr_t*)src->base->next;
	}

	return len;
}
