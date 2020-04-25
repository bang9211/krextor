#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/signal.h>
#include <sys/time.h>
#include "simctrl_props.h"
#include "simctrl_report.h"
#include "simctrl_rmi.h"
#include "simctrl_scen.h"
#include "simctrl_util.h"


static int _simctrl_scenmgr_load_conf( simctrl_scenmgr_t *mgr, simctrl_props_t *props );
static int _simctrl_scenmgr_load_class( simctrl_scenmgr_t *mgr, simctrl_propclass_t *class);
static int _simctrl_scenmgr_report_cmp( char *a, char *b );
static void _simctrl_scenmgr_indexing( simctrl_scenmgr_t *mgr );
static int _simctrl_scengrp_compare(const void *a, const void *b);
static int _simctrl_scen_compare(const void *a, const void *b);

static void _simctrl_scen_display_title( simctrl_scen_t *scen );
static void _simctrl_scen_display_tail( simctrl_scen_t *scen, int result);
static void _simctrl_scen_display_result( simctrl_scen_t *scen, int result);
static int _simctrl_scen_sprintf_result( simctrl_scen_t *scen, int result, char *buf, int buflen);

static int _simctrl_scencmd_start( simctrl_scencmd_t *cmd, simctrl_scenconf_t *conf);
static void _simctrl_scencmd_read( simctrl_scencmd_t *cmd, simctrl_scenconf_t *conf, int thrflag);
static void _simctrl_scencmd_stop( simctrl_scencmd_t *cmd, simctrl_scenconf_t *conf );
static int _simctrl_scencmd_report( simctrl_scencmd_t *cmd, 
		simctrl_scenconf_t *conf, char *result, int reslen );

static void _simctrl_scen_clear_break_signal();
static void _simctrl_scen_start_break_signal( int sec );
static void _simctrl_scen_stop_break_signal();
static void _simctrl_scen_break();
static int _simctrl_scen_is_break();

static void _simctrl_scen_color_init();
static int _simctrl_scen_color_get();
static void _simctrl_scen_color_set( FILE *fp, int color );
static void _simctrl_scen_color_clear( FILE *fp );


//////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - scenario manager 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief simulator controller scenario manager instance 
 * @return scenario manager instance
 */
simctrl_scenmgr_t* simctrl_scenmgr_instance()
{
	static simctrl_scenmgr_t _singleton;
	return &_singleton;
}

/**
 * @brief simulator controller scenario manager load
 * @param mgr scenario manager instance
 * @param fname xml format property file name
 * @return result code
 * @see simctrl_scenmgr_unload()
 */
