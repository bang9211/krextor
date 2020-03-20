#include "uhash.h"


uhash_int_t* uh_int_init() {
    uhash_int_t *hash = (uhash_int_t*)malloc(sizeof(uhash_int_t));
    hash->h = kh_init(m32);
    return hash;
}

int uh_int_put(uhash_int_t* hash, khint_t key, char* value) {
//    int absent;
    int ret;
    khint_t k;
    
//    k = kh_get(m32, hash->h, key);
    while (hash->available > 0);
    hash->available++;
    
//    if (k == kh_end(hash->h)) {
//        k = kh_put(m32, hash->h, key, &absent);  // insert a key to the hash table
//        if (!absent) { 
//            kh_del(m32, hash->h, k);
//            hash->available--;
//            return 1;
//        }
//    }
//    kh_value(hash->h, k) = value;
    
    ret = kh_set(m32, hash->h, key, value);
    hash->available--;
    if (ret < 0) {
        return 0;
    }
    return 1;
}

char* uh_int_get(uhash_int_t* hash, khint_t key) {
    khint_t k;
    
    k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        return NULL;
    }
    return kh_value(hash->h, k);
}

int uh_int_is_exist(uhash_int_t* hash, khint_t key) {
    
    khint_t k = kh_get(m32, hash->h, key);
    if (k == kh_end(hash->h)) {
        return 0;
    }
    return 1;
}

void uh_int_destroy(uhash_int_t* hash) {
    kh_destroy(m32, hash->h);
}



uhash_str_t* uh_str_init() {
    uhash_str_t *hash = (uhash_str_t*)malloc(sizeof(uhash_str_t));
    hash->h = kh_init(str);
    return hash;
}

int uh_str_put(uhash_str_t* hash, char* key, char* value) {
//    int absent;
    int ret;
    khint_t k;
    
//    k = kh_get(khStrStr, hash->h, key);
    while (hash->available > 0);
    hash->available++;
    
//    if (k == kh_end(hash->h)) {
//        k = kh_put(khStrStr, hash->h, key, &absent);  // insert a key to the hash table
//        if (!absent) { 
//            kh_del(khStrStr, hash->h, k);
//            hash->available--;
//            return 1;
//        }
//    }
//    kh_value(hash->h, k) = value;
    
    ret = kh_set(str, hash->h, key, value);
    hash->available--;
    if (ret < 0) {
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
    kh_destroy(str, hash->h);
}