#ifndef DS_BST_DS_BST_H_
#define DS_BST_DS_BST_H_

#ifndef DS_BST_DS_BST_NODE_H_
#include "bst/ds_bst_node.h"
#endif

/*BST
**NOTE: (1) the key should be unique
**      (2) not thread safe
*/
typedef struct Bst{
    BstNode* root_;
    BstNode* ptr_;         /*used as iterator*/
    long long int size_;
}Bst;

/*Put all the method of bst to a struct, then define a variable
**of BstOperations with which to operate the bst nodes.
*/
typedef struct BstOperations{
    void (*init)(Bst *bst);
    void (*insert)(Bst* bst, void *node, \
                   int (*cmp)(const void* node1, const void* node2));
    long long int (*size)(const Bst* bst);
    void (*inorder_walk)(const Bst* bst, void (*op)(void* node));
    void* (*find)(const Bst* bst, const void* node, \
                 int (*cmp)(const void* node1, const void* node2));
    void* (*maximum)(const Bst* bst);
    void* (*minimum)(const Bst* bst);
    void* (*successor)(const Bst* bst, const void* node, \
                    int (*cmp)(const void* node1, const void* node2));
    void* (*predecessor)(const Bst* bst, const void* node, \
                     int (*cmp)(const void* node1, const void* node2));
    void (*erase)(Bst* bst, const void* node, \
                    int (*cmp)(const void* node1, const void* node2), \
                    void (*node_destructor)(void* node));
    void* (*inorder_next)(Bst* bst, int is_start_from_smallest);
    
}BstOperations;



extern const BstOperations Bst_Operations;





#endif