int simctrl_scenmgr_load( simctrl_scenmgr_t *mgr, char *fname )
{
	int rv, i, cnt;
	simctrl_props_t props;
	simctrl_propclass_t class;
	simctrl_report_chk_f func = NULL;

	if( !mgr || !fname ) return eSIMCTRL_INVALID_PARAM;
	if( mgr->load_flag ) return eSIMCTRL_ALREADY_EXIST;

	memset(mgr, 0, sizeof(simctrl_scenmgr_t));

	// properties를 초기화한다.
	rv = simctrl_props_open(&props, fname);
	if( rv != eSIMCTRL_SUCCESS ) return rv;

	// scenario configuration을 조회하여 scenario manager에 등록한다.
	rv = _simctrl_scenmgr_load_conf(mgr, &props);
	if( rv < eSIMCTRL_SUCCESS ) {
		simctrl_props_close(&props);
		return rv;
	}

	// scenario class를 조회하여 scenario manager에 등록한다.
	cnt = simctrl_props_get_class_count(&props);
	for( i = 0 ; i < cnt ; i ++ ) {
		rv = simctrl_props_get_class(&props, i, &class);
		if( rv == eSIMCTRL_SUCCESS ) {
			rv = _simctrl_scenmgr_load_class(mgr, &class);
		}
		if( rv < eSIMCTRL_SUCCESS ) {
			simctrl_props_close(&props);
			return rv;
		}
	}

	// report plugin이 등록된 경우 dlopen하고 report check callback 등록한다.
	if( mgr->conf.report_plugin[0] ) {
		mgr->dll = dlopen( mgr->conf.report_plugin, RTLD_LAZY );
		//mgr->dll = dlopen( mgr->conf.report_plugin, RTLD_NOW | RTLD_GLOBAL );
		if( !mgr->dll ) {
			printf( "dlopen() failed. err=%d(%s), dll=%s\n", 
					errno, strerror(errno), mgr->conf.report_plugin);
			simctrl_props_close(&props);
			return eSIMCTRL_ERROR;
		} 
		func = dlsym( mgr->dll, "simctrl_report_compare_function");
		if( !func ) {
			printf( "dlsym() failed. err=%d(%s), dll=%s, func=%s\n", 
					errno, strerror(errno), mgr->conf.report_plugin, 
					"simctrl_report_compare_function");
			dlclose(mgr->dll);
			mgr->dll = NULL;
			simctrl_props_close(&props);
			return eSIMCTRL_ERROR;
		}
		simctrl_report_set_chk_callback( func );
	}

	// report line compare callback function을 등록해준다.
	simctrl_report_set_cmp_callback( _simctrl_scenmgr_report_cmp );

	mgr->load_flag = 1;
	simctrl_props_close(&props);
	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller scenario manager unload 
 * @param mgr scenario manager instance
 * @param gname group name (if null then unload all group)
 * @param name scenario name (if null then unload all scenario in group) 
 * @return result code
 * @see simctrl_scenmgr_load()
 */
int simctrl_scenmgr_unload( simctrl_scenmgr_t *mgr, char *gname, char *name )
{
	int rv;

	if( !mgr ) return eSIMCTRL_INVALID_PARAM;

	if( !gname ) { // scenario class 전체 삭제한다.
		while( mgr->ngrp > 0 ) {
			rv = simctrl_scenmgr_del_grp(mgr, mgr->grps[mgr->ngrp-1].name);
			if( rv < eSIMCTRL_SUCCESS ) return rv;
		}
	} else {
		if( !name ) { // scenario class를 삭제한다.
			rv = simctrl_scenmgr_del_grp(mgr, gname);
			if( rv < eSIMCTRL_SUCCESS ) return rv;
		} else { // scenario class의 특정 scenario만 삭제한다.
			rv = simctrl_scenmgr_del_scen(mgr, gname, name);
			if( rv < eSIMCTRL_SUCCESS ) return rv;
		}
	}

	// group이 하나도 없다면 load flag를 초기화하여 다른 시나리오 로딩할 수 있도록 한다.
	if( mgr->ngrp == 0 ) {
		mgr->load_flag = 0;
		if( mgr->dll ) {
			dlclose(mgr->dll);
			mgr->dll = NULL;
		}
	}
	return eSIMCTRL_SUCCESS;
}


/**
 * @brief simulator controller scneario manager에 group을 추가한다.
 * @param mgr scenario manager instance
 * @param grp scenario group instance
 * @return result code
 * @see simctrl_scenmgr_del_grp(), simctrl_scenmgr_get_grp()
 */
int simctrl_scenmgr_add_grp( simctrl_scenmgr_t *mgr, simctrl_scengrp_t *grp )
{
	if( !mgr || !grp ) return eSIMCTRL_INVALID_PARAM;
	if( mgr->ngrp >= MAX_SIMCTRL_GROUP ) return eSIMCTRL_NO_MEMORY;
	if( simctrl_scenmgr_get_grp( mgr, grp->name) ) return eSIMCTRL_ALREADY_EXIST;

	// add group
	grp->conf = &mgr->conf;
	memcpy(&mgr->grps[mgr->ngrp], grp, sizeof(simctrl_scengrp_t));
	mgr->ngrp += 1;
	qsort(&mgr->grps, mgr->ngrp, sizeof(simctrl_scengrp_t), _simctrl_scengrp_compare);

	// re-indexing scenario
	_simctrl_scenmgr_indexing(mgr);

	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller scneario manager에 group을 삭제한다.
 * @param mgr scenario manager instance
 * @param name scenario group name
 * @return result code
 * @see simctrl_scenmgr_add_grp(), simctrl_scenmgr_get_grp()
 */
int simctrl_scenmgr_del_grp( simctrl_scenmgr_t *mgr, char *name )
{
	simctrl_scengrp_t *grp;

	if( !mgr || !name ) return eSIMCTRL_INVALID_PARAM;

	// get group
	grp = simctrl_scenmgr_get_grp( mgr, name);
	if( !grp ) return eSIMCTRL_NOT_FOUND;

	// remove group
	memcpy(grp, &mgr->grps[mgr->ngrp - 1], sizeof(simctrl_scengrp_t));
	memset(&mgr->grps[mgr->ngrp - 1], 0, sizeof(simctrl_scengrp_t));
	mgr->ngrp -= 1;
	if( mgr->ngrp < 0 ) mgr->ngrp = 0;
	qsort(&mgr->grps, mgr->ngrp, sizeof(simctrl_scengrp_t), _simctrl_scengrp_compare);

	// re-indexing scenario
	_simctrl_scenmgr_indexing(mgr);

	return eSIMCTRL_SUCCESS; 
}

/**
 * @brief simulator controller scneario manager에서 특정 group을 조회한다. 
 * @param mgr scenario manager instance
 * @param name scenario group name
 * @return scenario group instance 
 * @see simctrl_scenmgr_get_grp_i(), simctrl_scenmgr_add_grp(), simctrl_scenmgr_del_grp()
 */
simctrl_scengrp_t* simctrl_scenmgr_get_grp( simctrl_scenmgr_t *mgr, char *name )
{
	if( !mgr || !name ) return NULL;
	return (bsearch(name, &mgr->grps, mgr->ngrp, sizeof(simctrl_scengrp_t), 
				_simctrl_scengrp_compare));
}

/**
 * @brief simulator controller scneario manager에서 특정 group을 조회한다. 
 * @param mgr scenario manager instance
 * @param idx scenario group index 
 * @return scenario group instance 
 * @see simctrl_scenmgr_get_grp(), simctrl_scenmgr_add_grp(), simctrl_scenmgr_del_grp()
 */
simctrl_scengrp_t* simctrl_scenmgr_get_grp_i( simctrl_scenmgr_t *mgr, int idx )
{
	if( !mgr || idx < 0 || idx >= mgr->ngrp ) return NULL;
	return &(mgr->grps[idx]);
}


/**
 * @brief simulator controller scneario manager에 scenario를 추가한다.
 * @param mgr scenario manager instance
 * @param gname scenraio group name
 * @param scen scenario instance
 * @return result code
 * @see simctrl_scenmgr_del_scen(), simctrl_scenmgr_get_scen()
 * @see simctrl_scengrp_add_scen()
 */
int simctrl_scenmgr_add_scen( simctrl_scenmgr_t *mgr, char *gname, simctrl_scen_t *scen )
{
	int rv;
	simctrl_scengrp_t *grp, tmpgrp;

	if( !mgr || !gname || !scen ) return eSIMCTRL_INVALID_PARAM;

	grp = simctrl_scenmgr_get_grp( mgr, gname);
	if( !grp ) { // group이 없으면 생성해준다.
		if(strlen(gname) >= sizeof(grp->name) ) return eSIMCTRL_BUF_OVERFLOW;
		grp = &tmpgrp;
		memset(grp, 0, sizeof(simctrl_scengrp_t));
		strcpy(grp->name, gname);

		rv = simctrl_scenmgr_add_grp( mgr, grp );
		if( rv < eSIMCTRL_SUCCESS ) return rv;

		grp = simctrl_scenmgr_get_grp( mgr, gname);
		if( !grp ) return eSIMCTRL_NOT_FOUND;
	}

	rv = simctrl_scengrp_add_scen( grp, scen);
	if( rv < eSIMCTRL_SUCCESS ) return rv;

	// re-indexing scenario
	_simctrl_scenmgr_indexing(mgr);

	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller scneario manager에 scenario를 삭제한다.
 * @param mgr scenario manager instance
 * @param gname scenraio group name
 * @param scenraio name
 * @return result code
 * @see simctrl_scenmgr_add_scen(), simctrl_scenmgr_get_scen()
 * @see simctrl_scengrp_del_scen()
 */
int simctrl_scenmgr_del_scen( simctrl_scenmgr_t *mgr, char *gname, char *name ) 
{
	int rv;
	simctrl_scengrp_t *grp;

	if( !mgr || !gname || !name ) return eSIMCTRL_INVALID_PARAM;

	// get group
	grp = simctrl_scenmgr_get_grp( mgr, gname);
	if( !grp ) return eSIMCTRL_NOT_FOUND;

	// remove scenario
	rv = simctrl_scengrp_del_scen( grp, name);
	if( rv < eSIMCTRL_SUCCESS ) return rv;

	if( grp->nscen == 0 ) { // group의 scenario가 없다면 group 삭제해준다.
		rv = simctrl_scenmgr_del_grp( mgr, gname);
		if( rv < eSIMCTRL_SUCCESS ) return rv;
	}
	
	// re-indexing scenario
	_simctrl_scenmgr_indexing(mgr);

	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller scneario manager에서 특정 scenario를 조회한다. 
 * @param mgr scenario manager instance
 * @param gname scenario group name
 * @param name scenario name
 * @return scenario instance 
 * @see simctrl_scenmgr_get_scen_i(), simctrl_scenmgr_add_scen(), simctrl_scenmgr_del_scen()
 * @see simctrl_scengrp_get_scen(), simctrl_scengrp_get_scen_i()
 */
simctrl_scen_t* simctrl_scenmgr_get_scen( simctrl_scenmgr_t *mgr, char *gname, char *name )
{
	simctrl_scengrp_t *grp = simctrl_scenmgr_get_grp(mgr, gname);
	return simctrl_scengrp_get_scen(grp, name);
}

/**
 * @brief simulator controller scneario manager에서 특정 scenario를 조회한다. 
 * @param mgr scenario manager instance
 * @param idx scenario index in scenario manager 
 * @return scenario instance 
 * @see simctrl_scenmgr_get_scen(), simctrl_scenmgr_add_scen(), simctrl_scenmgr_del_scen()
 * @see simctrl_scengrp_get_scen(), simctrl_scengrp_get_scen_i()
 */
simctrl_scen_t* simctrl_scenmgr_get_scen_i( simctrl_scenmgr_t *mgr, int idx )
{
	int i, j;
	simctrl_scengrp_t *grp;

	if( !mgr || idx < 0 ) return NULL;
	for( i = 0 ; i < mgr->ngrp ; i ++ ) {
		if( ((i+1) < mgr->ngrp) && (mgr->grps[i+1].scens[0].idx <= idx) ) continue;
		grp = &mgr->grps[i];
		for( j = 0 ; j < grp->nscen ; j ++ ) {
			if( grp->scens[j].idx == idx ) return &grp->scens[j];
		}
	}
	return NULL;
}


/**
 * @brief simulator controller scenario manager의 정보를 buffer에 기록
 * @param mgr scenario manager instance
 * @param buf buffer
 * @param buflen buffer length
 * @param mode display mode 
 * @return string print size
 * @see simctrl_scenmgr_display(), simctrl_scengrp_sprintf()
 */
int simctrl_scenmgr_sprintf( simctrl_scenmgr_t *mgr, char *buf, int buflen, int mode )
{
	int i, wlen;
	if( buf ) buf[0] = '\0';
	if( !mgr || !buf || buflen <= 0 ) return 0;
	for( wlen = i = 0 ; i < mgr->ngrp ; i ++ ) {
		wlen += simctrl_scengrp_sprintf(&mgr->grps[i], buf+wlen, buflen-wlen, mode);
	}
	return wlen;
}

/**
 * @brief simulator controller scenario manager의 정보를 화면에 출력 
 * @param mgr scenario manager instance
 * @param mode display mode 
 * @see simctrl_scenmgr_sprintf(), simctrl_scengrp_display()
 */
void simctrl_scenmgr_display( simctrl_scenmgr_t *mgr, int mode ) 
{
	int i;
	if( !mgr ) return ;
	for( i = 0 ; i < mgr->ngrp ; i ++ ) {
		simctrl_scengrp_display(&mgr->grps[i], mode);
	}
}

/** 
 * @brief simulator controller scenario 수행 결과를 화면에 출력
 * @param mgr scenario manager instance
 * @param scens checked scenario list. (check - 0:none, 1:success, 2:failed) 
 * @param stime start time value
 */
void simctrl_scenmgr_display_result( simctrl_scenmgr_t *mgr, char *scens, struct timeval st)
{
	int i, cnt, diff, wlen, buflen;
	char buff[1024*1024], fname[SIMCTRL_FILE_SIZE];;
	FILE *wfp = NULL;
	struct timeval etime;
	simctrl_scen_t *scen;

	if( !mgr || !scens ) return ;

	buflen = sizeof(buff);

	if( mgr->conf.file_report_flag ) {
		wlen = strlcpy(fname, "scen-run.report.", sizeof(fname));
		simctrl_cvt_time_to_str(st.tv_sec, "%Y%m%d%H%M%S", fname+wlen, sizeof(fname)-wlen);

		wfp = simctrl_fopen(fname, "w");
		if( !wfp ) {
			printf("File report mode. buf file open failed.\n");
		} else {
			printf("File report mode. report file is '%s'\n", fname);
		}
	}

	for( cnt = i = 0 ; i < mgr->nscen ; i ++ ) {
		if( scens[i] ) cnt += 1; // 테스트한 시나리오의 개수를 집계한다.
		if( scens[i] != 2 ) continue;  // 실패한 시나리오에 대해서만 결과 출력한다.
		scen = simctrl_scenmgr_get_scen_i(mgr, i); 
		if( !scen ) continue;

		_simctrl_scen_display_result( scen, eSIMCTRL_ERROR); 
		if( wfp ) {
			_simctrl_scen_sprintf_result( scen, eSIMCTRL_ERROR, buff, buflen); 
			fputs(buff, wfp);
		}
	}

	gettimeofday(&etime, 0);
	diff = (etime.tv_sec*1000 + etime.tv_usec/1000) - (st.tv_sec*1000 + st.tv_usec/1000);

	printf("\n");
	simctrl_display_line('-');
	printf("Ran %d tests in %d.%03d seconds.\n", cnt, diff / 1000, diff % 1000);
	fflush(stdout);
	if( wfp ) {
		wlen = snprintf(buff, buflen, "\n");
		wlen += simctrl_sprintf_line(buff+wlen, (buflen > wlen) ? buflen-wlen : 0, '-');
		wlen += snprintf(buff+wlen, (buflen > wlen) ? buflen-wlen : 0,
						"Ran %d tests in %d.%03d seconds.\n", cnt, diff / 1000, diff % 1000);
		fputs(buff, wfp);	
	}

	printf("\n");
	simctrl_display_line('*');
	printf(" Simulation result report.\n");
	simctrl_display_line('-');
	fflush(stdout);
	if( wfp ) {
		wlen = snprintf(buff, buflen, "\n");
		wlen += simctrl_sprintf_line(buff+wlen, (buflen > wlen) ? buflen-wlen : 0, '*');
		wlen += snprintf(buff+wlen, (buflen > wlen) ? buflen-wlen : 0,
						" Simulation result report.\n");
		wlen += simctrl_sprintf_line(buff+wlen, (buflen > wlen) ? buflen-wlen : 0, '-');
		fputs(buff, wfp);	
	}

	for( i = 0 ; i < mgr->nscen ; i ++ ) {
		if( scens[i] == 0 ) continue;
		scen = simctrl_scenmgr_get_scen_i(mgr, i); 
		if( !scen ) continue;

		simctrl_scen_display(scen, SIMCTRL_DISPLAY_RESULT);
		printf(" - %s\n", scens[i] == 2 ? "fail" : "ok");
		usleep(1000);
		fflush(stdout);
		if( wfp ) {
			wlen = simctrl_scen_sprintf(scen, buff, buflen, SIMCTRL_DISPLAY_RESULT);
			wlen += snprintf(buff+wlen, (buflen > wlen) ? buflen-wlen : 0,
							" - %s\n", scens[i] == 2 ? "fail" : "ok");
			fputs(buff, wfp);
		}
	}

	simctrl_display_line('*');
	if( wfp ) {
		simctrl_sprintf_line(buff, buflen, '*');
		fputs(buff, wfp);
	}
	if( wfp ) fclose(wfp);

	usleep(10000);
	fflush(stdout);
}


//////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - scenario group 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief simulator controller scneario group에 scneario를 추가한다.
 * @param grp scenario group instance
 * @param scen scenario instance
 * @return result code
 * @see simctrl_scengrp_del_scen(), simctrl_scengrp_get_scen()
 */
int simctrl_scengrp_add_scen( simctrl_scengrp_t *grp, simctrl_scen_t *scen )
{
	if( !grp || !scen ) return eSIMCTRL_INVALID_PARAM;
	if( grp->nscen >= MAX_SIMCTRL_SCENARIO ) return eSIMCTRL_NO_MEMORY;
	if( simctrl_scengrp_get_scen( grp, scen->name) ) return eSIMCTRL_ALREADY_EXIST;

	// add scenario
	scen->grp = grp;
	memcpy(&grp->scens[grp->nscen], scen, sizeof(simctrl_scen_t));
	grp->nscen += 1;
	qsort(&grp->scens, grp->nscen, sizeof(simctrl_scen_t), _simctrl_scen_compare);

	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller scneario group에 scneario를 삭제한다.
 * @param grp scenario group instance
 * @param name scenario name
 * @return result code
 * @see simctrl_scengrp_add_scen(), simctrl_scengrp_get_scen()
 */
int simctrl_scengrp_del_scen( simctrl_scengrp_t *grp, char *name )
{
	simctrl_scen_t *scen;

	if( !grp || !name ) return eSIMCTRL_INVALID_PARAM;

	// get scenario
	scen = simctrl_scengrp_get_scen( grp, name);
	if( !scen ) return eSIMCTRL_NOT_FOUND;

	// remove scenario
	memcpy(scen, &grp->scens[grp->nscen - 1], sizeof(simctrl_scen_t));
	memset(&grp->scens[grp->nscen - 1], 0, sizeof(simctrl_scen_t));
	grp->nscen -= 1;
	if( grp->nscen < 0 ) grp->nscen = 0;
	qsort( &grp->scens, grp->nscen, sizeof(simctrl_scen_t), _simctrl_scen_compare);

	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller scneario group에서 특정 scenario를 조회한다. 
 * @param grp scenario group instance
 * @param name scenario name
 * @return scenario instance 
 * @see simctrl_scengrp_get_scen_i(), simctrl_scengrp_add_scen(), simctrl_scengrp_del_scen()
 */
simctrl_scen_t* simctrl_scengrp_get_scen( simctrl_scengrp_t *grp, char *name )
{
	if( !grp || !name ) return NULL;
	return (bsearch(name, &grp->scens, grp->nscen, sizeof(simctrl_scen_t), 
				_simctrl_scen_compare));
}

/**
 * @brief simulator controller scneario group에서 특정 scenario를 조회한다. 
 * @param grp scenario group instance
 * @param idx scenario index in scenario manager
 * @return scenario instance 
 * @see simctrl_scengrp_get_scen(), simctrl_scengrp_add_scen(), simctrl_scengrp_del_scen()
 */
simctrl_scen_t* simctrl_scengrp_get_scen_i( simctrl_scengrp_t *grp, int idx )
{
	if( !grp || idx < 0 || idx >= grp->nscen ) return NULL;
	return &(grp->scens[idx]);
}


/**
 * @brief simulator controller scenario group의 정보를 buffer에 기록
 * @param grp scenario group instance
 * @param buf buffer
 * @param buflen buffer length
 * @param mode display mode 
 * @return string print size
 * @see simctrl_scengrp_display(), simctrl_scen_sprintf()
 */
int simctrl_scengrp_sprintf( simctrl_scengrp_t *grp, char *buf, int buflen, int mode )
{
	int i, wlen = 0;

	if( buf ) buf[0] = '\0';
	if( !grp || !buf || buflen <= 0 ) return 0;

	if( mode & SIMCTRL_DISPLAY_CMD ) {
		wlen += snprintf(buf, buflen-1, "Class >> %s\n", grp->name);
	}
	for( wlen = i = 0 ; i < grp->nscen ; i ++ ) {
		wlen += simctrl_scen_sprintf(&grp->scens[i], buf+wlen, buflen-wlen, mode);
	}
	return wlen;
}

/**
 * @brief simulator controller scenario group의 정보를 화면에 출력 
 * @param grp scenario group instance
 * @param mode display mode 
 * @see simctrl_scengrp_sprintf(), simctrl_scen_display()
 */
void simctrl_scengrp_display( simctrl_scengrp_t *grp, int mode )
{
	int i;

	if( !grp ) return ;

	if( mode & SIMCTRL_DISPLAY_CMD ) {
		printf("Scenario class >> %s\n", grp->name);
	}
	for( i = 0 ; i < grp->nscen ; i ++ ) {
		simctrl_scen_display(&grp->scens[i], mode);
	}
}


//////////////////////////////////////////////////////////////////////////////
// public functions - scenario 
//////////////////////////////////////////////////////////////////////////////

/**
 * @brief simulator controller scenario의 정보를 buffer에 기록
 * @param scen scenario instance
 * @param buf buffer
 * @param buflen buffer length
 * @param mode display mode 
  @return string print size
 * @see simctrl_scen_display()
 */
int simctrl_scen_sprintf( simctrl_scen_t *scen, char *buf, int buflen, int mode )
{
	int i, wlen = 0;

	if( buf ) buf[0] = '\0';
	if( !scen || !buf || buflen <= 0 ) return 0; 

	if( mode & SIMCTRL_DISPLAY_IDX ) {
		wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, 
					"[%3d] ", scen->idx);
	}
	if( (mode & SIMCTRL_DISPLAY_CLASS) && scen->grp ) {
		wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, 
					"%-16s ", scen->grp->name);
	}
	if( mode & SIMCTRL_DISPLAY_NAME ) {
		wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, 
				"%s%-32s ", (mode & SIMCTRL_DISPLAY_CLASS) ? "> " : "", scen->name);
	}
	if( mode & SIMCTRL_DISPLAY_DESC ) {
		wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, 
				"%s%s", (mode & SIMCTRL_DISPLAY_NAME) ? "| " : "", scen->desc);
	}
	if( (mode & SIMCTRL_DISPLAY_CR) || (mode & SIMCTRL_DISPLAY_RMI) || 
			(mode & SIMCTRL_DISPLAY_CMD) ) {
		wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, "\n");
	}
	if( mode & SIMCTRL_DISPLAY_RMI ) {
		for( i = 0 ; i < scen->nrmi ; i ++ ) {
			wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0,
					"      - rmi #%d\n"
					"        command       = \"%s\"\n"
					"        display       = \"%s\"\n",
					i+1,
					scen->rmis[i].command,
					scen->rmis[i].display_flag ? "true" : "false");
		}
	}
	if( mode & SIMCTRL_DISPLAY_CMD ) {
		for( i = 0 ; i < scen->ncmd ; i ++ ) {
			wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, 
					"      - command #%d\n"
					"        script        = \"%s\"\n"
					"        check         = \"%s\"\n"
					"        check_type    = \"%s\"\n"
					"        display       = \"%s\"\n"
					"        report        = \"%s\"\n"
					"        timeout       = \"%d sec\"\n"
					"        sleep_time    = \"%d sec\"\n"
					"        serial        = \"%s\"\n"
					"        remote_host   = \"%s\"\n"
					"        remote_passwd = \"%s\"\n",
					i+1,
					scen->cmds[i].command, 
					scen->cmds[i].check,
					simctrl_scen_type_str(scen->cmds[i].check_type),
					scen->cmds[i].display_flag ? "true" : "false", 
					scen->cmds[i].report_flag ? "true" : "false",
					scen->cmds[i].timeout, 
					scen->cmds[i].sleep_time, 
					scen->cmds[i].serial_flag ? "true" : "false",
					scen->cmds[i].remote_host,
					scen->cmds[i].remote_passwd);
		}
	}
	return wlen;
}

