#include "uhash.h"
#include <stdio.h>

///////////////////////////////////
//int32_t
///////////////////////////////////

uhash_rid_t* uh_rid_init() {
    uhash_rid_t *hash = (uhash_rid_t*)malloc(sizeof(uhash_rid_t));
    hash->h = kh_init(rid);
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_rid_put(uhash_rid_t* hash, khint_t key) {
    int r;
    
    pthread_mutex_lock(&hash->mutex_lock);
    kh_put_rid(hash->h, key, &r);
    pthread_mutex_unlock(&hash->mutex_lock);

    if (r < 0) {
        return 0;
    }
    return 1;
}

int uh_rid_is_exist(uhash_rid_t* hash, khint_t key) {
    khint_t k = kh_get(rid, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_rid_del(uhash_rid_t* hash, khint_t key) {
    khint_t k;
    pthread_mutex_lock(&hash->mutex_lock);
    k = kh_get(rid, hash->h, key);
    if (k == kh_end(hash->h)) {
        pthread_mutex_unlock(&hash->mutex_lock);
        return;
    }
    kh_del(rid, hash->h, k);
    pthread_mutex_unlock(&hash->mutex_lock);
}

void uh_rid_destroy(uhash_rid_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(rid, hash->h);
    free(hash);
}

///////////////////////////////////
//int32_t - time_t
///////////////////////////////////

uhash_tmt_t* uh_tmt_init() {
    uhash_tmt_t *hash = (uhash_tmt_t*)malloc(sizeof(uhash_tmt_t));
    hash->h = kh_init(tmt);
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_tmt_put(uhash_tmt_t* hash, khint_t key, time_t value) {
    int r;
    khint_t k;
    
    pthread_mutex_lock(&hash->mutex_lock);
    r = kh_set(tmt, hash->h, key, value);
    pthread_mutex_unlock(&hash->mutex_lock);

    if (r < 0) {
        return 0;
    }
    return 1;
}

time_t uh_tmt_get(uhash_tmt_t* hash, khint_t key) {
    khint_t k;
    time_t r;
    
    k = kh_get(tmt, hash->h, key);
    if (k == kh_end(hash->h)) {
        return -1;
    }
    r = kh_value(hash->h, k);

    return r;
}

int uh_tmt_is_exist(uhash_tmt_t* hash, khint_t key) {
    khint_t k = kh_get(tmt, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_tmt_del(uhash_tmt_t* hash, khint_t key) {
    khint_t k;
    pthread_mutex_lock(&hash->mutex_lock);
    k = kh_get(tmt, hash->h, key);
    if (k == kh_end(hash->h)) {
        pthread_mutex_unlock(&hash->mutex_lock);
        return;
    }
    kh_del(tmt, hash->h, k);
    pthread_mutex_unlock(&hash->mutex_lock);
}

void uh_tmt_destroy(uhash_tmt_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(tmt, hash->h);
    free(hash);
}


///////////////////////////////////
//int32_t - char*
///////////////////////////////////

uhash_int_t* uh_int_init() {
    uhash_int_t *hash = (uhash_int_t*)malloc(sizeof(uhash_int_t));
    hash->h = kh_init(m32);
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_int_put(uhash_int_t* hash, khint_t key, char* value) {
    int r;
    khint_t k;
    
    pthread_mutex_lock(&hash->mutex_lock);
    r = kh_set(m32, hash->h, key, value);
    pthread_mutex_unlock(&hash->mutex_lock);

    if (r < 0) {
        return 0;
    }
    return 1;
}

char* uh_int_get(uhash_int_t* hash, khint_t key) {
    khint_t k;
    char* r;
    
    k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        return NULL;
    }
    r = kh_value(hash->h, k);

    return r;
}

int uh_int_is_exist(uhash_int_t* hash, khint_t key) {
    khint_t k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_int_del(uhash_int_t* hash, khint_t key) {
    khint_t k;
    pthread_mutex_lock(&hash->mutex_lock);
    k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        pthread_mutex_unlock(&hash->mutex_lock);
        return;
    }
    kh_del(m32, hash->h, k);
    pthread_mutex_unlock(&hash->mutex_lock);
}

void uh_int_destroy(uhash_int_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(m32, hash->h);
    free(hash);
}


///////////////////////////////////
//char* - char*
///////////////////////////////////

uhash_str_t* uh_str_init() {
    uhash_str_t *hash = (uhash_str_t*)malloc(sizeof(uhash_str_t));
    hash->h = kh_init(str);
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_str_put(uhash_str_t* hash, char* key, char* value) {
    int r;
    khint_t k;
    
    pthread_mutex_lock(&hash->mutex_lock);
    r = kh_set(str, hash->h, key, value);
    pthread_mutex_unlock(&hash->mutex_lock);

    if (r < 0) {
        return 0;
    }
    return 1;
}

char* uh_str_get(uhash_str_t* hash, kh_cstr_t key) {
    khint_t k;
    
    k = kh_get(str, hash->h, key);
    if (k == kh_end(hash->h)) {
        return NULL;
    }
    return kh_value(hash->h, k);
}

int uh_str_is_exist(uhash_str_t* hash, kh_cstr_t key) {
    khint_t k = kh_get(str, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_str_del(uhash_str_t* hash, kh_cstr_t key) {
    khint_t k;
    pthread_mutex_lock(&hash->mutex_lock);
    k = kh_get(str, hash->h, key);
    if (k == kh_end(hash->h)) {
        pthread_mutex_unlock(&hash->mutex_lock);
        return;
    }
    kh_del(str, hash->h, k);
    pthread_mutex_unlock(&hash->mutex_lock);
}

void uh_str_destroy(uhash_str_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(str, hash->h);
    free(hash);
}


///////////////////////////////////
//int32_t - uxc_ixpc_t*
///////////////////////////////////

uhash_ipc_t* uh_ipc_init() {
    uhash_ipc_t *hash = (uhash_ipc_t*)malloc(sizeof(uhash_ipc_t));
    hash->h = kh_init(ipc);
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_ipc_put(uhash_ipc_t* hash, khint_t key, uxc_ixpc_t* value) {
    int r;
    khint_t k;
    
    pthread_mutex_lock(&hash->mutex_lock);
    r = kh_set(ipc, hash->h, key, value);
    pthread_mutex_unlock(&hash->mutex_lock);
    if (r < 0) {
        return 0;
    }
    return 1;
}

uxc_ixpc_t* uh_ipc_get(uhash_ipc_t* hash, khint_t key) {
    khint_t k;
    
    k = kh_get(ipc, hash->h, key);
    if (k == kh_end(hash->h)) {
        return NULL;
    }
    return kh_value(hash->h, k);
}

int uh_ipc_is_exist(uhash_ipc_t* hash, khint_t key) {
    khint_t k = kh_get(ipc, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_ipc_del(uhash_ipc_t* hash, khint_t key) {
    khint_t k;
    pthread_mutex_lock(&hash->mutex_lock);
    k = kh_get(ipc, hash->h, key);
    if (k == kh_end(hash->h)) {
        pthread_mutex_unlock(&hash->mutex_lock);
        return;
    }
    kh_del(ipc, hash->h, k);
    pthread_mutex_unlock(&hash->mutex_lock);
}

void uh_ipc_destroy(uhash_ipc_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(ipc, hash->h);
    free(hash);
}