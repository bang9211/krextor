#include <stdlib.h>
#include <unistd.h>
#include "simcmd.h"
#include "ipmssim_cmd.h"


static int _g_simcmd_type = SIMCMD_TYPE_RL;

extern simcmd_reg_t simcmd_reg_common[];
extern simcmd_reg_t simcmd_reg_common_char[];


/////////////////////////////////////////////////////////////////////////////////////////
// public functions 
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief simuator command manager �ʱ�ȭ �Լ�
 * @param prompt prompt string
 * @param timeout_msec stdin select timeout(milliseconds)
 * @return result code
 */
int simcmd_init( char *prompt, int timeout_msec )
{
	int rv;

	// readline �ʱ�ȭ
	rv = simcmd_rl_init( prompt, timeout_msec );
	if( rv != eSIMCMD_SUCCESS ) return rv;

	// readchar �ʱ�ȭ
	rv = simcmd_rc_init( prompt, timeout_msec );
	if( rv != eSIMCMD_SUCCESS ) return rv;

	// ibrs �ʱ�ȭ �� commoand�� ����Ѵ�.
	rv = ipmssim_cmd_init( prompt, timeout_msec );

	// common command�� ����Ѵ�. - readline
	rv = simcmd_register( SIMCMD_TYPE_RL, "common", simcmd_reg_common);
	if( rv != eSIMCMD_SUCCESS ) return rv;

	// common command�� ����Ѵ�. - readchar
	rv = simcmd_register( SIMCMD_TYPE_RC, "common", simcmd_reg_common_char);
	if( rv != eSIMCMD_SUCCESS ) return rv;

	// default�� readline�̴�.
	_g_simcmd_type = SIMCMD_TYPE_RL;
	simcmd_reg_set_mainidx( SIMCMD_TYPE_RL );

	ipmssim_cmd_start();

	// TODO : �ٸ� class �ʱ�ȭ �� �ʿ� ���� ��� �߰����ش�.

	return eSIMCMD_SUCCESS;
}


/**
 * @brief simulator command type ���� �Լ�
 * @param type simulator command type (SIMCMD_TYPE_RL|SIMCMD_TYPE_RC)
 * @return type or -1 (lange error case)
 */
int simcmd_set_type( int type )
{
	if( type == SIMCMD_TYPE_RL || type == SIMCMD_TYPE_RC ) {
		_g_simcmd_type = type;
		simcmd_reg_set_mainidx( type );
		return type;
	} else {
		return eSIMCMD_RANGE_ERROR; 
	}
}


/**
 * @brief simulator command type ��ȸ �Լ�
 * @return simulator command type (SIMCMD_TYPE_RL|SIMCMD_TYPE_RC)
 */
int simcmd_get_type( void )
{
	return _g_simcmd_type;
}


/**
 * @brief simulator command ��� �Լ� 
 * @param type simulator command type (SIMCMD_TYPE_RL|SIMCMD_TYPE_RC)
 * @param name command group name
 * @param reg simulator command list (name, callback function, help)
 * @return result code 
 */
int simcmd_register( int type, char *name, simcmd_reg_t *reg )
{
	int rv;

	if( type == SIMCMD_TYPE_RL || type == SIMCMD_TYPE_RC ) { 
		rv = simcmd_reg_set_mainidx( type );
		if( rv < eSIMCMD_SUCCESS ) return rv;
		rv = simcmd_reg_register(name, reg);
		simcmd_reg_set_mainidx( _g_simcmd_type );
	} else {
		rv = eSIMCMD_RANGE_ERROR;
	}
	return rv;
}


/**
 * @brief simulator command ���� �� command ���� �Լ�
 * @return result code
 */
int simcmd_readline( void )
{
	if( _g_simcmd_type == SIMCMD_TYPE_RL ) return simcmd_rl_readline();
	else /*_g_simcmd_type == SIMCMD_TYPE_RC*/ return simcmd_rc_readchar();
}