/**
 * @brief simulator controller scenario의 정보를 화면에 출력 
 * @param scen scenario instance
 * @param mode display mode 
 * @see simctrl_scen_sprintf()
 */
void simctrl_scen_display( simctrl_scen_t *scen, int mode )
{
	int i;

	if( !scen ) return ;

	if( mode & SIMCTRL_DISPLAY_IDX ) printf("[%3d] ", scen->idx);
	if( (mode & SIMCTRL_DISPLAY_CLASS) && scen->grp ) printf("%-16s ", scen->grp->name); 
	if( mode & SIMCTRL_DISPLAY_NAME ) {
		printf("%s%-32s ",  (mode & SIMCTRL_DISPLAY_CLASS) ? "> " : "", scen->name);
	}
	if( mode & SIMCTRL_DISPLAY_DESC ) {
		printf("%s%s", (mode & SIMCTRL_DISPLAY_NAME) ? "| " : "", scen->desc);
	}
	if( (mode & SIMCTRL_DISPLAY_CR) || (mode & SIMCTRL_DISPLAY_RMI) ||
			(mode & SIMCTRL_DISPLAY_CMD) ) printf("\n"); 
	if( mode & SIMCTRL_DISPLAY_RMI ) {
		for( i = 0 ; i < scen->nrmi ; i ++ ) {
			printf( "      - rmi #%d\n"
					"        command       = \"%s\"\n"
					"        display       = \"%s\"\n",
					i+1,
					scen->rmis[i].command,
					scen->rmis[i].display_flag ? "true" : "false");
		}
	}
	if( mode & SIMCTRL_DISPLAY_CMD ) {
		for( i = 0 ; i < scen->ncmd ; i ++ ) {
			printf( "      - command #%d\n"
					"        script        = \"%s\"\n"
					"        check         = \"%s\"\n"
					"        check_type    = \"%s\"\n"
					"        display       = \"%s\"\n"
					"        report        = \"%s\"\n"
					"        timeout       = \"%d sec\"\n"
					"        sleep_time    = \"%d sec\"\n"
					"        serial        = \"%s\"\n"
					"        remote_host   = \"%s\"\n"
					"        remote_passwd = \"%s\"\n",
					i+1,
					scen->cmds[i].command, 
					scen->cmds[i].check,
					simctrl_scen_type_str(scen->cmds[i].check_type),
					scen->cmds[i].display_flag ? "true" : "false", 
					scen->cmds[i].report_flag ? "true" : "false", 
					scen->cmds[i].timeout, 
					scen->cmds[i].sleep_time,
					scen->cmds[i].serial_flag ? "true" : "false",
					scen->cmds[i].remote_host,
					scen->cmds[i].remote_passwd);
		}
	}
	usleep(1000);
	fflush(stdout);
}


