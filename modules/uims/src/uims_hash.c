#include "uims/uims_hash.h"

////////////////////////////////////////////////////////////////////////////////
// functions for uims_hash_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief hash-table을 초기화한다.
 * @param hash hash table의 포인터
 * @param nbins hash table에서 사용할 배열의 크기
 * @param cfunc hash table에 등록되는 node들을 비교하는 함수의 포인터
 * @param hfunc hash table에 등록되는 node의 hash 값을 구하는 함수.
 * @param allocator 내부적으로 배열을 할당, 재할당 하기 위해 사용되는 memory allocator
 * @return 성공 여부,
 *         UX_SUCCESS: 초기화 성공 
 *         UX_EINVAL : 만약 hash, cfunc, hfunc가 NULL 또는 nbins <=0 이면
 *         UX_EALREADY : 이미 초기화를 했다면
 *         UX_ENOMEM : 만약 내부 사용 배열 할당에 성공하지 못했으면
 */
ux_status_t uims_hash_init(uims_hash_t* hash, int nbins, ux_compare_f cfunc,
				 ux_hash_f hfunc, ux_mem_t *allocator)
{
	int i;

	if( !hash || nbins <= 0 || !cfunc || !hfunc) return UX_EINVAL;
	if( hash->bins != NULL) return UX_EALREADY;

	hash->nbins = nbins;
	hash->cfunc = cfunc;
	hash->hfunc = hfunc;
	hash->allocator = (allocator) ? ux_mem_ref(allocator) : ux_mem_default_ref();
	hash->bins = ux_alloc( hash->allocator, hash->nbins * sizeof(uims_hrbtree_t));
	if( !hash->bins) {
		ux_mem_unref(hash->allocator);
		return UX_ENOMEM;
	}
	for( i = 0; i < hash->nbins; ++i ) {
		ux_rbtree_init( &hash->bins[i], hash->cfunc);
		ux_mutex_init( &hash->mutex[i], NULL);
	}

	return UX_SUCCESS;
}

/**
 * @brief hash table의 사용을 종료한다.
 * @param hash hash table의 포인터
 */
void uims_hash_final(uims_hash_t* hash) {
	ux_mem_t *allocator = hash->allocator;

	ux_free( allocator, hash->bins);
	hash->bins = NULL;
	hash->allocator = NULL;
	ux_mem_unref( allocator);
}


/**
 * @internal
 */
static ux_rbtree_t* _uims_hash_find_node(uims_hash_t* hash, int hashval, void* key, uims_hnode_t **fnode)
{
	int idx = (hashval & 0x7FFFFFFF) % hash->nbins;
	ux_rbtree_t *troot = &hash->bins[idx];
	uims_hnode_t *hnode;

	hnode = (uims_hnode_t*)ux_rbtree_find( troot, key);
	if( hnode->hash == hashval) *fnode = hnode;
	else *fnode = NULL;
	return troot;
}

/**
 * @brief hash table에서 전달된 키와 매칭되는 node를 반환한다.
 * @param hash hash table의 포인터
 * @param key 찾고자하는 노드의 키값.
 * @return 매칭되는 hash node 만약 매칭되는 값이 없으면 NULL
 */
uims_hnode_t* uims_hash_find(uims_hash_t *hash, void* key)
{
	int hashval;
	uims_hnode_t* fnode;

	hashval = hash->hfunc(key) & 0x7FFFFFFF;
	_uims_hash_find_node(hash, hashval, key, &fnode);

	return fnode;
}

/**
 * @brief hash table에 전달된 key로 새로운 hash node를 추가한다.
 * @param hash hash table의 포인터
 * @param key 추가하고자 하는 node의 key
 * @param node hash table에 추가하고자 하는 node
 * @return 성공 여부, 기존에 해당 키의 데이터가 이미 존재한다면, UX_EEXIST 반환
 *        성공일 경우 UX_SUCCESS 반환
 */
ux_status_t uims_hash_insert(uims_hash_t *hash, void* key, uims_hnode_t *node)
{
	ux_rbtree_t *troot;
	uims_hnode_t *fnode;

	// TODO : get idx & lock, unlock function ...
	
	node->key = key;
	node->hash = hash->hfunc(node->key) & 0x7FFFFFFF;
	troot = _uims_hash_find_node(hash, node->hash, node->key, &fnode);
	if( fnode != NULL ) return UX_EEXIST;

	ux_rbtree_insert(troot, node->key, &node->rbnode);

	return UX_SUCCESS;
}

/**
 * @brief hash table로 부터 전달된 key의 node를 삭제한다.
 * @param hash hash table의 포인터
 * @param key 삭제하고자 하는 node의 key
 * @return 삭제된 node를 반환한다. 해당 키의 데이터가 없다면 NULL을 반환한다.
 */
uims_hnode_t* uims_hash_remove(uims_hash_t *hash, void* key)
{
	int hashval;
	ux_rbtree_t *troot;
	uims_hnode_t *fnode;
	
	hashval = hash->hfunc(key) & 0x7FFFFFFF;
	troot = _uims_hash_find_node(hash, hashval, key, &fnode);
	if( fnode != NULL ) {
		ux_rbtree_remove_node( troot, &fnode->rbnode);
	}

	return fnode;
}

/**
 * @brief hash table로 부터 전달된 node를 삭제한다.
 * @param hash hash table의 포인터
 * @param node 삭제하고자 하는 node
 */
void uims_hash_remove_node(uims_hash_t *hash, uims_hnode_t* node)
{
	int idx;
	ux_rbtree_t *troot;

	if( node == NULL) return;
	idx = node->hash % hash->nbins;
	troot = &hash->bins[idx];
	ux_rbtree_remove_node( troot, &node->rbnode);
}

/**
 * @brief hash table의 모든 노드를 삭제한다.
 * @param hash hash table의 포인터
 */
void uims_hash_clear(uims_hash_t *hash)
{
	int i;

	for( i = 0; i < hash->nbins; ++i ) {
		ux_rbtree_clear(&hash->bins[i]);
		ux_mutex_final( &hash->mutex[i]);
	}
}

/**
 * @brief hash table에서 사용하는 내부 배열의 크기를 반환한다.
 * @param hash hash table의 포인터
 * @return hash table에서 사용하는 내부 배열의 크기
 */
int uims_hash_nbins(uims_hash_t *hash)
{
	return hash->nbins;
}

