#ifndef __SIMCTRL_DEF_H__
#define __SIMCTRL_DEF_H__


#ifdef __cplusplus 
extern "C" {
#endif 


/** @brief string buffer size */
#define SIMCTRL_NAME_SIZE			64
#define SIMCTRL_FILE_SIZE			256
#define SIMCTRL_DESC_SIZE			1024	
#define SIMCTRL_KEYWORD_SIZE		128
#define SIMCTRL_HOST_SIZE			128
#define SIMCTRL_PORT_SIZE			8

/** @brief simulator controller prompt string */
#define SIMCTRL_PROMPT_STR			"[SIMULATOR CONTROLLER] "

/** @brief select timeout (msec) */
#define SIMCTRL_SELECT_TIMEOUT		10

/** @brief max scenario group(class) count */
#define MAX_SIMCTRL_GROUP			30	/* 2016.01.11 modified by choisk. 20 -> 30 */	
/** @brief max scenario count */
#define MAX_SIMCTRL_SCENARIO		100	
/** @brief max rmi count */
#define MAX_SIMCTRL_RMI				20	/* 2019.04.23 modified by choisk. 10 -> 20 */
/** @brief max command in scenario count */
#define MAX_SIMCTRL_COMMAND			30	/* 2016.01.11 modified by choisk. 8 -> 16 */
										/* 2019.04.23 modified by choisk. 16 -> 30 */
/** @brief max ignore keyword count */
#define MAX_SIMCTRL_KEYWORD			64


/** @brief scenario type */
#define SIMCTRL_SCEN_TYPE_NULL		-1
#define SIMCTRL_SCEN_TYPE_DEFAULT	0
#define SIMCTRL_SCEN_TYPE_SEAGULL	1
#define SIMCTRL_SCEN_TYPE_SIPP		2
#define SIMCTRL_SCEN_TYPE_RMI		3
#define simctrl_scen_type_str( _scen_type ) ( 				\
	(_scen_type) == SIMCTRL_SCEN_TYPE_NULL ? "null" : 		\
	(_scen_type) == SIMCTRL_SCEN_TYPE_DEFAULT ? "none" : 	\
	(_scen_type) == SIMCTRL_SCEN_TYPE_SEAGULL ? "seagull" :	\
	(_scen_type) == SIMCTRL_SCEN_TYPE_SIPP ? "sipp" : 		\
	(_scen_type) == SIMCTRL_SCEN_TYPE_RMI ? "rmi" :         \
	"unknown" )


/** @brief define error codes */
#define eSIMCTRL_SUCC_CONTINUE		 1
#define eSIMCTRL_MSG_CONTINUE		 2
#define eSIMCTRL_SUCCESS			 0
#define eSIMCTRL_ERROR				-1
#define eSIMCTRL_INVALID_PARAM		-2
#define eSIMCTRL_NOT_FOUND			-3
#define eSIMCTRL_ALREADY_EXIST		-4
#define eSIMCTRL_NO_MEMORY			-5
#define eSIMCTRL_BUF_OVERFLOW		-6
#define eSIMCTRL_INVALID_CONF		-7
#define eSIMCTRL_FILE_OPEN_FAIL		-8
#define eSIMCTRL_SCEN_TIMEOUT		-9

#define simctrl_strerror( _errno ) (								\
	(_errno) == eSIMCTRL_SUCC_CONTINUE ? "Success with continue" : 	\
	(_errno) == eSIMCTRL_MSG_CONTINUE ? "Message continue" : 		\
	(_errno) == eSIMCTRL_SUCCESS ? "Success" :						\
	(_errno) == eSIMCTRL_ERROR ? "Error" :							\
	(_errno) == eSIMCTRL_INVALID_PARAM ? "Invalid parameter" :		\
	(_errno) == eSIMCTRL_NOT_FOUND ? "Not found" :					\
	(_errno) == eSIMCTRL_ALREADY_EXIST ? "Already exist" :			\
	(_errno) == eSIMCTRL_NO_MEMORY ? "No memory" :					\
	(_errno) == eSIMCTRL_BUF_OVERFLOW ? "Buffer overflow" :			\
	(_errno) == eSIMCTRL_INVALID_CONF ? "Invalid configure"	:		\
	(_errno) == eSIMCTRL_FILE_OPEN_FAIL ? "File open failed" :		\
	(_errno) == eSIMCTRL_SCEN_TIMEOUT ? "Scenario timeout" :		\
	"Unknown error number" )


#ifdef __cplusplus 
}
#endif 


#endif //#ifndef __SIMCTRL_DEF_H__