/**
 * @brief simulator controller scenario 실행 및 결과 리포트 함수 
 * @param scen scenario instance
 * @return result code
 */
int simctrl_scen_proc( simctrl_scen_t *scen )
{
	int rv, i, j, serial_idx=0, result=0, len=0, type=0;
	simctrl_rmi_t *rmi = simctrl_rmi_instance();

	if( !scen || !scen->ncmd ) {
		printf( "Invalid parameters. scen=%p, scen->ncmd=%d\n",
				scen, scen ? scen->ncmd : 0);
		return eSIMCTRL_INVALID_PARAM;
	}

	fflush(stdin);
	fflush(stdout);
	fflush(stderr);
	_simctrl_scen_clear_break_signal();

	scen->report[0] = '\0';
	_simctrl_scen_display_title( scen );

	// 시나리오의 rmi 명령어를 실행한다.
	rmi->proc_flag = 1; // SCENARIO에서 RMI 실행 중 COMMAND에서 읽지 못하도록 설정
	for( i = 0 ; i < scen->nrmi ; i ++ ) {
		rv = simctrl_rmi_send_request(rmi, scen->rmis[i].command);
		if( rv < eSIMCTRL_SUCCESS ) {
			rmi->proc_flag = 0;
			return rv;
		}

		if( scen->grp->conf->display_flag && scen->rmis[i].display_flag ) type = 2;
		else type = 0;

		for( j = 0 ; j < 50 ; j ++ ) {
			rv = simctrl_rmi_recv_result(rmi, type);
			if( rv == eSIMCTRL_SUCCESS ) break; // 성공케이스. 다음 단계로 이동
			if( rv == eSIMCTRL_SUCC_CONTINUE ) continue; // 성공 & 응답이 더 남아있음
			if( rv == eSIMCTRL_MSG_CONTINUE ) {
				if( type == 2 ) type = 3;
				continue; // 단일 메시지가 나뉘어 전송되고 있는 중, 추가로 더 수신하도록 한다.
			}
			if( rv < eSIMCTRL_SUCCESS && rv != eSIMCTRL_NOT_FOUND ) {
				rmi->proc_flag = 0;
				return rv;
			}
			usleep( 100000 ); // 아직 응답을 못받은 상태이다. 0.1초 쉬어주고 다시 응답 대기
		}
		if( j == 50 ) {
			printf( "Receive rmi command response timeout.\n");
			rmi->proc_flag = 0;
			return eSIMCTRL_ERROR;
		}
	}
	rmi->proc_flag = 0;

	// 시나리오의 command 들을 실행한다.
	_simctrl_scen_color_init();
	for( i = 0 ; i < scen->ncmd ; i ++ ) {
		rv = _simctrl_scencmd_start( &scen->cmds[i], scen->grp->conf );
		if( rv < eSIMCTRL_SUCCESS ) {
			for( j = serial_idx ; j < i ; j ++ ) {
				_simctrl_scencmd_stop( &scen->cmds[j], scen->grp->conf );
				rv = _simctrl_scencmd_report( &scen->cmds[j], scen->grp->conf, NULL, 0 );
			}
			return eSIMCTRL_ERROR;
		}
		if( scen->cmds[i].serial_flag ) { // 이번 command가 끝나야지 다음을 실행할 수 있다.
			for( j = serial_idx ; j <= i ; j ++ ) {
				_simctrl_scencmd_stop( &scen->cmds[j], scen->grp->conf );
				rv = _simctrl_scencmd_report( &scen->cmds[j], scen->grp->conf,
						scen->report+len, sizeof(scen->report)-len);
				len = strlen(scen->report);
				if( scen->cmds[j].report_flag && rv < 0 ) result = rv;
			}
			serial_idx = j;
			_simctrl_scen_color_init();
		}
		// 시나리오가 break된 상태라면 더 진행하지 않고 빠져나가도록 한다.
		if( _simctrl_scen_is_break() ) {
			result = eSIMCTRL_ERROR;
			break;
		}
	}

	// 실행한 시나리오의 command 들의 실행 종료를 기다리고 결과 리포트 기록한다.
	for( i = serial_idx ; i < scen->ncmd ; i ++ ) {
		_simctrl_scencmd_stop( &scen->cmds[i], scen->grp->conf );
		rv = _simctrl_scencmd_report( &scen->cmds[i], scen->grp->conf,
					scen->report+len, sizeof(scen->report)-len);
		len = strlen(scen->report);
		if( scen->cmds[i].report_flag && rv < 0 ) result = rv;
	}
	if( len  >= (sizeof(scen->report) - 8) ) { // max 도달했구나, ... 붙여주자.
		strcpy(scen->report+(len-8), "\n...\n");
	}

	_simctrl_scen_display_tail( scen, result);
	usleep(10000);
	fflush(stdout);
	return result;
}


