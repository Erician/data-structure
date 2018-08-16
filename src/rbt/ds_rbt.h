#ifndef DS_RBT_DS_RBT_H_
#define DS_RBT_DS_RBT_H_

#ifndef DS_RBT_DS_RBT_NODE_H_
#include "rbt/ds_rbt_node.h"
#endif

/*RBT
**NOTE: (1) the key should be unique
**      (2) not thread safe
*/
typedef struct Rbt{
    RbtNode* root_;
    RbtNode* nil_;          /*instead of NULL, remember No funs return nil*/
    RbtNode* ptr_;          /*used as iterator*/
    long long int size_;
}Rbt;

/*Put all the method of Rbt to a struct, then define a variable
**of RbtOperations with which to operate the Rbt nodes.
*/
typedef struct RbtOperations{
    void (*init)(Rbt *rbt);
    void (*insert)(Rbt* rbt, void *node, \
                   int (*cmp)(const void* node1, const void* node2)); 
    long long int (*size)(const Rbt* rbt);
    void (*inorder_walk)(const Rbt* rbt, void (*op)(void* node));
    void* (*find)(const Rbt* rbt, const void* node, \
                 int (*cmp)(const void* node1, const void* node2));
    void* (*maximum)(const Rbt* rbt);
    void* (*minimum)(const Rbt* rbt);
    void* (*successor)(const Rbt* rbt, const void* node, \
                    int (*cmp)(const void* node1, const void* node2));
    void* (*predecessor)(const Rbt* rbt, const void* node, \
                     int (*cmp)(const void* node1, const void* node2));
    void (*erase)(Rbt* rbt, const void* node, \
                    int (*cmp)(const void* node1, const void* node2), \
                    void (*node_destructor)(void* node));
    void* (*inorder_next)(Rbt* rbt, int is_start_from_smallest);
    
}RbtOperations;
extern const RbtOperations Rbt_Operations;


#endif

