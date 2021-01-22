#ifndef DS_BST_DS_BPT_NODE_H_
#include "bpt/ds_bpt_node.h"
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*all operations on BstNode*/
static BptNode* BptNode_Constructor(void);
static void BptNode_Destructor(BptNode* bst_node);

const BptNodeOperations BptNode_Operations = {
    .constructor = BptNode_Constructor,
    .destructor = BptNode_Destructor,
};

static BptNode* 
BptNode_Constructor(void){
    BptNode *bpt_node = (BptNode*)malloc(sizeof(BptNode));
    memset(bpt_node, 0, sizeof(BptNode));
    bpt_node->is_leaf_ = false;
    bpt_node->num_keys_ = 0;
    bpt_node->parent_ = NULL;
    return bpt_node;
}

static void 
BptNode_Destructor(BptNode* bpt_node){
    free(bpt_node);
}