#ifndef DS_BST_DS_BST_NODE_H_
#include "bst/ds_bst_node.h"
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*all operations on BstNode*/
static BstNode* BstNode_Constructor(void* node);
static void BstNode_Destructor(BstNode* bst_node);

const BstNodeOperations BstNode_Operations = {
    .constructor = BstNode_Constructor,
    .destructor = BstNode_Destructor,
};

static BstNode* 
BstNode_Constructor(void* node){
    BstNode *bst_node = (BstNode*)malloc(sizeof(BstNode));
    memset(bst_node, 0, sizeof(BstNode));
    bst_node->node_ = node;
    return bst_node;
}

static void 
BstNode_Destructor(BstNode* bst_node){
    free(bst_node);
}
