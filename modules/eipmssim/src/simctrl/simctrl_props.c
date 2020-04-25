#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "simctrl_props.h"
#include "simctrl_util.h"


//////////////////////////////////////////////////////////////////////////////
// public functions for simulator controller - properties
//////////////////////////////////////////////////////////////////////////////

/** 
 * @brief simulator controller properties open
          xml 형식의 파일을 열어 파싱하고 파싱 내역을 props instance에 설정한다. 
 * @param props properties instance
 * @param fname xml format property file name
 * @return result code
 * @see simctrl_props_close()
 */
int simctrl_props_open( simctrl_props_t *props, char *fname)
{
	int rv;
	char xml[1024];
	struct stat fnstat;

	if( !props || !fname ) {
		printf( "Invalid parameters. props=%p, fname=%p\n", props, fname);
		return eSIMCTRL_INVALID_PARAM;
	}
	
	rv = simctrl_get_conf_path(fname, xml, sizeof(xml));
	if( rv < eSIMCTRL_SUCCESS ) return rv; 

	if( stat(xml, &fnstat) ) {
		printf( "Invalid xml file. fname=%s, err=%d(%s)\n", fname, errno, strerror(errno) );
		return eSIMCTRL_INVALID_PARAM;
	}
	if( S_ISDIR(fnstat.st_mode) ) {
		printf( "omitting directory %s\n", fname );
		return eSIMCTRL_INVALID_PARAM;
	}

	rv = ux_props_init(props, ux_propxml_parser());
	if( rv < UX_SUCCESS ) return eSIMCTRL_INVALID_PARAM;

	rv = ux_props_load(props, xml);
	if( rv < UX_SUCCESS ) return eSIMCTRL_ERROR;

	return eSIMCTRL_SUCCESS;
}

/** 
 * @brief simulator controller properties close 
 * @param props properties instance
 * @see simctrl_props_open()
 */
void simctrl_props_close( simctrl_props_t *props )
{
	if( props ) ux_props_final(props);
}


/**
 * @brief simulator controller properties에서 configuration를 조회하는 함수
 * @param props properties instance
 * @param conf properties group(configuration) instance
 * @return result code 
 */
