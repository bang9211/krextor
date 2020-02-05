#include "uims/uims_sipmsg.h"

/**
 * @brief 해당 credential(신용장)의 domain이 맞는지 체크한다.
 * @param hdr authorization header
 * @param realm domain realm
 * @return 실행 결과
 * @retval UX_SUCCESS 확인 됨
 * @retval UX_ENOENT 정보가 없음
 * @retval UX_EPERM 권한이 없음 - 매칭 되지 않음
 */
UX_DECLARE(int) uims_sipmsg_check_credential( usip_authorization_hdr_t *hdr,
						const char *realm)
{
	int len;
	const char *pval;

	if( realm && realm[0]) {
		pval = hdr->params ? usip_params_get_value( hdr->params, "realm") : NULL;
		if( pval == NULL || pval[0] == '\0') return UX_ENOENT;
		if( pval[0] == '"') {
			len = strlen(realm);
			if( strncasecmp( pval+1, realm, len) != 0) return UX_EPERM;
			if( pval[len+1] != '"' ) return UX_EPERM;
		} else {
			if( strcasecmp( pval+1, realm) != 0) return UX_EPERM;
		}
	}

	return UX_SUCCESS;
}

UX_DECLARE(int) uims_sipmsg_read_impu_from_str( const char *str, char *buffer, int bufsize)
{
	int n;
	const char *s;

	n = strcspn(str, "<:");
	if( str[n] == '\0') return UX_EINVAL;

	s = str+n;
	if( *s == ':') {
		if( s == str ) return UX_EINVAL;
		s = s-1;
		while( s > str) { 
			if( !usip_is_uri_scheme(s[-1])) break;
			s--;
		}
	} else {
		s++;
	}
 	while( *s ) {
		if( usip_is_lws(*s) ) s++;
	}
	
	n = strcspn(s, ";>&?");

	if( bufsize <= n) return n;

	memcpy( buffer, s, n);
	buffer[n] = '\0';

	return n;
}

UX_DECLARE(char*) uims_sipmsg_get_impu_from_str( usip_mobj_t *mobj, const char *str,
						char *buffer, int bufsize, int *err)
{
	int rv, size;
	char *value;

	rv = uims_sipmsg_read_impu_from_str( str, buffer, bufsize);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		return NULL;
	}
	if( rv < bufsize) return buffer;

	size = rv+1;
	value = ux_alloc( usip_mobj_get_allocator(mobj), size);
	if( value == NULL) {
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	rv = uims_sipmsg_read_impu_from_str( str, value, size);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		return NULL;
	}
	if( rv >= size) {
		if(err) *err = UX_ENOBUFS;
		return NULL;
	}

	return value;
}



UX_DECLARE(int) uims_sipmsg_read_impu( usip_uri_t *uri, char *buffer, int bufsize)
{
	int rv;

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

UX_DECLARE(char*) uims_sipmsg_get_impu( usip_mobj_t *mobj, usip_uri_t *uri, char *buffer,
						int bufsize, int *err)
{
	int rv, size;
	char *value;

	rv = uims_sipmsg_read_impu( uri, buffer, bufsize);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		return NULL;
	}
	if( rv < bufsize) return buffer;

	size = rv+1;
	value = ux_alloc( usip_mobj_get_allocator(mobj), size);
	if( value == NULL) {
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	rv = uims_sipmsg_read_impu( uri, value, size);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		return NULL;
	}
	if( rv >= size) {
		if(err) *err = UX_ENOBUFS;
		return NULL;
	}

	return value;
}

UX_DECLARE(int) uims_sipmsg_read_impi_from_to( usip_mobj_t *mobj, char *buffer, int bufsize)
{
	int rv;
	usip_uri_t *uri;

	if( mobj->to == NULL) return UX_ENOENT;

	uri = mobj->to->uri;
	rv = snprintf(buffer, bufsize, "%s%s%s", (uri->user) ? uri->user : "", 
					(uri->user && uri->host) ? "@" : "", (uri->host) ? uri->host : "");
	if( rv <= 0) {
		return strlen((uri->user) ? uri->user : "") +
			strlen((uri->user && uri->host) ? "@" : "") +
			strlen((uri->host) ? uri->host : "");
	}

	return rv;
}

UX_DECLARE(int) uims_sipmsg_read_impi( usip_mobj_t *mobj, const char *realm,
						char *buffer, int bufsize)
{
	int rv, len;
	const char *username;

	if( mobj->authorization == NULL) return UX_ENOENT; 

	rv = uims_sipmsg_check_credential( mobj->authorization, realm);
	if( rv < UX_SUCCESS) return rv;

	if (mobj->authorization->params) {
		username = usip_params_get_value( mobj->authorization->params, "username");
	} else {
		username = NULL;
	}
	if( username == NULL || username[0] == '\0') return UX_ENOENT;

	if( username[0] == '"') {
		username++;
		len = strlen(username);
		if(len > 1 && username[len-1] == '"') {
			len--;
			rv = snprintf(buffer, bufsize, "%*.*s", len, len, username);
			if(rv <= 0) return len;
			return rv;
		}
		return UX_EINVAL;
	}

	rv = snprintf(buffer, bufsize, "%s", username); 
	if( rv <= 0) return strlen(username);
	return rv;
}

