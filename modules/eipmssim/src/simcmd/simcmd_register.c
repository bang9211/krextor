#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "simcmd_errno.h"
#include "simcmd_register.h"


//////////////////////////////////////////////////////////////////////////////
// private defines, predefine functions
//////////////////////////////////////////////////////////////////////////////

#define _SIMCMD_REG_DEFAULT_COUNT	100
/** @brief private simulator command register type */
typedef struct _simcmd_reg_s _simcmd_reg_t;
/** @brief private simulator command register group type */
typedef struct _simcmd_reg_grp_s _simcmd_reg_grp_t;
/** @brief private simulator command register list type */
typedef struct _simcmd_reg_list_s _simcmd_reg_list_t;

/** @breif private simulator command register struct */
struct _simcmd_reg_s {
	simcmd_reg_t base; /**< simulator command register ����. name, help, func */
	char *grpname; /**< simulator command register Ȯ�� ����. group name */
};

/** @breif private simulator command register group struct */
struct _simcmd_reg_grp_s {
	char *name;	/**< history�� ���� ���� ����. group name */
	int ncmd; /**< history�� ���� ���� ����. reference command count */ 
};

/** @breif private simulator command register list struct */
struct _simcmd_reg_list_s {
	_simcmd_reg_t *regs; /**< register command list */
	int nreg; /**< register command count */
	int allocreg; /**< register struct memory allocation count */
	_simcmd_reg_grp_t *grps; /**< register group list */
	int ngrp; /**< register group count */
	int allocgrp; /**< register group struct memory allocation count */
};

static _simcmd_reg_list_t _g_reg_lists[MAX_SIMCMD_REG_COUNT];
static _simcmd_reg_list_t *_g_reg_list = &_g_reg_lists[0]; 


static int _simcmd_reg_cmp_f( const void *a, const void *b );
static _simcmd_reg_t* _simcmd_reg_get( char *name );
static int _simcmd_reg_insert( char *grpname, simcmd_reg_t *reg );
static int _simcmd_reg_update( char *grpname, simcmd_reg_t *reg );
static int _simcmd_reg_delete( char *name ); 
static int _simcmd_reg_set( char *grpname, simcmd_reg_t *src, _simcmd_reg_t *dst );

static int _simcmd_reg_grp_add( char *grpname );
static int _simcmd_reg_grp_del( char *grpname );


//////////////////////////////////////////////////////////////////////////////
// public functions 
//////////////////////////////////////////////////////////////////////////////

/** 
 * @brief simulator commmand register main index�� �����Ѵ�. 
          �ϳ��� simulator���� state ���� �ٸ� command list�� ����� �� �־� 
 	      register�� index(0~9)�� �������� ������ ������ �� �ֵ��� �Ѵ�. 
 * @param idx register main index
 * @return idx or -1 (lange error case)
 */
int simcmd_reg_set_mainidx( int idx )
{
	if( idx < 0 || idx >= MAX_SIMCMD_REG_COUNT ) return eSIMCMD_RANGE_ERROR; 
	_g_reg_list = &_g_reg_lists[idx];
	return idx;
}


/**
 * @brief simulator command ��� �Լ� 
 * @param grpname command group name
 * @param reg simulator command list (name, callback function, help)
 * @return result code
 */
int simcmd_reg_register( char *grpname, simcmd_reg_t *reg )
{
	int rv, i;

	if( !reg || !reg->name || !reg->func ) {
		return eSIMCMD_INVALID_PARAM;
	}

	for( i = 0 ; reg[i].name ; i ++ ) {
		// insert�ϰ� �̹� �����ϴ� ��� update���ش�.
		rv = _simcmd_reg_insert( grpname, &reg[i] );
		if( rv == eSIMCMD_CMD_ALREADY_EXIST ) {
			rv = _simcmd_reg_update( grpname, &reg[i] );
		}
		if( rv != eSIMCMD_SUCCESS ) return rv;
	}

	return 0;
}


/**
 * @brief simulator command ��� ���� ��ȸ �Լ�
 * @return register command count
 */
