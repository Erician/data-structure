#ifndef DS_BST_DS_BST_H_
#include "bst/ds_bst.h"
#endif

#include <stdio.h>

/*All operations on bst*/
static void Bst_Init(Bst *bst);
static void Bst_Insert(Bst* bst, void* node, \
                       int (*cmp)(const void* node1, const void* node2));
/*op means operation on all nodes*/
static void Bst_Inorder_Walk(const Bst* bst, void (*op)(void* node));
/*if found, return a pointer pointing the node with the same key_, or NULL*/
static void* Bst_Find(const Bst* bst, const void* node, \
                      int (*cmp)(const void* node1, const void* node2));
static void* Bst_Maximum(const Bst* bst);
static void* Bst_Minimum(const Bst* bst);
/*when node is biggest, its successor does not exist, and return NULL*/
static void* Bst_Successor(const Bst* bst, const void* node, \
                           int (*cmp)(const void* node1, const void* node2));
static void* Bst_Predecessor(const Bst* bst, const void* node, \
                             int (*cmp)(const void* node1, const void* node2));
/*delete the first BstNode that is found and free it, if node_destructor
**is not NULL, free the node_ too.
*/
static void Bst_Erase(Bst* bst, const void* node, \
                       int (*cmp)(const void* node1, const void* node2), \
                       void (*node_destructor)(void* node));
static void* Bst_Inorder_Next(Bst* bst, int is_start_from_smallest);

static long long int Bst_Size(const Bst* bst);

const BstOperations Bst_Operations = {
    .init = Bst_Init,
    .insert = Bst_Insert,
    .size = Bst_Size,
    .inorder_walk = Bst_Inorder_Walk,
    .find = Bst_Find,
    .maximum = Bst_Maximum,
    .minimum = Bst_Minimum,
    .successor = Bst_Successor,
    .predecessor = Bst_Predecessor,
    .erase = Bst_Erase,
    .inorder_next = Bst_Inorder_Next,
};

static void 
Bst_Init(Bst *bst){
    bst -> root_ = NULL;
    bst -> size_ = 0;
}

static void 
Bst_Insert(Bst* bst, void* node, \
           int (*cmp)(const void* node1, const void* node2)){
    bst -> size_ += 1;
    BstNode *bst_node = BstNode_Operations.constructor(node);
    if(bst->root_ == NULL){
        bst->root_ = bst_node;
        return ;
    }

    BstNode *current, *parent;
    current = parent = bst->root_;
    while(current != NULL){
        parent = current;
        if(cmp(current->node_, node) >= 0)
            current = current->left_;
        else
            current = current->right_;
    }
    bst_node -> parent_ = parent;
    if(cmp(parent->node_, node)>=0)
        parent->left_ = bst_node;
    else
        parent->right_ = bst_node;
}

static long long int 
Bst_Size(const Bst* bst){
    return bst->size_;
}

static void 
Recursive_Inorder_Walk(const BstNode* bst_node, void (*op)(void* node)){
    if(bst_node != NULL){
        Recursive_Inorder_Walk(bst_node->left_, op);
        op(bst_node->node_);
        Recursive_Inorder_Walk(bst_node->right_, op);
    }
}

static void 
Bst_Inorder_Walk(const Bst* bst, void (*op)(void* node)){
    Recursive_Inorder_Walk(bst->root_, op);
}

static BstNode*
Find(BstNode* bst_node, const void* node, \
     int (*cmp)(const void* node1, const void* node2)){
    int cmp_result = 0;
    BstNode* current = bst_node;
    while(current != NULL){
        cmp_result = cmp(node, current->node_);
        if( cmp_result == 0){
            return current;
        }else if(cmp_result > 0){
            current = current -> right_;
        }else{
            current = current -> left_;
        }
    }
    return NULL;
}
static void* 
Bst_Find(const Bst* bst, const void* node, \
         int (*cmp)(const void* node1, const void* node2)){
    BstNode* found = Find(bst->root_, node, cmp);
    return found == NULL ? NULL : found->node_;
}

static BstNode*
Maximum(BstNode* current){
    if(current == NULL)
        return NULL;
    while(current->right_ != NULL)
        current = current->right_;
    return current;
}
static void*
Bst_Maximum(const Bst *bst){
    return Maximum(bst->root_)->node_;
}

static BstNode*
Minimum(BstNode *current){
    if(current == NULL)
        return NULL;
    while(current->left_ != NULL)
        current = current->left_;
    return current;
}
static void*
Bst_Minimum(const Bst *bst){
    return Minimum(bst->root_)->node_;
}

static BstNode*
Successor(const Bst* bst, const BstNode* bst_node){
    if(bst_node == NULL)
        return NULL;
    if(bst_node->right_ != NULL)
        return Minimum(bst_node->right_);
    BstNode* parent = bst_node->parent_;
    while(parent != NULL && bst_node == parent->right_){
        bst_node = parent;
        parent = bst_node -> parent_;
    }
    return parent == NULL ? parent : parent;
}
static void* 
Bst_Successor(const Bst* bst, const void* node, \
              int (*cmp)(const void* node1, const void* node2)){
    BstNode* successor = Successor(bst, Find(bst->root_, node, cmp));
    return successor != NULL ? successor->node_ : successor;
}

static void* 
Bst_Predecessor(const Bst* bst, const void* node, \
                int (*cmp)(const void* node1, const void* node2)){
    BstNode* bst_node = Find(bst->root_, node, cmp);
    if(bst_node == NULL)
        return NULL;
    if(bst_node -> left_ != NULL)
        return Maximum(bst_node->left_)->node_;
    BstNode* parent = bst_node->parent_;
    while(parent != NULL &&bst_node == parent->left_){
        bst_node = parent;
        parent = bst_node -> parent_;
    }
    return parent == NULL ? parent : parent->node_;
}

static void 
Transplant(Bst* bst, BstNode* old, BstNode* new){
    if(old->parent_ == NULL)
        bst->root_ = new;
    else if(old->parent_->left_ == old)
        old->parent_->left_ = new;
    else
        old->parent_->right_ = new;
    if(new != NULL)
        new->parent_ = old->parent_;
}

static void
Bst_Erase(Bst* bst, const void* node, \
           int (*cmp)(const void* node1, const void* node2), \
           void (*node_destructor)(void* node)){
    BstNode* bst_node = Find(bst->root_, node, cmp);
    if(bst_node == NULL)
        return ;
    if(bst_node->left_ == NULL)
        Transplant(bst, bst_node, bst_node->right_);
    else if(bst_node->right_ == NULL)
        Transplant(bst, bst_node, bst_node->left_);
    else{
        BstNode* successor = Minimum(bst_node->right_);
        if(bst_node->right_ != successor){
            Transplant(bst, successor, successor->right_);
            successor->right_ = bst_node->right_;
            successor->right_->parent_ = successor;
        }
        Transplant(bst, bst_node, successor);
        successor->left_ = bst_node->left_;
        successor->left_->parent_ = successor;
    }
    if(node_destructor != NULL){
        node_destructor(bst_node->node_);
    }
    BstNode_Operations.destructor(bst_node);
    bst->size_ -= 1;
}

static void* 
Bst_Inorder_Next(Bst* bst, int is_start_from_smallest){
    
    if(is_start_from_smallest > 0){
        bst->ptr_ = Minimum(bst->root_);
    }
    if(bst->ptr_ == NULL)
        return NULL;
    void *return_value = bst->ptr_->node_;
    bst->ptr_ = Successor(bst, bst->ptr_);
    return return_value;
}










