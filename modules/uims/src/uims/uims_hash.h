#ifndef __UIMS_HASH_H__
#define __UIMS_HASH_H__

#include <uxlib/ux_rbtree.h>
#include <uxlib/ux_memory.h>

/**
 * @ingroup uims hash
 * @file
 * @brief uims hash table interface 
 */

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// uims hash table structure
////////////////////////////////////////////////////////////////////////////////

/**@brief hash table node�� ����ü */
typedef struct uims_hnode_s uims_hnode_t;
/**@brief hash table node�� ����ü*/
struct uims_hnode_s {
	ux_rbnode_t rbnode; /**< rbtree node */ 
	int         hash;  /**< node�� hash �� */
	void       *key;   /**< node�� key �� */
};

typedef struct uims_hrbtree_s uims_hrbtree_t; 
struct uims_hrbtree_s {
	ux_mutex_t mutex[1];
	ux_rbtree_t rbtree[1];
};

/**@brief hash table ����ü*/
typedef struct uims_hash_s uims_hash_t; 
/**@brief hash table ����ü*/
struct uims_hash_s {
	int          nbins; /**< hash table ���ο� ���Ǵ� �迭�� ũ�� */
	ux_compare_f cfunc; /**< node�� key�� ���ϱ� ���� �Լ��� ������ */
	ux_hash_f    hfunc; /**< node�� hash ���� ���ϱ� ���� �Լ��� ������ */
	ux_mem_t     *allocator; /**< ���� �޸� ������ ����� allocator */
	uims_hrbtree_t *bins; /**< node�� ����� hash table������ �迭 */
};

////////////////////////////////////////////////////////////////////////////////
// uims hash functions
////////////////////////////////////////////////////////////////////////////////

ux_status_t uims_hash_init(uims_hash_t* hash, int nbins, ux_compare_f cfunc,
				 ux_hash_f hfunc, ux_mem_t *allocator);
void uims_hash_final(uims_hash_t* hash);

ux_status_t uims_hash_insert(uims_hash_t *hash, void* key, uims_hnode_t *node);
uims_hnode_t* uims_hash_find(uims_hash_t *hash, void* key);
uims_hnode_t* uims_hash_remove(uims_hash_t *hash, void* key);
void uims_hash_remove_node(uims_hash_t *hash, uims_hnode_t* node);

void uims_hash_clear(uims_hash_t *hash);
int uims_hash_nbins(uims_hash_t *hash);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __UIMS_HASH_H__