int simcmd_reg_count()
{
	return _g_reg_list->nreg;
}

/**
 * @brief index�� �������� ��ϵ� simulator command�� name�� ��ȸ�ϴ� �Լ�
 * @param idx simulator command index
 * @return simulator command name
 */
char* simcmd_reg_get_name( int idx )
{
	if( idx < 0 || idx >= _g_reg_list->nreg ) return NULL;
	return _g_reg_list->regs[idx].base.name;
}

/**
 * @brief name�� �������� ��ϵ� simulator command�� help�� ��ȸ�ϴ� �Լ�
 * @param name simulator command name
 * @return simulator command help
 */
char* simcmd_reg_get_help( char *name )
{
	_simcmd_reg_t *reg = _simcmd_reg_get( name );
	if( !reg ) return NULL;
	else return reg->base.help;
}

/**
 * @brief name�� �������� ��ϵ� simulator command�� ó���� callback function�� ��ȸ�ϴ� �Լ�
 * @param name simulator command name
 * @return simulator command processing callback function
 */
simcmd_proc_f simcmd_reg_get_func( char *name )
{
	_simcmd_reg_t *reg = _simcmd_reg_get( name );
	if( !reg ) return NULL;
	else return reg->base.func;
}

/**
 * @brief name�� �������� ��ϵ� simulator command�� group name�� ��ȸ�ϴ� �Լ�
 * @param name simulator command name
 * @return simulator command group name
 */
char* simcmd_reg_get_grpname( char *name )
{
	_simcmd_reg_t *reg = _simcmd_reg_get( name );
	if( !reg ) return NULL;
	else return reg->grpname;
}


/**
 * @brief simulator command group ��� ���� ��ȸ �Լ�
 * @return register command group count
 */
int simcmd_reg_grp_count()
{
	return _g_reg_list->ngrp;
}

/**
 * @brief index�� �������� ��ϵ� simulator command group�� name�� ��ȸ�ϴ� �Լ�
 * @param idx simulator command group index
 * @return simulator command group name
 */
char* simcmd_reg_get_grpname_i( int idx )
{
	if( idx < 0 || idx >= _g_reg_list->ngrp ) return NULL;
	return _g_reg_list->grps[idx].name;
}


//////////////////////////////////////////////////////////////////////////////
// private functions 
//////////////////////////////////////////////////////////////////////////////

/** @internal simulator command name�� ���ϴ� �Լ�. */
static int _simcmd_reg_cmp_f( const void *a, const void *b )
{
	_simcmd_reg_t *aa = (_simcmd_reg_t*)a;
	_simcmd_reg_t *bb = (_simcmd_reg_t*)b;
	return (strcasecmp(aa->base.name, bb->base.name));
}

/** @internal name�� �������� _simcmd_reg_t�� ã�´�. */
static _simcmd_reg_t* _simcmd_reg_get( char *name )
{
	static _simcmd_reg_t _reg;

	if( !name ) return NULL;

	_reg.base.name = name;
	return bsearch(&_reg, _g_reg_list->regs, _g_reg_list->nreg, 
				sizeof(_simcmd_reg_t), _simcmd_reg_cmp_f);
}

