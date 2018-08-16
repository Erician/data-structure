#ifndef DS_RBT_DS_RBT_NODE_H_
#include "rbt/ds_rbt_node.h"
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*all operations on RbtNode*/
static RbtNode* RbtNode_Constructor(void* node);
static void RbtNode_Destructor(RbtNode* rbt_node);

const RbtNodeOperations RbtNode_Operations = {
    .constructor = RbtNode_Constructor,
    .destructor = RbtNode_Destructor,
};

static RbtNode* 
RbtNode_Constructor(void* node){
    RbtNode *rbt_node = (RbtNode*)malloc(sizeof(RbtNode));
    memset(rbt_node, 0, sizeof(RbtNode));
    rbt_node->node_ = node;
    return rbt_node;
}

static void 
RbtNode_Destructor(RbtNode* rbt_node){
    free(rbt_node);
}

/*all const of RbtNode*/
const RbtNodeConst RbtNode_Const = {
    .Color_.BLACK_ = 0,
    .Color_.RED_ = 1,
};