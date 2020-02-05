#include "uims/uims_hash.h"

////////////////////////////////////////////////////////////////////////////////
// functions for uims_hash_t
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief hash-table�� �ʱ�ȭ�Ѵ�.
 * @param hash hash table�� ������
 * @param nbins hash table���� ����� �迭�� ũ��
 * @param cfunc hash table�� ��ϵǴ� node���� ���ϴ� �Լ��� ������
 * @param hfunc hash table�� ��ϵǴ� node�� hash ���� ���ϴ� �Լ�.
 * @param allocator ���������� �迭�� �Ҵ�, ���Ҵ� �ϱ� ���� ���Ǵ� memory allocator
 * @return ���� ����,
 *         UX_SUCCESS: �ʱ�ȭ ���� 
 *         UX_EINVAL : ���� hash, cfunc, hfunc�� NULL �Ǵ� nbins <=0 �̸�
 *         UX_EALREADY : �̹� �ʱ�ȭ�� �ߴٸ�
 *         UX_ENOMEM : ���� ���� ��� �迭 �Ҵ翡 �������� ��������
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
 * @brief hash table�� ����� �����Ѵ�.
 * @param hash hash table�� ������
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
 * @brief hash table���� ���޵� Ű�� ��Ī�Ǵ� node�� ��ȯ�Ѵ�.
 * @param hash hash table�� ������
 * @param key ã�����ϴ� ����� Ű��.
 * @return ��Ī�Ǵ� hash node ���� ��Ī�Ǵ� ���� ������ NULL
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
 * @brief hash table�� ���޵� key�� ���ο� hash node�� �߰��Ѵ�.
 * @param hash hash table�� ������
 * @param key �߰��ϰ��� �ϴ� node�� key
 * @param node hash table�� �߰��ϰ��� �ϴ� node
 * @return ���� ����, ������ �ش� Ű�� �����Ͱ� �̹� �����Ѵٸ�, UX_EEXIST ��ȯ
 *        ������ ��� UX_SUCCESS ��ȯ
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
 * @brief hash table�� ���� ���޵� key�� node�� �����Ѵ�.
 * @param hash hash table�� ������
 * @param key �����ϰ��� �ϴ� node�� key
 * @return ������ node�� ��ȯ�Ѵ�. �ش� Ű�� �����Ͱ� ���ٸ� NULL�� ��ȯ�Ѵ�.
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
 * @brief hash table�� ���� ���޵� node�� �����Ѵ�.
 * @param hash hash table�� ������
 * @param node �����ϰ��� �ϴ� node
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
 * @brief hash table�� ��� ��带 �����Ѵ�.
 * @param hash hash table�� ������
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
 * @brief hash table���� ����ϴ� ���� �迭�� ũ�⸦ ��ȯ�Ѵ�.
 * @param hash hash table�� ������
 * @return hash table���� ����ϴ� ���� �迭�� ũ��
 */
int uims_hash_nbins(uims_hash_t *hash)
{
	return hash->nbins;
}