int simctrl_props_get_configuration( simctrl_props_t *props, simctrl_propconf_t *conf)
{
	int i, count;
	const char *name, *val;
	simctrl_propgrp_t *scenarios, *def;

	if( !props || !conf ) {
		printf( "Invalid parameters. props=%p, conf=%p\n", props, conf); 
		return eSIMCTRL_INVALID_PARAM;
	}

	// init & set default value 
	memset(conf, 0, sizeof(simctrl_propconf_t));
	conf->display_trace_log = 1;
	conf->realtime_display = 0;
	conf->file_report = 0;
	conf->scenario_timeout = 0;
	conf->scenario_interval = 0;
	conf->scenario_type = SIMCTRL_SCEN_TYPE_DEFAULT;
	conf->rmi_host = "";
	conf->rmi_port = "";
	conf->report_plugin = "";
	conf->ignore_keyword_cnt = 0;
	conf->replace_keyword_cnt = 0;
	conf->define_cnt = 0;

	scenarios = ux_props_get_group(props, "scenarios");
	if( !scenarios ) {
		printf( "Not defined scenarios group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	conf->propgrp = ux_propgrp_get_group(scenarios, "configuration");
	if( !conf->propgrp ) {
		printf( "Not defined configuration group in scenarios.\n");
		return eSIMCTRL_INVALID_CONF;
	}
	count = ux_propgrp_get_group_count(conf->propgrp, "define");

	for( i = 0 ; i < count ; i ++ ) {
		def = ux_propgrp_get_groupi(conf->propgrp, "define", i);
		if( !def ) continue;

		name = ux_propgrp_get_str(def, "name", NULL);
		if( !name ) continue;

		if( !strcasecmp(name, "display-trace-log") ) {
			val = ux_propgrp_get_str(def, "value", "on");
			if( !val ) return eSIMCTRL_INVALID_CONF;
			conf->display_trace_log = !strcasecmp(val, "on") ? 1 : 0;
		} else if( !strcasecmp(name, "realtime-display") ) {
			val = ux_propgrp_get_str(def, "value", "off");
			if( !val ) return eSIMCTRL_INVALID_CONF;
			conf->realtime_display = !strcasecmp(val, "on") ? 1 : 0;
		} else if( !strcasecmp(name, "file-report") ) {
			val = ux_propgrp_get_str(def, "value", "off");
			if( !val ) return eSIMCTRL_INVALID_CONF;
			conf->file_report = !strcasecmp(val, "on") ? 1 : 0;
		} else if( !strcasecmp(name, "scenario-timeout") ) {
			conf->scenario_timeout = ux_propgrp_get_int(def, "value", 0);
		} else if( !strcasecmp(name, "scenario-interval") ) {
			conf->scenario_interval = ux_propgrp_get_int(def, "value", 0);
		} else if( !strcasecmp(name, "scenario-type") ) {	
			val = ux_propgrp_get_str(def, "value", "none");
			if( !val ) return eSIMCTRL_INVALID_CONF;
			if( !strcasecmp(val, "seagull") ) {
				conf->scenario_type = SIMCTRL_SCEN_TYPE_SEAGULL;
			} else if( !strcasecmp(val, "sipp") ) {
				conf->scenario_type = SIMCTRL_SCEN_TYPE_SIPP;
			} else if( !strcasecmp(val, "none") ) {
				conf->scenario_type = SIMCTRL_SCEN_TYPE_DEFAULT;
			} else {
				conf->scenario_type = SIMCTRL_SCEN_TYPE_DEFAULT;
			}
		} else if( !strcasecmp(name, "rmi-host") ) {
			conf->rmi_host = ux_propgrp_get_str(def, "value", "");
			if( !conf->rmi_host ) return eSIMCTRL_INVALID_CONF;
		} else if( !strcasecmp(name, "rmi-port") ) {
			conf->rmi_port = ux_propgrp_get_str(def, "value", "");
			if( !conf->rmi_port ) return eSIMCTRL_INVALID_CONF;
		} else if( !strcasecmp(name, "report-plugin") ) {
			conf->report_plugin = ux_propgrp_get_str(def, "value", "");
			if( !conf->report_plugin ) return eSIMCTRL_INVALID_CONF;
		} else if( !strcasecmp(name, "ignore-keyword") ) {
			if( conf->ignore_keyword_cnt >= MAX_SIMCTRL_KEYWORD ) {
				printf("Too many exists ignore-keyword.\n");
				return eSIMCTRL_INVALID_CONF;
			}
			val = ux_propgrp_get_str(def, "value", NULL);
			if( !val ) return eSIMCTRL_INVALID_CONF;
			conf->ignore_keyword[conf->ignore_keyword_cnt] = val;
			conf->ignore_keyword_cnt += 1;
		} else if( !strcasecmp(name, "replace-keyword") ) {
			if( conf->replace_keyword_cnt >= MAX_SIMCTRL_KEYWORD ) {
				printf("Too many exists replace-keyword.\n");
				return eSIMCTRL_INVALID_CONF;
			}
			val = ux_propgrp_get_str(def, "value", NULL);
			if( !val ) return eSIMCTRL_INVALID_CONF;
			conf->replace_keyword[conf->replace_keyword_cnt] = val;
			conf->replace_keyword_cnt += 1;
		} else {
			if( conf->define_cnt >= MAX_SIMCTRL_KEYWORD ) {
				printf("Too many exists define fields.\n");
				return eSIMCTRL_INVALID_CONF;
			}
			val = ux_propgrp_get_str(def, "value", NULL);
			if( !val) return eSIMCTRL_INVALID_CONF;
			conf->defines[conf->define_cnt].name = name;
			conf->defines[conf->define_cnt].value = val;
			conf->define_cnt += 1;
		}
	}
	return eSIMCTRL_SUCCESS;
}

/**
 * @brief simulator controller properties에서 설정된 class 개수 확인 함수 
 * @param props properties instance
 * @return class group count 
 */
int simctrl_props_get_class_count( simctrl_props_t *props )
{
	simctrl_propgrp_t *scenarios;

	if( !props ) {
		printf( "Invalid parameters. props=%p\n", props);
		return eSIMCTRL_INVALID_PARAM;
	}

	scenarios = ux_props_get_group(props, "scenarios");
	if( !scenarios ) {
		printf( "Not defined scenarios group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	return ux_propgrp_get_group_count(scenarios, "class");
}	

/**
 * @brief simulator controller properties에서 index번째 class 조회 함수
 * @param props properties instance
 * @param idx class group index
 * @param class properties group(class) instance
 * @return result code 
 */
int simctrl_props_get_class( simctrl_props_t *props, int idx, simctrl_propclass_t *class)
{
	simctrl_propgrp_t *scenarios;

	if( !props || idx < 0 || !class ) {
		printf( "Invalid parameters. props=%p, idx=%d, class=%p\n", props, idx, class);
		return eSIMCTRL_INVALID_PARAM;
	}

	scenarios = ux_props_get_group(props, "scenarios");
	if( !scenarios ) {
		printf( "Not defined scenarios group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	class->propgrp = ux_propgrp_get_groupi(scenarios, "class", idx);
	if( !class->propgrp ) {
		printf( "Not defined class group in scenarios.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	class->name = ux_propgrp_get_str(class->propgrp, "name", NULL); 
	if( !class->name ) {
		printf( "Not defined name in class.\n");
		return eSIMCTRL_INVALID_CONF;
	}
	return eSIMCTRL_SUCCESS;
}


/**
 * @brief simulator controller class properties에서 설정된 rmi 개수 확인 함수
 * @param class class properties instance
 * @return rmi group count 
 */
int simctrl_propclass_get_rmi_count( simctrl_propclass_t *class )
{
	if( !class ) {
		printf( "Invalid parameters. class=%p\n", class);
		return eSIMCTRL_INVALID_PARAM;
	}

	return ux_propgrp_get_group_count(class->propgrp, "rmi");
}

/**
 * @brief simulator controller class properties에서 index번째 rmi 조회하 함수
 * @param class class properties instance
 * @param idx rmi group index
 * @param rmi properties group(rmi) instance
 * @return result code 
 */
int simctrl_propclass_get_rmi( simctrl_propclass_t *class, int idx, simctrl_proprmi_t *rmi)
{
	const char *val;

	if( !class || idx < 0 || !rmi ) {
		printf( "Invalid parameters. class=%p, idx=%d, rmi=%p\n", class, idx, rmi);
		return eSIMCTRL_INVALID_PARAM;
	}

	rmi->propgrp = ux_propgrp_get_groupi(class->propgrp, "rmi", idx);
	if( !rmi->propgrp ) {
		printf( "Not defined rmi group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	rmi->command = ux_propgrp_get_str(rmi->propgrp, "command", NULL); 
	if( !rmi->command ) {
		printf( "Not defined command in rmi .\n");
		return eSIMCTRL_INVALID_CONF;
	}

	val = ux_propgrp_get_str(rmi->propgrp, "display", "true");
	rmi->display = !strcasecmp(val, "true") ? 1 : 0;

	return eSIMCTRL_SUCCESS;
}


/**
 * @brief simulator controller class properties에서 설정된 scenario 개수 확인 함수
 * @param class class properties instance
 * @return scenario group count 
 */
int simctrl_propclass_get_scenario_count( simctrl_propclass_t *class )
{
	if( !class ) {
		printf( "Invalid parameters. class=%p\n", class);
		return eSIMCTRL_INVALID_PARAM;
	}

	return ux_propgrp_get_group_count(class->propgrp, "scenario");
}

/**
 * @brief simulator controller class properties에서 index번째 scenario 조회하 함수
 * @param class class properties instance
 * @param idx scenario group index
 * @param scenario properties group(scenario) instance
 * @return result code 
 */
int simctrl_propclass_get_scenario( simctrl_propclass_t *class, int idx, simctrl_propscen_t *scen)
{
	if( !class || idx < 0 || !scen ) {
		printf( "Invalid parameters. class=%p, idx=%d, scen=%p\n", class, idx, scen);
		return eSIMCTRL_INVALID_PARAM;
	}

	scen->propgrp = ux_propgrp_get_groupi(class->propgrp, "scenario", idx);
	if( !scen->propgrp ) {
		printf( "Not defined scenario group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	scen->name = ux_propgrp_get_str(scen->propgrp, "name", NULL); 
	if( !scen->name ) {
		printf( "Not defined name in scenario.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	scen->desc = ux_propgrp_get_str(scen->propgrp, "description", "");

	return eSIMCTRL_SUCCESS;
}


/**
 * @brief simulator controller scenario properties에서 설정된 rmi 개수 확인 함수
 * @param scen scenario properties instance
 * @return rmi group count 
 */
int simctrl_propscen_get_rmi_count( simctrl_propscen_t *scen )
{
	if( !scen ) {
		printf( "Invalid parameters. scen=%p\n", scen);
		return eSIMCTRL_INVALID_PARAM;
	}

	return ux_propgrp_get_group_count(scen->propgrp, "rmi");
}

/**
 * @brief simulator controller scenario properties에서 index번째 rmi 조회하 함수
 * @param scen scenario properties instance
 * @param idx rmi group index
 * @param rmi properties group(rmi) instance
 * @return result code 
 */
int simctrl_propscen_get_rmi( simctrl_propscen_t *scen, int idx, simctrl_proprmi_t *rmi)
{
	const char *val;

	if( !scen || idx < 0 || !rmi ) {
		printf( "Invalid parameters. scen=%p, idx=%d, rmi=%p\n", scen, idx, rmi);
		return eSIMCTRL_INVALID_PARAM;
	}

	rmi->propgrp = ux_propgrp_get_groupi(scen->propgrp, "rmi", idx);
	if( !rmi->propgrp ) {
		printf( "Not defined rmi group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	rmi->command = ux_propgrp_get_str(rmi->propgrp, "command", NULL); 
	if( !rmi->command ) {
		printf( "Not defined command in rmi .\n");
		return eSIMCTRL_INVALID_CONF;
	}

	val = ux_propgrp_get_str(rmi->propgrp, "display", "true");
	rmi->display = !strcasecmp(val, "true") ? 1 : 0;

	return eSIMCTRL_SUCCESS;
}


/**
 * @brief simulator controller scenario properties에서 설정된 command 개수 확인 함수
 * @param scen scenario properties instance
 * @return command group count 
 */
int simctrl_propscen_get_command_count( simctrl_propscen_t *scen )
{
	if( !scen ) {
		printf( "Invalid parameters. scen=%p\n", scen);
		return eSIMCTRL_INVALID_PARAM;
	}

	return ux_propgrp_get_group_count(scen->propgrp, "command");
}

/**
 * @brief simulator controller scenario properties에서 index번째 command 조회 함수
 * @param class class properties instance
 * @param idx scenario group index
 * @param scenario properties group(scenario) instance
 * @return result code 
 */
int simctrl_propscen_get_command( simctrl_propscen_t *scen, int idx, simctrl_propcmd_t *cmd )
{
	const char *val;

	if( !scen || idx < 0 || !cmd ) { 
		printf( "Invalid parameters. scen=%p, idx=%d, cmd=%p\n", scen, idx, cmd);
		return eSIMCTRL_INVALID_PARAM; 
	}

	cmd->propgrp = ux_propgrp_get_groupi(scen->propgrp, "command", idx);
	if( !cmd->propgrp ) {
		printf( "Not defined command group.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	cmd->script = ux_propgrp_get_str(cmd->propgrp, "script", NULL);
	if( !cmd->script ) {
		printf("Not defined script field in command.\n");
		return eSIMCTRL_INVALID_CONF;
	}

	cmd->check = ux_propgrp_get_str(cmd->propgrp, "check", "");

	val = ux_propgrp_get_str(cmd->propgrp, "check_type", NULL);
	if( !val ) cmd->check_type = SIMCTRL_SCEN_TYPE_NULL; 
	else if( !strcasecmp(val, "seagull") ) cmd->check_type = SIMCTRL_SCEN_TYPE_SEAGULL; 
	else if( !strcasecmp(val, "sipp") ) cmd->check_type = SIMCTRL_SCEN_TYPE_SIPP; 
	else if( !strcasecmp(val, "none") ) cmd->check_type = SIMCTRL_SCEN_TYPE_DEFAULT; 
	else cmd->check_type = SIMCTRL_SCEN_TYPE_DEFAULT;

	val = ux_propgrp_get_str(cmd->propgrp, "display", "true"); 
	cmd->display = !strcasecmp(val, "true") ? 1 : 0; 

	val = ux_propgrp_get_str(cmd->propgrp, "report", "true");
	cmd->report = !strcasecmp(val, "true") ? 1 : 0;

	cmd->timeout = -1; // default none(-1) 
	val = ux_propgrp_get_str(cmd->propgrp, "timeout", ""); 
	if( val && val[0] ) {
		cmd->timeout = strtol( val, 0, 0 );
		if( cmd->timeout < 0 || cmd->timeout > 3600 ) {
			printf("Invalid timeout value(%d). range is 0 ~ 3600\n", cmd->timeout);
			return eSIMCTRL_INVALID_CONF;
		}
	}

	cmd->sleep = ux_propgrp_get_int(cmd->propgrp, "sleep", 0);
	if( cmd->sleep < 0 || cmd->sleep > 360 ) {
		printf("Invalid sleep value(%d). range is 0 ~ 360\n", cmd->sleep);
		return eSIMCTRL_INVALID_CONF;
	}

	val = ux_propgrp_get_str(cmd->propgrp, "serial", "false");
	cmd->serial = !strcasecmp(val, "true") ? 1 : 0;

	cmd->remote_host = ux_propgrp_get_str(cmd->propgrp, "remote_host", "");
	cmd->remote_passwd = ux_propgrp_get_str(cmd->propgrp, "remote_passwd", "");

	return eSIMCTRL_SUCCESS;
}


/**
 * @brief properties group에서 특정 field에 대한 값(integer)을 구하는 함수 
 * @param grp properties group instance
 * @param name field name
 * @param dft default return value (if find value of name is null)
 * @return integer type value
 * @see simctrl_propgrp_get_str()
 */
int simctrl_propgrp_get_int( simctrl_propgrp_t *grp, const char *name, int dft )
{
	if( !grp || !name ) {
		printf( "Invalid parameters. grp=%p, name=%p\n", grp, name);
		return dft;
	}

	return ux_propgrp_get_int(grp, name, dft);
}

/**
 * @brief properties group에서 특정 field에 대한 값(string)을 구하는 함수 
 * @param grp properties group instance
 * @param name field name
 * @param val load value string buffer
 * @param len load value string buffer length
 * @param dft default return value (if find value of name is null)
 * @return load value string buffer pointer 
 * @see simctrl_propgrp_get_int()
 */
char* simctrl_propgrp_get_str( simctrl_propgrp_t *grp, const char *name, 
		char *val, int len, const char *dft )
{
	const char *ptr;

	if( !grp || !name || !val || len <= 1 ) {
		printf( "Invalid parameters. grp=%p, name=%p, val=%p, len=%d\n", 
				grp, name, val, len);
		return NULL;
	}

	val[0] = '\0';

	ptr = ux_propgrp_get_str(grp, name, dft);
	if( !ptr || strlen(ptr) >= len ) {
		printf( "Invaild %s parameter. value is %s.\n", name, ptr ? ptr : "Null");
	}
	strcpy(val, ptr);
	return val;
}

/**
 * @brief properties group에서 name field에 대한 값(string)을 구하는 함수 
 * @param grp properties group instance
 * @param val load value string buffer
 * @param len load value string buffer length
 * @return load value string buffer pointer 
 * @see simctrl_propgrp_get_str()
 */
char *simctrl_propgrp_get_name( simctrl_propgrp_t *grp, char *val, int len )
{
	return simctrl_propgrp_get_str( grp, "name", val, len, NULL);
}

/**
 * @brief properties group에서 description field에 대한 값(string)을 구하는 함수 
 * @param grp properties group instance
 * @param val load value string buffer
 * @param len load value string buffer length
 * @return load value string buffer pointer 
 * @see simctrl_propgrp_get_str()
 */
char *simctrl_propgrp_get_desc( simctrl_propgrp_t *grp, char *val, int len )
{
	return simctrl_propgrp_get_str( grp, "description", val, len, "");
}


//////////////////////////////////////////////////////////////////////////////
// private functions for simulator controller - properties
//////////////////////////////////////////////////////////////////////////////
