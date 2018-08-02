#ifndef DS_NODE_DS_NODE_H_
#include "node/ds_node.h"
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*all operations*/
static Node* Node_Constructor(long long int key);
static void Node_Destructor(void* node);
static int Node_Cmp(const void* node1, const void* node2);

NodeOperations Node_Operations={
    .constructor = Node_Constructor,
    .destructor = Node_Destructor,
    .cmp = Node_Cmp
};

static Node*
Node_Constructor(long long int key){
    Node* node = (Node*)malloc(sizeof(Node));
    memset(node, 0, sizeof(Node));
    node->key_ = key;
    return node;
}

static void
Node_Destructor(void* node){
    free(node);
}

static int Node_Cmp(const void *node1, const void *node2){
    if(((Node*)node1) -> key_ > ((Node*)node2) -> key_)
        return 1;
    else if(((Node*)node1) -> key_ == ((Node*)node2) -> key_)
        return 0;
    else
        return -1;
}