/** @internal _simcmd_reg_t�� ����Ѵ�. */
static int _simcmd_reg_insert( char *grpname, simcmd_reg_t* reg )
{
	int rv;
	_simcmd_reg_t *_reg;

	// list array full �� ��� �߰� �Ҵ����ش�.
	if( _g_reg_list->nreg == _g_reg_list->allocreg ) {
		if( _g_reg_list->allocreg == 0 ) {
			_g_reg_list->regs = malloc(sizeof(_simcmd_reg_t)*_SIMCMD_REG_DEFAULT_COUNT);
		} else {
			_g_reg_list->regs = realloc( _g_reg_list->regs, 
					sizeof(_simcmd_reg_t)*(_g_reg_list->allocreg+_SIMCMD_REG_DEFAULT_COUNT));
		}
		if( !_g_reg_list->regs ) return eSIMCMD_MEMORY_ALLOC;
		_g_reg_list->allocreg += _SIMCMD_REG_DEFAULT_COUNT;
	}

	// list array�� �̹� ��ϵǾ��ִ��� Ȯ���Ѵ�.
	_reg = _simcmd_reg_get(reg->name);
	if( _reg ) return eSIMCMD_CMD_ALREADY_EXIST;

	// list array�� �������� �Ҵ��ϰ� ������ ����Ѵ�.
	_reg = &_g_reg_list->regs[_g_reg_list->nreg];
	memset(_reg, 0, sizeof(_simcmd_reg_t));
	rv = _simcmd_reg_set( grpname, reg, _reg );
	if( rv != eSIMCMD_SUCCESS ) return rv;

	// list array�� name�� �������� �������ش�.
	_g_reg_list->nreg += 1;
	qsort(_g_reg_list->regs, _g_reg_list->nreg, sizeof(_simcmd_reg_t), _simcmd_reg_cmp_f);

	return eSIMCMD_SUCCESS;
}

/** @internal _simcmd_reg_t�� �����Ѵ�. */
static int _simcmd_reg_update( char *grpname, simcmd_reg_t *reg )
{
	int rv;
	_simcmd_reg_t *_reg;

	// list array�� ��ϵǾ��ִ��� Ȯ���Ѵ�.
	_reg = _simcmd_reg_get(reg->name);
	if( !_reg ) return eSIMCMD_CMD_NOT_FOUND;

	// ��ϵ� ������ ������Ʈ���ش�.
	rv = _simcmd_reg_set( grpname, reg, _reg );
	if( rv != eSIMCMD_SUCCESS ) return rv;

	return eSIMCMD_SUCCESS;
}

/** @internal _simcmd_reg_t�� �����Ѵ�. */
static int _simcmd_reg_delete( char *name ) 
{
	_simcmd_reg_t *_reg;

	// list array�� ��ϵǾ��ִ��� Ȯ���Ѵ�.
	_reg = _simcmd_reg_get(name);
	if( !_reg ) return eSIMCMD_CMD_NOT_FOUND;

	// ��ϵ� ������ �������ش�.
	if( _reg->base.name ) free(_reg->base.name);
	if( _reg->base.help ) free(_reg->base.help);
	if( _reg->grpname ) {
		// help�� ���� group ������ �����Ѵ�.
		_simcmd_reg_grp_del( _reg->grpname );
		free(_reg->grpname);
	}

	// list array���� ���ܽ�Ű�� name�� �������� �������ش�.
	memcpy(_reg, &_g_reg_list->regs[_g_reg_list->nreg-1], sizeof(_simcmd_reg_t));
	memset(&_g_reg_list->regs[_g_reg_list->nreg-1], 0, sizeof(_simcmd_reg_t));
	_g_reg_list->nreg -= 1;
	if( _g_reg_list->nreg < 0 ) _g_reg_list->nreg = 0;
	qsort(_g_reg_list->regs, _g_reg_list->nreg, sizeof(_simcmd_reg_t), _simcmd_reg_cmp_f);

	return eSIMCMD_SUCCESS;
}

/** @internal _simcmd_reg_t�� ���� �����Ѵ�. */
static int _simcmd_reg_set( char *grpname, simcmd_reg_t *src, _simcmd_reg_t *dst )
{
	int rv = eSIMCMD_SUCCESS;

	// ��ȸ�� �������� �Ҵ��� �޸� ��ȯ�Ѵ�. 
	// Name�� Key �����̹Ƿ� �������� �ʵ��� �Ѵ�.
	if( dst->grpname ) {
		// help�� ���� group ������ �����Ѵ�.
		_simcmd_reg_grp_del( dst->grpname );
		free( dst->grpname );
		dst->grpname = NULL;
	}
	if( dst->base.name ) {
		free( dst->base.name );
		dst->base.name = NULL;
	}
	dst->base.func = NULL;

	// ������ ������ �ݿ����ش�.
	if( !dst->base.name ) { // name�� �����Ǿ����� �ʴٸ� �������ش�.
		dst->base.name = strdup( src->name );
		if( !dst->base.name ) rv = eSIMCMD_MEMORY_ALLOC;
	}
	if( grpname ) {
		dst->grpname = strdup( grpname );
		if( !dst->grpname ) rv = eSIMCMD_MEMORY_ALLOC;
		// help�� ���� group ������ �߰����ش�.
		else _simcmd_reg_grp_add( dst->grpname );
	}
	if( src->help ) {
		dst->base.help = strdup( src->help ); // TODO : help modify. \n �� ..
		if( !dst->base.help ) rv = eSIMCMD_MEMORY_ALLOC; 
	}
	dst->base.func = src->func;

	if( rv != eSIMCMD_SUCCESS ) {
		if( dst->grpname ) {
			free( dst->grpname );
			dst->grpname = NULL;
		}
		if( dst->base.name ) {
			free( dst->base.name );
			dst->base.name = NULL;
		}
		dst->base.func = NULL;
	}

	return rv;
}