//////////////////////////////////////////////////////////////////////////////
// private functions
//////////////////////////////////////////////////////////////////////////////

/** @internal properties에서 configuration 정보를 조회하여 scenario manager에 설정 */
static int _simctrl_scenmgr_load_conf( simctrl_scenmgr_t *mgr, simctrl_props_t *props )
{
	int rv, i;
	simctrl_propconf_t conf;
	
	if( !mgr || !props ) return eSIMCTRL_INVALID_PARAM;

	rv = simctrl_props_get_configuration(props, &conf);
	if( rv < eSIMCTRL_SUCCESS ) return rv;

	mgr->conf.display_flag = conf.display_trace_log;
	mgr->conf.realtime_display = conf.realtime_display;
	mgr->conf.file_report_flag = conf.file_report;
	mgr->conf.scen_timeout = conf.scenario_timeout;
	mgr->conf.scen_interval = conf.scenario_interval;
	mgr->conf.scen_type = conf.scenario_type;
	if( !conf.rmi_host ) mgr->conf.rmi_host[0] = '\0';
	else {
		for( i = 0 ; i < conf.define_cnt ; i ++ ) {
			if( strstr(conf.rmi_host, conf.defines[i].name) ) {
				simctrl_replace_str( (char*)conf.rmi_host, 
						(char*)conf.defines[i].name, (char*)conf.defines[i].value,
						mgr->conf.rmi_host, sizeof(mgr->conf.rmi_host) );
				break;
			}
		}
		if( i == conf.define_cnt ) {
			simctrl_strlcpy(mgr->conf.rmi_host, conf.rmi_host, sizeof(mgr->conf.rmi_host));
		}
	}
	if( !conf.rmi_port ) mgr->conf.rmi_port[0] = '\0';
	else { 
		for( i = 0 ; i < conf.define_cnt ; i ++ ) {
			if( strstr(conf.rmi_port, conf.defines[i].name) ) {
				simctrl_replace_str( (char*)conf.rmi_port, 
						(char*)conf.defines[i].name, (char*)conf.defines[i].value,
						mgr->conf.rmi_port, sizeof(mgr->conf.rmi_port) );
				break;
			}
		}
		if( i == conf.define_cnt ) {
			simctrl_strlcpy(mgr->conf.rmi_port, conf.rmi_port, sizeof(mgr->conf.rmi_port));
		}	
	}
	if( !conf.report_plugin ) mgr->conf.report_plugin[0] = '\0';
	else simctrl_strlcpy(mgr->conf.report_plugin, conf.report_plugin, 
			sizeof(mgr->conf.report_plugin)); 
	for( i = 0 ; i < conf.ignore_keyword_cnt ; i ++ ) {
		simctrl_strlcpy(mgr->conf.ignore_keyword[i], conf.ignore_keyword[i], 
				sizeof(mgr->conf.ignore_keyword[i]));
	}
	mgr->conf.ignore_keyword_cnt = conf.ignore_keyword_cnt;
	for( i = 0 ; i < conf.replace_keyword_cnt ; i ++ ) {
		simctrl_strlcpy(mgr->conf.replace_keyword[i], conf.replace_keyword[i],
				sizeof(mgr->conf.replace_keyword[i]));
	}
	mgr->conf.replace_keyword_cnt = conf.replace_keyword_cnt;
	for( i = 0 ; i < conf.define_cnt ; i ++ ) {
		simctrl_strlcpy(mgr->conf.defines[i].name, conf.defines[i].name,
				sizeof(mgr->conf.defines[i].name));
		simctrl_strlcpy(mgr->conf.defines[i].value, conf.defines[i].value,
				sizeof(mgr->conf.defines[i].value));
	}
	mgr->conf.define_cnt = conf.define_cnt;
	return eSIMCTRL_SUCCESS;
}

