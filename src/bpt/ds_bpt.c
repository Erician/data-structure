#ifndef DS_BPT_DS_BPT_H_
#include "bpt/ds_bpt.h"
#endif

#include <stdio.h>

/*All operations on bpt*/
static void Bpt_Init(Bpt *bpt);
static void Bpt_Insert(Bpt* bpt, void* key, \
                       int (*cmp)(const void* node1, const void* node2));
/*if found, return a pointer pointing the node with the same key_, or NULL*/
static void* Bpt_Find(const Bpt* bpt, const void* key, \
                      int (*cmp)(const void* node1, const void* node2));
/*delete the first BptNode that is found and free it, if node_destructor
**is not NULL, free the node_ too.
*/
static void Bpt_Erase(Bpt* bpt, void* key, \
                       int (*cmp)(const void* node1, const void* node2), \
                       void (*node_destructor)(void* node));
static long long int Bpt_Size(const Bpt* bpt);

static void insert_into_parent(Bpt *bpt, BptNode *left, void *key, BptNode *right);
static void insert_into_parent(Bpt *bpt, BptNode *left, void *key, BptNode *right);
static void delete_entry(Bpt *bpt, BptNode *n, void *key, void *pointer, \
                         int (*cmp)(const void* node1, const void* node2));


const BptOperations Bpt_Operations = {
    .init = Bpt_Init,
    .insert = Bpt_Insert,
    .find = Bpt_Find,
    .erase = Bpt_Erase,
    .size = Bpt_Size,
};

static void 
Bpt_Init(Bpt *bpt){
    bpt -> root_ = NULL;
}

long long int 
Bpt_Size(const Bpt* bpt) {
    long long int ret = 0;
    int i;

    if (bpt->root_ == NULL)
        return 0;
    BptNode *cur = bpt->root_;
    while (!cur->is_leaf_) {
        cur = cur->pointers_[0];
    }
    while (cur != NULL) {
        ret += cur->num_keys_;
        cur = cur->pointers_[ORDER-1];
    }
    return ret;
}

static BptNode *
find_leaf(const Bpt *bpt, const void *key, \
          int (*cmp)(const void* node1, const void* node2)) {
    if (bpt->root_ == NULL) {
        printf("ERROR: empty tree\n");
        return NULL;
    }

    int i;
    BptNode *c = bpt->root_;
    while (!c->is_leaf_) {
        i = 0;
        while (i < c->num_keys_) {
            if (cmp(key, c->keys_[i]) >= 0)
                i++;
            else
                break;
        }
        c = c->pointers_[i];
    }
    return c;
}

/* Finds the appropriate place to
 * split a node that is too big into two.
 */
int cut(int length) {
	if (length % 2 == 0)
		return length/2;
	else
		return length/2 + 1;
}

static void
insert_into_node_after_splitting(Bpt *bpt, BptNode *old_node, int left_index, \
    void *key, BptNode *right) {
    int i, j, split;
    void *k_prime;
    BptNode *new_node, *child;
    void *temp_keys[ORDER];
    void *temp_pointers[ORDER + 1];

    for (i = 0, j = 0; i < old_node->num_keys_ + 1; i++, j++) {
        if (j == left_index + 1)
            j++;
        temp_pointers[j] = old_node->pointers_[i];
    }

    for (i = 0, j = 0; i < old_node->num_keys_; i++, j++) {
        if (j == left_index)
            j++;
        temp_keys[j] = old_node->keys_[i];
    }

    temp_pointers[left_index + 1] = right;
    temp_keys[left_index] = key;

    split = cut(ORDER);
    new_node = BptNode_Operations.constructor();
    old_node->num_keys_ = 0;
    for (i = 0; i < split - 1; i++) {
        old_node->pointers_[i] = temp_pointers[i];
        old_node->keys_[i] = temp_keys[i];
        old_node->num_keys_++;
    }
    old_node->pointers_[i] = temp_pointers[i];
    k_prime = temp_keys[split - 1];
    for (++i, j=0; i < ORDER; i++, j++) {
        new_node->pointers_[j] = temp_pointers[i];
        new_node->keys_[j] = temp_keys[i];
        new_node->num_keys_++;
    }
    new_node->pointers_[j] = temp_pointers[i];
    new_node->parent_ = old_node->parent_;

    for (i = 0; i <= new_node->num_keys_; i++) {
        child = new_node->pointers_[i];
        child->parent_ = new_node;
    }

    return insert_into_parent(bpt, old_node, k_prime, new_node);
}