UX_DECLARE(char*) uims_sipmsg_get_impi( usip_mobj_t *mobj, const char *realm,
						char *buffer, int bufsize, int *err)
{
	int rv, size;
	char *value;

	rv = uims_sipmsg_read_impi( mobj, realm, buffer, bufsize);
	if( rv < UX_SUCCESS) {
		if( rv == UX_EINVAL) {
			if(err) *err = rv;
			return NULL;
		}
		rv = uims_sipmsg_read_impi_from_to( mobj, buffer, bufsize);
		if( rv < UX_SUCCESS) {
			if(err) *err = rv;
			return NULL;
		}
		if( rv < bufsize) return buffer;
		size = rv+1;
		value = ux_alloc( usip_mobj_get_allocator(mobj), size);
		if( value == NULL) {
			if(err) *err = UX_ENOMEM;
			return NULL;
		}
		rv = uims_sipmsg_read_impi_from_to( mobj, value, size);
		if( rv < UX_SUCCESS) {
			if(err) *err = rv;
			return NULL;
		}
		if( rv >= size) {
			if(err) *err = UX_ENOBUFS;
			return NULL;
		}
		return value;
	}
		
	if( rv < bufsize) return buffer;

	size = rv+1;
	value = ux_alloc( usip_mobj_get_allocator(mobj), size);
	if( value == NULL) {
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	rv = uims_sipmsg_read_impi( mobj, realm, value, size);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		return NULL;
	}
	if( rv >= size) {
		if(err) *err = UX_ENOBUFS;
		return NULL;
	}

	return value;
}

UX_DECLARE(char*) uims_sipmsg_get_visited_network_id( usip_mobj_t *mobj)
{
	usip_unknown_t *hdr;

	hdr = (usip_unknown_t*)usip_msg_get_hdr_n( mobj->base->msg, "P-Visited-Network-Id", 0);
	if( hdr == NULL) return NULL;

	return hdr->value;
}

UX_DECLARE(uint64_t) uims_sipmsg_get_expires( usip_mobj_t *mobj)
{
	int i;
	uint64_t expires;
	const char *value;
	usip_contact_hdr_t *contact;

	if( mobj->expires) {
		return mobj->expires->delta;
	}

	contact = mobj->contact;
	for( i = 0; contact && i < 1024; ++i) {
		if( contact->params) {
			value = usip_params_get_value( contact->params, "expires");
			expires = (value && value[0]) ? strtoul( value, NULL, 10) : 0;
			if( expires) return expires;
		}
		contact = (usip_contact_hdr_t*)contact->base->next;
	}

	return 0;
}

UX_DECLARE(char*) uims_sipmsg_get_hdrstr( usip_hdr_t *hdr, char *buffer,
						int bufsize, ux_mem_t *allocator, int *err)
{
	int rv, size;
	char *value;

	rv = usip_hdr_encode( hdr, buffer, bufsize, USIP_HFLAG_NO_NAME | USIP_HFLAG_NO_CRLF);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		return NULL;
	}
	if( rv < bufsize) {
		return buffer;
	}
		
	size = rv+1;
	value = ux_alloc( allocator, size);
	if( value == NULL) {
		if(err) *err = UX_ENOMEM;
		return NULL;
	}

	rv = usip_hdr_encode( hdr, value, size, USIP_HFLAG_NO_NAME | USIP_HFLAG_NO_CRLF);
	if( rv < UX_SUCCESS) {
		if(err) *err = rv;
		ux_free( allocator, value);
		return NULL;
	}
	if( rv >= size) {
		if(err) *err = UX_ENOBUFS;
		ux_free( allocator, value);
		return NULL;
	}

	return value;
}

/**
 * @brief 해당 route 정보를 메시지 record-route에 설정한다. 
 * @param mobj record route를 설정할 message object 
 * @param route route header 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) uims_sipmsg_set_record_route( usip_mobj_t *mobj, usip_route_hdr_t *route)
{
	int rv;
	usip_route_hdr_t *head, *dst, **rr;

	head = NULL;
	for( rr = &head; route; route = (usip_route_hdr_t*)route->base->next) {
	    dst = (usip_route_hdr_t*)usip_hdr_create_c(usip_mobj_get_allocator(mobj), usip_record_route_hdef(), route->base);
        if( dst == NULL) {
			ux_log(UXL_CRT, "Failed to create Record-Route header");
            return UX_ENOMEM;
		}
		*rr = dst;
		rr = (usip_route_hdr_t**)&dst->base->next;
	}
	if( head ) {
		rv = usip_msg_add_hdr( mobj->base->msg, (usip_hdr_t*)head, USIP_HFLAG_APPEND);
		if( rv < UX_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set Record-Route header. (err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}
	return USIP_SUCCESS;
}

/**
 * @brief 해당 via 정보를 메시지에 설정한다. 
 * @param mobj via를 설정할 message object 
 * @param via via header 
 * @return 실행 결과
 */
UX_DECLARE(ux_status_t) uims_sipmsg_set_via( usip_mobj_t *mobj, usip_via_hdr_t *via)
{
	int rv;
	usip_via_hdr_t *head, *dst, **v;

	head = NULL;
	for( v = &head; via; via = (usip_via_hdr_t*)via->base->next) {
	    dst = (usip_via_hdr_t*)usip_hdr_create_c(usip_mobj_get_allocator(mobj), usip_via_hdef(), via->base);
        if( dst == NULL) {
			ux_log(UXL_CRT, "Failed to create Record-Route header");
            return UX_ENOMEM;
		}
		*v = dst;
		v = (usip_via_hdr_t**)&dst->base->next;
	}
	if( head ) {
		rv = usip_msg_add_hdr( mobj->base->msg, (usip_hdr_t*)head, USIP_HFLAG_APPEND);
		if( rv < UX_SUCCESS) {
			ux_log(UXL_MAJ, "Failed to set Record-Route header. (err=%d,%s)", rv, usip_errstr(rv));
			return rv;
		}
	}
	return USIP_SUCCESS;
}