/** @internal properties에서 class정보를 조회하여 scenario manager에 설정 */
static int _simctrl_scenmgr_load_class( simctrl_scenmgr_t *mgr, simctrl_propclass_t *class)
{
	int rv, i, j, k, cnt_scen, cnt_rmi1, cnt_rmi2, cnt_cmd;
	char buf[2][SIMCTRL_COMMAND_SIZE];
	simctrl_proprmi_t propgrprmi[MAX_SIMCTRL_RMI];
	simctrl_propscen_t propscen;
	simctrl_proprmi_t proprmi;
	simctrl_propcmd_t propcmd;
	simctrl_scen_t scen; 
	simctrl_scenrmi_t *rmi;
	simctrl_scencmd_t *cmd;

	if( !mgr || !class ) return eSIMCTRL_INVALID_PARAM;

	// get rmi group in class
	cnt_rmi1 = simctrl_propclass_get_rmi_count( class );
	if( cnt_rmi1 < 0 ) cnt_rmi1 = 0;
	else if( cnt_rmi1 > MAX_SIMCTRL_RMI ) {
		printf( "Invalid class rmi parameter. class name=%s, rmi count=%d, max=%d.\n",
				class->name, cnt_rmi1, MAX_SIMCTRL_RMI);
		return eSIMCTRL_ERROR;
	}
	for( i = 0 ; i < cnt_rmi1 ; i ++ ) {
		rv = simctrl_propclass_get_rmi( class, i, &propgrprmi[i]);
		if( rv < eSIMCTRL_SUCCESS ) return rv;
	}

	// get scenario group in class
	cnt_scen = simctrl_propclass_get_scenario_count( class );
	if( cnt_scen < 0 ) return eSIMCTRL_INVALID_CONF; 
	for( i = 0 ; i < cnt_scen ; i ++ ) {
		rv = simctrl_propclass_get_scenario( class, i, &propscen);
		if( rv < eSIMCTRL_SUCCESS ) return rv; 

		simctrl_strlcpy(scen.name, propscen.name, sizeof(scen.name));
		simctrl_strlcpy(scen.desc, propscen.desc, sizeof(scen.desc));

		// get rmi group in scenario
		cnt_rmi2 = simctrl_propscen_get_rmi_count(&propscen);
		if( cnt_rmi2 < 0 ) cnt_rmi2 = 0;
		else if( cnt_rmi2 > MAX_SIMCTRL_RMI ) {
			printf( "Invalid scenario rmi parameter. class name=%s, "
					"scenario name=%s, rmi count=%d, max=%d.\n",
					class->name, scen.name, cnt_rmi2, MAX_SIMCTRL_RMI);
			return eSIMCTRL_ERROR;
		}
		scen.nrmi = cnt_rmi1 + cnt_rmi2;

		for( j = 0 ; j < cnt_rmi1 ; j ++ ) {
			rmi = &scen.rmis[j];
			memset(rmi, 0, sizeof(simctrl_scenrmi_t));

			strlcpy( buf[0], propgrprmi[j].command, sizeof(buf[0]));
			for( k = 0 ; k < mgr->conf.define_cnt ; k ++ ) {
				if( strstr(buf[0], mgr->conf.defines[k].name) ) {
					simctrl_replace_str(buf[0], mgr->conf.defines[k].name, 
							mgr->conf.defines[k].value, buf[1], sizeof(buf[1]));
					strlcpy( buf[0], buf[1], sizeof(buf[0]));
				}
			}
			simctrl_strlcpy(rmi->command, buf[0], sizeof(rmi->command));
			rmi->display_flag = propgrprmi[j].display;
		}
		for( j = 0 ; j < cnt_rmi2 ; j ++ ) {
			rv = simctrl_propscen_get_rmi( &propscen, j, &proprmi);
			if( rv < eSIMCTRL_SUCCESS ) return rv;

			rmi = &scen.rmis[j+cnt_rmi1];
			memset(rmi, 0, sizeof(simctrl_scenrmi_t));

			strlcpy( buf[0], proprmi.command, sizeof(buf[0]));
			for( k = 0 ; k < mgr->conf.define_cnt ; k ++ ) {
				if( strstr(buf[0], mgr->conf.defines[k].name) ) {
					simctrl_replace_str(buf[0], mgr->conf.defines[k].name, 
							mgr->conf.defines[k].value, buf[1], sizeof(buf[1]));
					strlcpy( buf[0], buf[1], sizeof(buf[0]));
				}
			}
			simctrl_strlcpy(rmi->command, buf[0], sizeof(rmi->command));
			rmi->display_flag = proprmi.display;
		}

		// get command group in scenario
		cnt_cmd = simctrl_propscen_get_command_count(&propscen);
		if( cnt_cmd == 0 || cnt_cmd > MAX_SIMCTRL_COMMAND ) {
			printf( "Invalid scenario command parameter. class name=%s, "
					"scenario name=%s, command count=%d, max=%d.\n", 
					class->name, scen.name, cnt_cmd, MAX_SIMCTRL_COMMAND);
			return eSIMCTRL_ERROR;
		}
		scen.ncmd = cnt_cmd; 

		for( j = 0 ; j < cnt_cmd ; j ++ ) {
			rv = simctrl_propscen_get_command(&propscen, j, &propcmd);
			if( rv < eSIMCTRL_SUCCESS ) return rv; 

			cmd = &scen.cmds[j];	
			memset(cmd, 0, sizeof(simctrl_scencmd_t));
			
			strlcpy( buf[0], propcmd.script, sizeof(buf[0]));
			for( k = 0 ; k < mgr->conf.define_cnt ; k ++ ) {
				if( strstr(buf[0], mgr->conf.defines[k].name) ) {
					simctrl_replace_str(buf[0], mgr->conf.defines[k].name,
							mgr->conf.defines[k].value, buf[1], sizeof(buf[1]));
					strlcpy( buf[0], buf[1], sizeof(buf[0]));
				}
			}
			simctrl_strlcpy(cmd->command, buf[0], sizeof(cmd->command));
			simctrl_strlcpy(cmd->check, propcmd.check, sizeof(cmd->check));
			cmd->check_type = propcmd.check_type;
			cmd->display_flag = propcmd.display;
			cmd->report_flag = propcmd.report;
			cmd->timeout = propcmd.timeout;
			cmd->sleep_time = propcmd.sleep;
			cmd->serial_flag = propcmd.serial;
			simctrl_strlcpy(cmd->remote_host, propcmd.remote_host, sizeof(cmd->remote_host));
			simctrl_strlcpy(cmd->remote_passwd, propcmd.remote_passwd, sizeof(cmd->remote_passwd));
		}

		// add scenario
		rv = simctrl_scenmgr_add_scen(mgr, (char*)class->name, &scen);
		if( rv < eSIMCTRL_SUCCESS ) {
			printf( "Add scenario failed. class name=%s, scenario name=%s\n", 
					class->name, scen.name);
			return rv;
		}	
	} // end of outer for loop

	return eSIMCTRL_SUCCESS;
}

/** @internal report class에서 expected / received 비교시 사용할 callback function
			  1. replace keyword를 찾아 source를 destination value로 변경한 후 비교 
              2. ignore keyword를 찾아 해당 keyword 이후 내용을 제외하고 비교 */
static int _simctrl_scenmgr_report_cmp( char *a, char *b )
{
	int rv, i, len;
	char *buf = NULL, *src, *dst, *ignore;
	char *org = a;
	simctrl_scenmgr_t *mgr = simctrl_scenmgr_instance();
	simctrl_scenconf_t *conf = &mgr->conf;

	for( i = 0 ; i < conf->replace_keyword_cnt ; i ++ ) {
		dst = strstr(conf->replace_keyword[i], "=>");
		if( !dst ) continue;
		src = conf->replace_keyword[i];
		dst[0] = '\0';
		if( strstr(a, src) ) {
			len = strlen(a) - strlen(src) + strlen(dst+2) + 1;
			buf = malloc(len);

			simctrl_replace_str(a, src, dst+2, buf, len);

			if( a != org ) free(a); // 직전 수행한 buf 메모리를 반환해준다.
			a = buf;
		}
		dst[0] = '=';
	}
	
	for( i = 0 ; i < conf->ignore_keyword_cnt ; i ++ ) {
		ignore = strstr(a, conf->ignore_keyword[i]);
		if( ignore ) return strncmp(a, b, ignore - a + (strlen(conf->ignore_keyword[i])));
	}

	rv = strcmp(a, b);
	if( buf ) free(buf);
	return rv;
}

/** @internal scenario manager에 등록된 scenario를 순서대로 index를 설정해준다. */
static void _simctrl_scenmgr_indexing( simctrl_scenmgr_t *mgr )
{
	int i, j, idx;
	simctrl_scengrp_t *grp;

	if( !mgr ) return ;

	for( idx = i = 0 ; i < mgr->ngrp ; i ++ ) {
		grp = &mgr->grps[i];
		for( j = 0 ; j < grp->nscen ; j ++ ) {
			grp->scens[j].idx = idx;
			grp->scens[j].grp = grp;
			idx += 1;
		}
	}
	mgr->nscen = idx; // 마지막 index를 저장한다.
}


/** @internal scenario group name compare */
static int _simctrl_scengrp_compare(const void *a, const void *b)
{
	simctrl_scengrp_t *aa = (simctrl_scengrp_t*)a;
	simctrl_scengrp_t *bb = (simctrl_scengrp_t*)b;
	return (strcmp(aa->name, bb->name));
}

/** @internal scenario name compare */
static int _simctrl_scen_compare(const void *a, const void *b)
{
	simctrl_scen_t *aa = (simctrl_scen_t*)a;
	simctrl_scen_t *bb = (simctrl_scen_t*)b;
	return (strcmp(aa->name, bb->name));
}


/** @internal display scenario title */
static void _simctrl_scen_display_title( simctrl_scen_t *scen )
{
	printf("\n");
	simctrl_display_line('*');
	printf("TEST CASE : %s > %s (scenario index - %d)\n", 
			scen->grp ? scen->grp->name : "", scen->name, scen->idx);
	simctrl_display_line('-');
	fflush(stdout);
}

/** @internal display scenario tail */
static void _simctrl_scen_display_tail( simctrl_scen_t *scen, int result )
{
	printf("\n");
	printf("result - %s\n", !result ? "ok" : "fail");
	simctrl_display_line('*');
	fflush(stdout);
}