static void
insert_into_parent(Bpt *bpt, BptNode *left, void *key, BptNode *right) {
    int left_index;
    BptNode *parent;

    parent = left->parent_;
    if (parent == NULL) {
        bpt->root_ = BptNode_Operations.constructor();
        bpt->root_->keys_[0] = key;
        bpt->root_->pointers_[0] = left;
        bpt->root_->pointers_[1] = right;
        bpt->root_->num_keys_++;
        bpt->root_->parent_ = NULL;
        left->parent_ = bpt->root_;
        right->parent_ = bpt->root_;
        return ;
    }

    left_index = 0;
    while (left_index <= parent->num_keys_ && \
           parent->pointers_[left_index] != left)
        left_index++;
    
    if (parent->num_keys_ < ORDER - 1) {
        int i;
        for (i = parent->num_keys_; i > left_index; i--) {
            parent->pointers_[i+1] = parent->pointers_[i];
            parent->keys_[i] = parent->keys_[i - 1];
        }
        parent->pointers_[left_index + 1] = right;
        parent->keys_[left_index] = key;
        parent->num_keys_++;
        return;
    }

    return insert_into_node_after_splitting(bpt, parent, left_index, key, right);
}

static void
insert_into_leaf_after_splitting(Bpt *bpt, BptNode *leaf, void* key, \
                                 int (*cmp)(const void* node1, const void* node2)) {
    BptNode *new_leaf;
    void *temp_keys[ORDER];
    void *temp_pointers[ORDER];
    int insertion_index, split, i, j;
    void *new_key;

    new_leaf = BptNode_Operations.constructor();
    new_leaf->is_leaf_ = true;

    insertion_index = 0;
    while (insertion_index < ORDER - 1 && cmp(key, leaf->keys_[insertion_index]) > 0) {
        insertion_index++;
    }

    for (i = 0, j = 0; i < leaf->num_keys_; i++, j++) {
        if (j == insertion_index)
            j++;
        temp_keys[j] = leaf->keys_[i];
        temp_pointers[j] = leaf->pointers_[i];
    }

    temp_keys[insertion_index] = key;
    temp_pointers[insertion_index] = key;

    leaf->num_keys_ = 0;
    split = cut(ORDER - 1);

    for (i = 0; i < split; i++) {
        leaf->keys_[i] = temp_keys[i];
        leaf->pointers_[i] = temp_pointers[i];
        leaf->num_keys_++;
    }

    for ( i = split, j = 0; i < ORDER; i++, j++) {
        new_leaf->keys_[j] = temp_keys[i];
        new_leaf->pointers_[j] = temp_pointers[i];
        new_leaf->num_keys_++;
    }

    new_leaf->pointers_[ORDER - 1] = leaf->pointers_[ORDER - 1];
    leaf->pointers_[ORDER - 1] = new_leaf;

    new_leaf->parent_ = leaf->parent_;
    new_key = new_leaf->keys_[0];

    return insert_into_parent(bpt, leaf, new_key, new_leaf);
}

static void 
Bpt_Insert(Bpt* bpt, void* key, \
           int (*cmp)(const void* node1, const void* node2)){
    if (bpt->root_ == NULL) {
        bpt->root_ = BptNode_Operations.constructor();
        bpt->root_->is_leaf_ = true;
        bpt->root_->keys_[0] = key;
        /* pointers_ should be used to store value, but we just store the key. */
        bpt->root_->pointers_[0] = key;
        bpt->root_->pointers_[ORDER - 1] = NULL;
        bpt->root_->parent_ = NULL;
        bpt->root_->num_keys_++;
        return;
    }

    BptNode *leaf = find_leaf(bpt, key, cmp);
    if (leaf->num_keys_ < ORDER - 1) {
        int i, insertion_point;

        insertion_point = 0;
        while (insertion_point < leaf->num_keys_ && cmp(key, leaf->keys_[insertion_point]) > 0) {
            insertion_point++;
        }
        /* move keys and pointers */
        for (i = leaf->num_keys_; i > insertion_point; i--) {
            leaf->keys_[i] = leaf->keys_[i - 1];
            leaf->pointers_[i] = leaf->pointers_[i - 1];
        }
        leaf->keys_[insertion_point] = key;
        leaf->pointers_[insertion_point] = key;
        leaf->num_keys_++;
        return;
    }

    /* leaf must be split */
    return insert_into_leaf_after_splitting(bpt, leaf, key, cmp);
}

