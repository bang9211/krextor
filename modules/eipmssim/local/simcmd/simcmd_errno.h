#ifndef __SIMCMD_ERRNO_H__
#define __SIMCMD_ERRNO_H__


#include <errno.h>


#define	eSIMCMD_SUCCESS 			0
#define eSIMCMD_ERROR				-1
#define eSIMCMD_INVALID_PARAM		-2
#define eSIMCMD_MEMORY_ALLOC		-3
#define eSIMCMD_CMD_NOT_FOUND		-4
#define eSIMCMD_CMD_ALREADY_EXIST	-5
#define eSIMCMD_RANGE_ERROR			-6
#define eSIMCMD_OPEN_FAILED			-7

#define simcmd_strerror( _errno ) 										\
	(_errno) == eSIMCMD_SUCCESS ? "Success" : 							\
	(_errno) == eSIMCMD_ERROR ? "Error" :								\
	(_errno) == eSIMCMD_INVALID_PARAM ? "Invalid parameter." : 			\
	(_errno) == eSIMCMD_MEMORY_ALLOC ? "Memory allocation failed." :	\
	(_errno) == eSIMCMD_CMD_NOT_FOUND ? "Not found command." : 			\
	(_errno) == eSIMCMD_CMD_ALREADY_EXIST ? "Already exist command." : 	\
	(_errno) == eSIMCMD_RANGE_ERROR ? "Invalid value range." :			\
	(_errno) == eSIMCMD_OPEN_FAILED ? "Open failed." :					\
	"Unknown error number."


#endif //#ifndef __SIMCMD_ERRNO_H__