/** @internal simulator command group name�� ����Ѵ�. */
static int _simcmd_reg_grp_add( char *grpname )
{
	int i;

	// list array full �� ��� �߰� �Ҵ����ش�.
	if( _g_reg_list->ngrp == _g_reg_list->allocgrp ) {
		if( _g_reg_list->allocgrp == 0 ) {
			_g_reg_list->grps = malloc(sizeof(_simcmd_reg_grp_t)*_SIMCMD_REG_DEFAULT_COUNT);
		} else {
			_g_reg_list->grps = realloc( _g_reg_list->grps,
					sizeof(_simcmd_reg_grp_t)*(_g_reg_list->allocgrp+_SIMCMD_REG_DEFAULT_COUNT));
		}
		if( !_g_reg_list->grps ) return eSIMCMD_MEMORY_ALLOC;
		_g_reg_list->allocgrp += _SIMCMD_REG_DEFAULT_COUNT;
	}

	for( i = 0 ; i < _g_reg_list->ngrp ; i ++ ) {
		if( !strcasecmp(grpname, _g_reg_list->grps[i].name) ) break;
	}
	if( i < _g_reg_list->ngrp ) { // �̹� ��ϵǾ��ִٸ� ������ �÷��ش�.
		_g_reg_list->grps[i].ncmd += 1;
	} else { // ��ϵǾ����� �ʴٸ� �߰����ش�.
		_g_reg_list->grps[i].name = strdup(grpname);
		if( !_g_reg_list->grps[i].name ) return eSIMCMD_MEMORY_ALLOC;
		_g_reg_list->grps[i].ncmd = 1;
		_g_reg_list->ngrp += 1;
	}
	return eSIMCMD_SUCCESS;
}

/** @internal simulator command group name�� �����Ѵ�. */
static int _simcmd_reg_grp_del( char *grpname )
{
	int i;

	// list array�� ��ϵǾ��ִ��� Ȯ���Ѵ�.
	for( i = 0 ; i < _g_reg_list->ngrp ; i ++ ) {
		if( !strcasecmp(grpname, _g_reg_list->grps[i].name) ) break;
	}
	if( i < _g_reg_list->ngrp ) { // ��ϵǾ����� cmd ���������� ���δ�.
		_g_reg_list->grps[i].ncmd -= 1;
		if( _g_reg_list->grps[i].ncmd <= 0 ) { // �����ϴ� cmd�� ���ٸ� �����Ѵ�.
			free( _g_reg_list->grps[i].name);
			memcpy(&_g_reg_list->grps[i], &_g_reg_list->grps[_g_reg_list->ngrp-1], 
					sizeof(_simcmd_reg_grp_t));
			memset(&_g_reg_list->grps[_g_reg_list->ngrp-1], 0, sizeof(_simcmd_reg_grp_t));
			_g_reg_list->ngrp -= 1;
			if( _g_reg_list->ngrp < 0 ) _g_reg_list->ngrp = 0;
		}
	} else { // �̹� �����Ǿ��ִٸ� �׳� ���� ó���Ѵ�.
		;
	}
	return eSIMCMD_SUCCESS;
}