/*if found, return a pointer pointing the node with the same key_, or NULL*/
static void*
Bpt_Find(const Bpt* bpt, const void* key, \
        int (*cmp)(const void* node1, const void* node2)) {
    if (bpt->root_ == NULL) {
        printf("ERROR: empty tree\n");
        return NULL;
    }

    int i = 0;
    BptNode *leaf = NULL;

    leaf = find_leaf(bpt, key, cmp);
    for (i = 0; i < leaf->num_keys_; i++) {
        if (cmp(key, leaf->keys_[i]) == 0)
            break;
    }
    if (i == leaf->num_keys_)
        return NULL;
    else
        return leaf->pointers_[i];
}

static void
remove_entry_from_node(BptNode *n, void *key, void *pointer, \
                       int (*cmp)(const void* node1, const void* node2)) {
    int i, num_pointers;

    i = 0;
    while (cmp(key, n->keys_[i]) != 0)
        i++;
    for (++i; i < n->num_keys_; i++)
        n->keys_[i - 1] = n->keys_[i];
    num_pointers = n->is_leaf_ ? n->num_keys_ : n->num_keys_ + 1;
    i = 0;
    while (n->pointers_[i] != pointer)
        i++;
    for (++i; i < num_pointers; i++)
        n->pointers_[i-1] = n->pointers_[i];

    n->num_keys_--;

    return;
}

static void
adjust_root(Bpt *bpt) {
    if (bpt->root_->num_keys_ > 0)
        return;

    if (!bpt->root_->is_leaf_) {
        bpt->root_ = bpt->root_->pointers_[0];
        bpt->root_->parent_ = NULL;
    } else {
        bpt->root_ = NULL;
    }
}

static void
redistribute_nodes(Bpt *bpt, BptNode *n, BptNode *neighbor, int neighbor_index, 
		           int k_prime_index, void *k_prime) {
    int i;
    BptNode *tmp;

    if (neighbor_index != -1) {
        if (!n->is_leaf_)
            n->pointers_[n->num_keys_ + 1] = n->pointers_[n->num_keys_];
        for (i = n->num_keys_; i > 0; i--) {
            n->keys_[i] = n->keys_[i - 1];
            n->pointers_[i] = n->pointers_[i - 1];
        }
        if (!n->is_leaf_) {
            n->pointers_[0] = neighbor->pointers_[neighbor->num_keys_];
            tmp = n->pointers_[0];
            tmp->parent_ = n;
            neighbor->pointers_[neighbor->num_keys_] = NULL;
            n->keys_[0] = k_prime;
            n->parent_->keys_[k_prime_index] = neighbor->keys_[neighbor->num_keys_ - 1];
        } else {
            n->pointers_[0] = neighbor->pointers_[neighbor->num_keys_ - 1];
            neighbor->pointers_[neighbor->num_keys_ - 1] = NULL;
            n->keys_[0] = neighbor->keys_[neighbor->num_keys_ - 1];
            n->parent_->keys_[k_prime_index] = n->keys_[0];
        }
    } else {
        if (n->is_leaf_) {
			n->keys_[n->num_keys_] = neighbor->keys_[0];
			n->pointers_[n->num_keys_] = neighbor->pointers_[0];
			n->parent_->keys_[k_prime_index] = neighbor->keys_[1];
		} else {
			n->keys_[n->num_keys_] = k_prime;
			n->pointers_[n->num_keys_ + 1] = neighbor->pointers_[0];
			tmp = n->pointers_[n->num_keys_ + 1];
			tmp->parent_ = n;
			n->parent_->keys_[k_prime_index] = neighbor->keys_[0];
		}
		for (i = 0; i < neighbor->num_keys_ - 1; i++) {
			neighbor->keys_[i] = neighbor->keys_[i + 1];
			neighbor->pointers_[i] = neighbor->pointers_[i + 1];
		}
		if (!n->is_leaf_)
			neighbor->pointers_[i] = neighbor->pointers_[i + 1];
    }
    n->num_keys_++;
    neighbor->num_keys_--;

    return;
}

