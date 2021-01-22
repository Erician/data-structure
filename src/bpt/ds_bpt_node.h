#ifndef DS_BPT_DS_BPT_NODE_H_
#define DS_BPT_DS_BPT_NODE_H_

#include <stdbool.h>

/* Default order is 3. */
#define ORDER 3

/* http://www.amittai.com/prose/bpt.c */

/*to make the TreeNode more common, the node is void* type
**pointing to a struct. To compare node1 and node2, there 
**is a function: cmp_node(node1, node2), and the following
**are the meanings of return values:
**  >0   node1 > node2
**  0    node1 == node2
**  <0   node1 < node2  
**/
/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * The heart of the node is the array
 * of keys and the array of corresponding
 * pointers.  The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs.  The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 * The num_keys field is used to keep
 * track of the number of valid keys.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 * In a leaf, the number of valid pointers
 * to data is always num_keys.  The
 * last leaf pointer points to the next leaf.
 */
typedef struct BptNode{
    void *pointers_[ORDER];
	void *keys_[ORDER-1];
	struct BptNode* parent_;
	bool is_leaf_;
	int num_keys_;
}BptNode;

typedef struct BptNodeOperations{
    BptNode* (*constructor)(void);
    void (*destructor)(BptNode* bpt_node);
}BptNodeOperations;

extern const BptNodeOperations BptNode_Operations;


#endif