/** @internal display scenario result & error string */
static void _simctrl_scen_display_result( simctrl_scen_t *scen, int result)
{
	printf("\n");
	simctrl_display_line('-');
	printf("%s %s scenario finish.\n", scen->grp ? scen->grp->name : "", scen->name);
	simctrl_display_line('-');
	printf("result - %s\n", !result ? "ok" : "fail");
	if( scen->report[0] ) printf("%s\n", scen->report);
	simctrl_display_line('*');
	fflush(stdout);
}
static int _simctrl_scen_sprintf_result( simctrl_scen_t *scen, int result, char *buf, int buflen)
{
	int wlen = 0;

	if( buf ) buf[0] = '\0';
	if( !scen || !buf || buflen <= 0 ) return 0; 

	wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, "\n");
	wlen += simctrl_sprintf_line(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, '-');
	wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0,
				"%s %s scenario finish.\n", scen->grp ? scen->grp->name : "", scen->name);
	wlen += simctrl_sprintf_line(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, '-');
	wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0,
				"result - %s\n", !result ? "ok" : "fail");
	if( scen->report[0] ) {
		wlen += snprintf(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, "%s\n", scen->report);
	}
	wlen += simctrl_sprintf_line(buf+wlen, (buflen > wlen) ? buflen-wlen : 0, '*');
	return wlen;
}


/** @internal scenario command를 시작한다. 
              fork하고 pipe를 열어 command 실행 및 처리 결과를 제어한다. */
static int _simctrl_scencmd_start( simctrl_scencmd_t *cmd, simctrl_scenconf_t *conf )
{
	int idx = 0, buflen, arg_pos_type = 0;
	char buff[SIMCTRL_COMMAND_SIZE*4];
	char *pos, *arg_pos, *path, *command;

	buflen = sizeof(buff);

	/* sleep_time이 설정되어있다면 명령어 수행전에 sleep 할 수 있도록 설정한다. */
	if( cmd->sleep_time > 0 ) {
		idx += snprintf(buff+idx, (buflen > idx) ? buflen-idx : 0,
				"sleep %d;", cmd->sleep_time);
	}

	/* remote_host가 설정되어있다면 ssh 접속 명령 설정한다. */
	if( cmd->remote_host[0] ) {
		// remote_passwd가 설정되어있다면 expect를 이용해 password 인증해준다. 
		if( cmd->remote_passwd[0] ) { 
			idx += snprintf(buff+idx, (buflen > idx) ? buflen-idx : 0,
					"expect << EOF | grep -v %s\n"
					"spawn -noecho ssh %s \"%s\"\n" 
					"expect {\n"
					"  -re \"incorrect\" { exit -1 }\n"
					"  -re \"No address|usage|route\" { exit -1 }\n"
					"  -re \"Connection refused\" { exit -1 }\n"
					"  -re \"Connection closed\" { exit -1 }\n"
					"  -re \"Unable to connect\" { exit -1 }\n"
					"  -re \"Permission denied\" { exit -1 }\n"
					"  timeout { exit -3 }\n"
					"  -re \"yes/no\" { send \"yes\\r\"; exp_continue }\n"
					"  -re \"assword:\" { send \"%s\\r\" }\n"
					"}\n"
					"expect {\n"
					"  -re \"Permission denied\" { exit -1 }\n"
					"  -re \"assword:\" { exit -1 }\n"
					"  interact\n"
		            "}\n"
					"EOF\n",
					cmd->remote_host, cmd->remote_host, cmd->command, cmd->remote_passwd);
		} else {
			idx += snprintf(buff+idx, (buflen > idx) ? buflen-idx : 0, 
					"ssh %s '%s'", cmd->remote_host, cmd->command);
		}
	} else {
		/* path로 이동 후 command 실행할 수 있도록 명령어를 생성한다. */
		// 1단계 - command의 마지막 '/'를 기준으로 path, command 분리
		// 1.1 command, argument 분리 
		// - 정확하게 분리하지는 못하고 ' or "에 쌓여져 있는것만 분리한다.
		// - 왜냐하면 directory에 공백이 있을 수도 있기에 공백으로만 구분하지는 못한다.
		//arg_pos = strchr(cmd->command, ' ');
		//if( !arg_pos ) arg_pos = strchr(cmd->command, '\t');
		//if( arg_pos ) arg_pos[0] = '\0'; // argument가 있다면 command와 argument 분리
		arg_pos = strchr(cmd->command, '\'');
		if( !arg_pos ) arg_pos = strchr(cmd->command, '\"');
		if( arg_pos ) {
			arg_pos[0] == '\'' ? arg_pos_type = 1 : 2;
			arg_pos[0] = '\0'; // argument가 있다면 command와 argument 분리
		}
		// 1.2 path, command 분리
		pos = strrchr(cmd->command, '/');
		if( pos ) {
			pos[0] = '\0';
			path = cmd->command;
			//pos[0] = '/';
			command = pos + 1;
		} else {
			path = NULL;
			command = cmd->command;
		}
		//if( arg_pos ) arg_pos[0] = ' ';
		if( arg_pos ) arg_pos[0] = arg_pos_type == 1 ? '\'' : '\"';

		// 2단계 - path에 있으면 change directory 명령을 추가
		//         그리고 path에 있는 ' \()[]{}<>'에 '\'를 추가해준다.
		if( path ) {
			idx += snprintf(buff+idx, (buflen > idx) ? buflen-idx : 0, "cd "); 
			while( *path ) {
				if( idx >= buflen - 2 ) break;
				if( *path == ' ' || *path == '\\' || 
						*path == '(' || *path == ')' ||
						*path == '[' || *path == ']' ||
						*path == '{' || *path == '}' ||
						*path == '<' || *path == '>' ) {
					buff[idx++] = '\\';
				}
				buff[idx++] = *path;
				path += 1;
			}
			if( idx < buflen - 2 ) {
				buff[idx++] = ';';
				buff[idx++] = ' ';
			}
			buff[idx] = '\0';
			pos[0] = '/'; // 1단계에서 '\0'으로 변경한 것을 복구시킨다.
		}
		// 3단계 - path와 command를 합쳐 명령어를 완성한다.
		snprintf(buff+idx, (buflen > idx) ? buflen-idx : 0, "%s 2>&1", command);
	}

	/* 자식 프로세스를 생성하고 pipe를 열어 생성한 명령어를 실행시킨다. */
	cmd->pipe_fp = popen(buff, "r");
	if( !cmd->pipe_fp ) {
		printf( "scenario execute failed. cmd=%s, err=%d(%s)\n",
				buff, errno, strerror(errno));
		return eSIMCTRL_ERROR;
	}

	/* 실시간 실행 결과 출력하는 경우 바로 실행 시킨다. */
	if( conf->realtime_display ) _simctrl_scencmd_read( cmd, conf, 1 );

	return eSIMCTRL_SUCCESS;
}

/** @internal scenario의 command가 종료될때 까지 기다린다. 
              command 처리 결과 내용을 버퍼에 저장하고 처리 결과를 리포팅한다. */
typedef struct _simctrl_scencmd_arg_s _simctrl_scencmd_arg_t;
struct _simctrl_scencmd_arg_s {
	simctrl_scencmd_t *cmd; 
	simctrl_scenconf_t *conf;
};

ux_mutex_t _g_read_mutex;
static void _simctrl_scencmd_read_thread_f( void *arg );