static void
coalesce_nodes(Bpt *bpt, BptNode *n, BptNode *neighbor, int neighbor_index, void *k_prime, \
               int (*cmp)(const void* node1, const void* node2)) {
    int i, j, neighbor_insertion_index, n_end;
    BptNode *tmp;

    if (neighbor_index == -1) {
		tmp = n;
		n = neighbor;
		neighbor = tmp;
	}

    neighbor_insertion_index = neighbor->num_keys_;
    if (!n->is_leaf_) {
		neighbor->keys_[neighbor_insertion_index] = k_prime;
		neighbor->num_keys_++;

		n_end = n->num_keys_;

		for (i = neighbor_insertion_index + 1, j = 0; j < n_end; i++, j++) {
			neighbor->keys_[i] = n->keys_[j];
			neighbor->pointers_[i] = n->pointers_[j];
			neighbor->num_keys_++;
			n->num_keys_--;
		}

		neighbor->pointers_[i] = n->pointers_[j];

		for (i = 0; i < neighbor->num_keys_ + 1; i++) {
			tmp = neighbor->pointers_[i];
			tmp->parent_ = neighbor;
		}
	} else {
		for (i = neighbor_insertion_index, j = 0; j < n->num_keys_; i++, j++) {
			neighbor->keys_[i] = n->keys_[j];
			neighbor->pointers_[i] = n->pointers_[j];
			neighbor->num_keys_++;
		}
		neighbor->pointers_[ORDER - 1] = n->pointers_[ORDER - 1];
	}

	return delete_entry(bpt, n->parent_, k_prime, n, cmp);
}

static void
delete_entry(Bpt *bpt, BptNode *n, void *key, void *pointer,\
             int (*cmp)(const void* node1, const void* node2)) {
    int min_keys;
    BptNode *neighbor;
    int neighbor_index;
    int k_prime_index;
    void *k_prime;
    int capacity;
    int i;

    remove_entry_from_node(n, key, pointer, cmp);

    if (n == bpt->root_)
        return adjust_root(bpt);

    /* check key num */
    min_keys = n->is_leaf_ ? cut(ORDER - 1) : cut(ORDER) - 1;
    if (n->num_keys_ >= min_keys)
        return;
    
    /* get neighbot index */
    for (i = 0; i <= n->parent_->num_keys_; i++) {
        if (n->parent_->pointers_[i] == n) {
            neighbor_index = i - 1;
            break;
        }
    }
    k_prime_index = neighbor_index == -1 ? 0 : neighbor_index;
    k_prime = n->parent_->keys_[k_prime_index];
    neighbor = neighbor_index == -1 ? n->parent_->pointers_[1] : n->parent_->pointers_[neighbor_index];

    capacity = n->is_leaf_ ? ORDER : ORDER - 1;

    if (neighbor->num_keys_ + n->num_keys_ < capacity) {
        return coalesce_nodes(bpt, n, neighbor, neighbor_index, k_prime, cmp);
    } else {
        return redistribute_nodes(bpt, n, neighbor, neighbor_index, k_prime_index, k_prime);
    }
}

static void
Bpt_Erase(Bpt* bpt, void* key, \
          int (*cmp)(const void* node1, const void* node2), \
          void (*node_destructor)(void* node)) {
    BptNode *key_leaf = NULL;
    int i;

    key_leaf = find_leaf(bpt, key, cmp);
    if (key_leaf == NULL)
        return;
    for (i = 0; i < key_leaf->num_keys_; i++) {
        if (cmp(key, key_leaf->keys_[i]) == 0) {
            void *deleted_key = key_leaf->keys_[i];
            delete_entry(bpt, key_leaf, key_leaf->keys_[i], key_leaf->pointers_[i], cmp);
            node_destructor(deleted_key);
            return;
        }
    }
    return;
}