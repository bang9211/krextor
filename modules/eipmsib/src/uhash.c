#include "uhash.h"
#include <stdio.h>


uhash_int_t* uh_int_init() {
    uhash_int_t *hash = (uhash_int_t*)malloc(sizeof(uhash_int_t));
    hash->h = kh_init(m32);
    hash->available = 0;
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_int_put(uhash_int_t* hash, khint_t key, char* value) {
//    int absent;
    int r;
    khint_t k;
    
//    k = kh_get(m32, hash->h, key);
    // while (hash->available > 0);
    // hash->available++;
    pthread_mutex_lock(&hash->mutex_lock);
//    if (k == kh_end(hash->h)) {
//        k = kh_put(m32, hash->h, key, &absent);  // insert a key to the hash table
//        if (!absent) { 
//            kh_del(m32, hash->h, k);
//            hash->available--;
//            return 1;
//        }
//    }
//    kh_value(hash->h, k) = value;
    
    r = kh_set(m32, hash->h, key, value);
    pthread_mutex_unlock(&hash->mutex_lock);
    // hash->available--;
    if (r < 0) {
        return 0;
    }
    return 1;
}

char* uh_int_get(uhash_int_t* hash, khint_t key) {
    khint_t k;
    char* r;
    
    // while (hash->available > 0);
    // hash->available++;
    pthread_mutex_lock(&hash->mutex_lock);
    k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        return NULL;
    }
    r = kh_value(hash->h, k);
    pthread_mutex_unlock(&hash->mutex_lock);
    // hash->available--;

    return r;
}

int uh_int_is_exist(uhash_int_t* hash, khint_t key) {
    
    khint_t k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_int_destroy(uhash_int_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(m32, hash->h);
    free(hash);
}



uhash_str_t* uh_str_init() {
    uhash_str_t *hash = (uhash_str_t*)malloc(sizeof(uhash_str_t));
    hash->h = kh_init(str);
    hash->available = 0;
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_str_put(uhash_str_t* hash, char* key, char* value) {
//    int absent;
    int r;
    khint_t k;
    
//    k = kh_get(khStrStr, hash->h, key);
    // while (hash->available > 0);
    // hash->available++;
    
    pthread_mutex_lock(&hash->mutex_lock);
//    if (k == kh_end(hash->h)) {
//        k = kh_put(khStrStr, hash->h, key, &absent);  // insert a key to the hash table
//        if (!absent) { 
//            kh_del(khStrStr, hash->h, k);
//            hash->available--;
//            return 1;
//        }
//    }
//    kh_value(hash->h, k) = value;
    
    r = kh_set(str, hash->h, key, value);
    pthread_mutex_unlock(&hash->mutex_lock);
    // hash->available--;
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

void uh_str_destroy(uhash_str_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(str, hash->h);
    free(hash);
}

uhash_ipc_t* uh_ipc_init() {
    uhash_ipc_t *hash = (uhash_ipc_t*)malloc(sizeof(uhash_ipc_t));
    hash->h = kh_init(ipc);
    hash->available = 0;
    if (pthread_mutex_init(&hash->mutex_lock, NULL) != 0) {
        printf("Failed to pthread_mutex_init\n");
    }
    return hash;
}

int uh_ipc_put(uhash_ipc_t* hash, khint_t key, uxc_ixpc_t* value) {
    int r;
    khint_t k;
    
    // while (hash->available > 0);
    // hash->available++;

    pthread_mutex_lock(&hash->mutex_lock);
    r = kh_set(ipc, hash->h, key, value);
    // hash->available--;
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

void uh_ipc_destroy(uhash_ipc_t* hash) {
    pthread_mutex_destroy(&hash->mutex_lock);
    kh_destroy(ipc, hash->h);
    free(hash);
}