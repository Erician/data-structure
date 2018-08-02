#ifndef DS_BST_DS_BST_NODE_H_
#define DS_BST_DS_BST_NODE_H_

/*to make the TreeNode more common, the node is void* type
**pointing to a struct. To compare node1 and node2, there 
**is a function: cmp_node(node1, node2), and the following
**are the meanings of return values:
**  >0   node1 > node2
**  0    node1 == node2
**  <0   node1 < node2  
**/
typedef struct BstNode{
    void *node_;                 
    struct BstNode *left_;
    struct BstNode *right_;
    struct BstNode *parent_;
}BstNode;

typedef struct BstNodeOperations{
    BstNode* (*constructor)(void *node);
    void (*destructor)(BstNode* bst_node);
}BstNodeOperations;

extern const BstNodeOperations BstNode_Operations;

#endif