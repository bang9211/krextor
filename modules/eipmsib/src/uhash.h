#ifndef UHASH_H
#define UHASH_H

#include <uxcutor/uxc_ipcmsg.h>
#include<pthread.h>
#include "khash.h"

// shorthand way to get the key from hashtable or defVal if not found
#define kh_get_val(kname, hash, key, defVal) ({k=kh_get(kname, hash, key);(k!=kh_end(hash)?kh_val(hash,k):defVal);})

// shorthand way to set value in hash with single line command.  Returns value
// returns 0=replaced existing item, 1=bucket empty (new key), 2-adding element previously deleted
#define kh_set(kname, hash, key, val) ({int ret; k = kh_put(kname, hash,key,&ret); kh_value(hash,k) = val; ret;})

KHASH_MAP_INIT_INT(m32, char*)      // instantiate structs and methods
KHASH_MAP_INIT_STR(str, char*);     // setup khash to handle string key with string body
KHASH_MAP_INIT_INT(ipc, uxc_ixpc_t*)      // instantiate structs and methods

typedef struct uhash_int_s uhash_int_t;
struct uhash_int_s {
    kh_m32_t* h;
    int available;
    pthread_mutex_t mutex_lock;
};

typedef struct uhash_str_s uhash_str_t;
struct uhash_str_s {
    kh_str_t* h;
    int available;
    pthread_mutex_t mutex_lock;
};

typedef struct uhash_ipc_s uhash_ipc_t;
struct uhash_ipc_s {
    kh_m32_t* h;
    int available;
    pthread_mutex_t mutex_lock;
};

uhash_int_t* uh_int_init();
int uh_int_put(uhash_int_t* hash, khint_t key, char* value);
char* uh_int_get(uhash_int_t* hash, khint_t key);
int uh_int_is_exist(uhash_int_t* hash, khint_t key);
void uh_int_destroy(uhash_int_t* hash);

uhash_str_t* uh_str_init();
int uh_str_put(uhash_str_t* hash, char* key, char* value);
char* uh_str_get(uhash_str_t* hash, kh_cstr_t key);
int uh_str_is_exist(uhash_str_t* hash, kh_cstr_t key);
void uh_str_destroy(uhash_str_t* hash);

uhash_ipc_t* uh_ipc_init();
int uh_ipc_put(uhash_ipc_t* hash, khint_t key, uxc_ixpc_t* value);
uxc_ixpc_t* uh_ipc_get(uhash_ipc_t* hash, khint_t key);
int uh_ipc_is_exist(uhash_ipc_t* hash, khint_t key);
void uh_ipc_destroy(uhash_ipc_t* hash);


#endif /* UHASH_H */