static void _simctrl_scencmd_read( simctrl_scencmd_t *cmd, simctrl_scenconf_t *conf, int thrflag )
{
	pthread_t id;
	_simctrl_scencmd_arg_t *arg;	

	arg = malloc(sizeof(_simctrl_scencmd_arg_t));
	arg->cmd = cmd;
	arg->conf = conf;

	if( thrflag ) {
		pthread_create(&id, NULL, (void*(*)(void*))_simctrl_scencmd_read_thread_f, (void*)arg);
	} else {
		_simctrl_scencmd_read_thread_f( (void*)arg );
	}
}
static void _simctrl_scencmd_read_thread_f( void *arg )
{
	int scen_type = SIMCTRL_SCEN_TYPE_NULL, cnt = 0, crflag = 0;
	char buff[1024];
	char fname[SIMCTRL_FILE_SIZE+32] = {0, };
	FILE *wfp = NULL;
	simctrl_scencmd_t *cmd = NULL;
	simctrl_scenconf_t *conf = NULL;
	int color = _simctrl_scen_color_get(); 

	if( !arg ) goto final;
	cmd = ((_simctrl_scencmd_arg_t*)arg)->cmd;
	conf = ((_simctrl_scencmd_arg_t*)arg)->conf;

	// 이미 종료되었다. 그냥 성공처리한다.
	if( !cmd || !cmd->pipe_fp ) goto final;

	// 전체 시나리오가 break 된 상태라면 해당 시나리오를 마무리한다.
	if( _simctrl_scen_is_break() ) goto final;

	// scenario type은 cmd에 설정되어있으면 그걸 사용하고 없으면 config에 것을 사용한다.
	if( cmd->check_type == SIMCTRL_SCEN_TYPE_NULL ) scen_type = conf->scen_type;
	else scen_type = cmd->check_type;

	// check 설정되어있다면 실행 이력을 기록할 파일을 연다.
	if( cmd->check[0] != '\0' ) {
		switch( scen_type ) {
			case SIMCTRL_SCEN_TYPE_SIPP : 
				// sipp는 화면 내용으로 점검하는 것이 아니라 trace_file로 점검한다.
				// 화면 내용을 점검할 파일을 열 필요가 없다.
				snprintf(fname, sizeof(fname)-1, "%s.tmp", cmd->check);
				break; 
			case SIMCTRL_SCEN_TYPE_SEAGULL : 
			case SIMCTRL_SCEN_TYPE_DEFAULT :
			default :
				snprintf(fname, sizeof(fname)-1, "%s.tmp", cmd->check);
				wfp = simctrl_fopen(fname, "w");
				if( !wfp ) { // directory가 없을 경우가 있으므로 mkdir 하고 다시 fopen한다.
					simctrl_mkdir_with_file( fname ); 
					wfp = simctrl_fopen(fname, "w");
				}
				if( !wfp ) {
					printf("file open failed. file=%s, err=%d(%s)\n",
							fname, errno, strerror(errno));
					// 일단 시나리오는 진행하고 나중에 report에서 예외처리하도록~
				}
				break;
		}
	}

	// 커맨드 실행 결과 화면에 출력되는 내용을 pipe에서 읽어 화면/파일에 보낸다.
	_simctrl_scen_start_break_signal( cmd->timeout < 0 ? conf->scen_timeout : cmd->timeout );
	while( !_simctrl_scen_is_break() && fgets(buff, sizeof(buff), cmd->pipe_fp) ) {
		// check carriage return 
		if( (buff[0] == '\n' && strlen(buff) == 1) ) { 
			if( crflag ) continue;
			crflag = 1;
		} else {
			crflag = 0;
		}

		// check escape character
		simctrl_decode_ascii(buff);

		// display mode 이면 화면에 출력한다.
		if( conf->display_flag && cmd->display_flag ) {
			ux_mutex_lock(&_g_read_mutex);
			printf( "\t| ");
			_simctrl_scen_color_set( stdout, color );
			printf( "%s", buff);
			_simctrl_scen_color_clear( stdout );
			ux_mutex_unlock(&_g_read_mutex);
			fflush(stdout);
		}

		// check 설정되어있다면 파일에 저장한다.
		if( wfp ) fputs(buff, wfp);

		cnt += 1;
		if( cnt >= 10000 ) {
			printf( "Too many result received...\n");
			_simctrl_scen_break();
			break;
		}
	}
	_simctrl_scen_stop_break_signal();
	if( wfp ) fclose( wfp );

final:
	// 커맨드 종료를 기다리고 리턴 코드를 전달 받는다.
	if( cmd && cmd->pipe_fp ) {
		fflush(cmd->pipe_fp);
		pclose(cmd->pipe_fp);
		cmd->pipe_fp = NULL;
	}
	// thread fucntion argument를 반환한다.
	if( arg ) free( arg );
}

static void _simctrl_scencmd_stop( simctrl_scencmd_t *cmd, simctrl_scenconf_t *conf )
{
	/* 실시간 실행 결과 출력한 경우는 read thread가 끝날때 까지 join해준다. */
	if( conf->realtime_display ) while( cmd->pipe_fp ) usleep(10);
	/* 순서대로 실행 결과를 출력하는 경우는 pipe read 및 출력한다. */
	else {
		_simctrl_scen_color_init();
		_simctrl_scencmd_read( cmd, conf, 0 );
	}
}

static int _simctrl_scencmd_report( simctrl_scencmd_t *cmd, 
		simctrl_scenconf_t *conf, char *result, int reslen )
{
	int rv = eSIMCTRL_SUCCESS, scen_type = SIMCTRL_SCEN_TYPE_NULL;
	char fname[SIMCTRL_FILE_SIZE+32] = {0, };
	
	// scenario type은 cmd에 설정되어있으면 그걸 사용하고 없으면 config에 것을 사용한다.
	if( cmd->check_type == SIMCTRL_SCEN_TYPE_NULL ) scen_type = conf->scen_type;
	else scen_type = cmd->check_type;

	// check 설정되어있다면 실행이력이 정상인지 확인한다.
	if( cmd->check[0] != '\0' ) {
		snprintf(fname, sizeof(fname)-1, "%s.tmp", cmd->check);

		if( _simctrl_scen_is_break() ) rv = eSIMCTRL_SCEN_TIMEOUT;
		else rv = simctrl_report_check(cmd->check, fname, scen_type, result, reslen);

		simctrl_remove(fname);
	}
			
	// report flag가 설정되어있다면 실행 결과값을 전달한다.
	if( conf->display_flag && cmd->report_flag ) {
		printf("\nscenario command(%s) finish. result=%d\n", cmd->command, rv);
		fflush(stdout);
	}
	return rv;
}


//////////////////////////////////////////////////////////////////////////////
// private functions for scenario timeout handler
//////////////////////////////////////////////////////////////////////////////

static int _g_simctrl_scen_stop_flag = 0;

/** @internal alarm signal handler function */
static void _simctrl_scen_break_signal_handler( int sig )
{
	_simctrl_scen_break();	
	signal( SIGALRM, _simctrl_scen_break_signal_handler);
}

/** @internal clear scenario timeout - alarm signal / flag를 초기화 한다. */
static void _simctrl_scen_clear_break_signal()
{
	_g_simctrl_scen_stop_flag = 0;
	alarm( 0 );
}

/** @internal start scenario timeout - alarm signal에 timeout 시간을 설정한다 */
static void _simctrl_scen_start_break_signal( int sec )
{
	signal( SIGALRM, _simctrl_scen_break_signal_handler);
	alarm( sec );
}

/** @internal stop scenario timeout - alarm signal을 종료한다. */
static void _simctrl_scen_stop_break_signal()
{
	alarm( 0 );
}

/** @internal scenario break 처리한다. popen에 의해 생성된 child pid 를 찾아 kill 한다. */
static void _simctrl_scen_break()
{
	_g_simctrl_scen_stop_flag = 1;
	simctrl_kill( getpid(), 1/*kill child process*/ );
	printf( "scenario break finished.\n");
}

/** @internal get scenario break status */
static int _simctrl_scen_is_break()
{
	return _g_simctrl_scen_stop_flag;
}


//////////////////////////////////////////////////////////////////////////////
// private functions for scenario color handler
//////////////////////////////////////////////////////////////////////////////

#define COLOR_1     "\033[0m"
#define COLOR_2     "\033[31;40m"
#define COLOR_3     "\033[32;40m"
#define COLOR_4     "\033[33;40m"
#define COLOR_5     "\033[34;40m"
#define COLOR_6     "\033[35;40m"
#define COLOR_NC    "\033[0m"

typedef enum _g_simctrl_scen_color_e _g_simctrl_scen_color_e;
enum _g_simctrl_scen_color_e {
	_COLOR_NONE = 0,
	_COLOR_RED,
	_COLOR_GREEN,
	_COLOR_YELLOW,
	_COLOR_BLUE,
	_COLOR_PURPLE,
};

char *_g_simctrl_scen_colors[] = { COLOR_1, COLOR_2, COLOR_3, COLOR_4, COLOR_5, COLOR_6 };
int _g_simctrl_scen_color_max = 6; 
int _g_simctrl_scen_color_idx = 0;

/** @internal init selected color index */
static void _simctrl_scen_color_init()
{
	_g_simctrl_scen_color_idx = 0;
}

/** @internal get color index */
static int _simctrl_scen_color_get()
{
	int color = ++_g_simctrl_scen_color_idx;
	return (color-1) % _g_simctrl_scen_color_max;
}

/** @internal set color to file descriptor */
static void _simctrl_scen_color_set( FILE *fp, int color ) 
{
	if( color < 0 || color >= _g_simctrl_scen_color_max ) return ;
	fprintf( fp, _g_simctrl_scen_colors[color] );
}

/** @internal clear color to file descriptor */
static void _simctrl_scen_color_clear( FILE *fp )
{
	fprintf( fp, COLOR_NC );
}
