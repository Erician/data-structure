#ifndef DS_RBT_DS_RBT_H_
#include "rbt/ds_rbt.h"
#endif

#include <stdio.h>
#include <malloc.h>
#include <string.h>

/*All operations on rbt*/
static void Rbt_Init(Rbt *rbt);
static void Rbt_Insert(Rbt* rbt, void* node, \
                       int (*cmp)(const void* node1, const void* node2));
/*op means operation on all nodes*/
static void Rbt_Inorder_Walk(const Rbt* rbt, void (*op)(void* node));
/*if found, return a pointer pointing the node with the same key_, or NULL*/
static void* Rbt_Find(const Rbt* rbt, const void* node, \
                      int (*cmp)(const void* node1, const void* node2));
static void* Rbt_Maximum(const Rbt* rbt);
static void* Rbt_Minimum(const Rbt* rbt);
/*when node is biggest, its successor does not exist, and return NULL*/
static void* Rbt_Successor(const Rbt* rbt, const void* node, \
                           int (*cmp)(const void* node1, const void* node2));
static void* Rbt_Predecessor(const Rbt* rbt, const void* node, \
                             int (*cmp)(const void* node1, const void* node2));
/*delete the first RbtNode that is found and free it, if node_destructor
**is not NULL, free the node_ too.
*/
static void Rbt_Erase(Rbt* rbt, const void* node, \
                       int (*cmp)(const void* node1, const void* node2), \
                       void (*node_destructor)(void* node));
static void* Rbt_Inorder_Next(Rbt* rbt, int is_start_from_smallest);

static long long int Rbt_Size(const Rbt* rbt);

const RbtOperations Rbt_Operations = {
    .init = Rbt_Init,
    .insert = Rbt_Insert,
    .size = Rbt_Size,
    .inorder_walk = Rbt_Inorder_Walk,
    .find = Rbt_Find,
    .maximum = Rbt_Maximum,
    .minimum = Rbt_Minimum,
    .successor = Rbt_Successor,
    .predecessor = Rbt_Predecessor,
    .erase = Rbt_Erase,
    .inorder_next = Rbt_Inorder_Next,
};

static void 
Rbt_Init(Rbt *rbt){
    rbt -> nil_ = (RbtNode*)malloc(sizeof(RbtNode));
    rbt -> nil_ -> node_ = NULL;
    rbt -> nil_ -> left_ = NULL;
    rbt -> nil_ -> right_ = NULL;
    rbt -> nil_ -> parent_ = NULL;
    rbt -> nil_ -> color_ = RbtNode_Const.Color_.BLACK_;
    rbt -> root_ = rbt->nil_;            
    rbt -> size_ = 0;
}

static void 
Left_Rotate(Rbt* rbt, RbtNode* x){
    RbtNode* y = x->right_;
    x->right_ = y->left_;
    if(y->left_ != rbt->nil_)
        y->left_->parent_ = x;
    y->parent_ = x->parent_;
    if(x->parent_ == rbt->nil_)
        rbt->root_ = y;
    else if(x == x->parent_->left_)
        x->parent_->left_ = y;
    else
        x->parent_->right_ = y;
    y->left_ = x;
    x->parent_ = y;
}

static void Right_Rotate(Rbt* rbt, RbtNode* y){
    RbtNode* x = y->left_;
    y->left_ = x->right_;
    if(x->right_ != rbt->nil_)
        x->right_->parent_ = y;
    x->parent_ = y->parent_;
    if(y->parent_ == rbt->nil_)
        rbt->root_ = x;
    else if(y == y->parent_->left_)
        y->parent_->left_ = x;
    else
        y->parent_->right_ = x;
    x->right_ = y;
    y->parent_ = x;
}

static void 
Insert_Fixup(Rbt* rbt, RbtNode* z){
    while(z->parent_->color_ == RbtNode_Const.Color_.RED_){
        if(z->parent_ == z->parent_->parent_->left_){
            RbtNode* y = z->parent_->parent_->right_;
            if(y->color_ == RbtNode_Const.Color_.RED_){
                z->parent_->color_ = RbtNode_Const.Color_.BLACK_;
                y->color_ = RbtNode_Const.Color_.BLACK_;
                z->parent_->parent_->color_ = RbtNode_Const.Color_.RED_;
                z = z->parent_->parent_;
            }else{ 
                if(z == z->parent_->right_){
                    z = z->parent_;
                    Left_Rotate(rbt, z);
                }
                z->parent_->color_ = RbtNode_Const.Color_.BLACK_;
                z->parent_->parent_->color_ = RbtNode_Const.Color_.RED_;
                Right_Rotate(rbt, z->parent_->parent_);
            }
        }else{
            RbtNode* y = z->parent_->parent_->left_;
            if(y->color_ == RbtNode_Const.Color_.RED_){
                z->parent_->color_ = RbtNode_Const.Color_.BLACK_;
                y->color_ = RbtNode_Const.Color_.BLACK_;
                z->parent_->parent_->color_ = RbtNode_Const.Color_.RED_;
                z = z->parent_->parent_;
            }else{
                if(z == z->parent_->left_){
                    z = z->parent_;
                    Right_Rotate(rbt, z);
                }
                z->parent_->color_ = RbtNode_Const.Color_.BLACK_;
                z->parent_->parent_->color_ = RbtNode_Const.Color_.RED_;
                Left_Rotate(rbt, z->parent_->parent_);
            }
        }
    }
    rbt->root_->color_ = RbtNode_Const.Color_.BLACK_;
}

static void 
Rbt_Insert(Rbt* rbt, void* node, \
           int (*cmp)(const void* node1, const void* node2)){
    rbt -> size_ += 1;
    RbtNode *z = RbtNode_Operations.constructor(node);
    RbtNode *current, *parent;
    parent = rbt->nil_;
    current = rbt->root_;
    while(current != rbt->nil_){
        parent = current;
        if(cmp(current->node_, node) >= 0)
            current = current->left_;
        else
            current = current->right_;
    }
    
    z->parent_ = parent;
    if(parent == rbt->nil_)
        rbt->root_ = z;
    else if(cmp(parent->node_, node)>=0)
        parent->left_ = z;
    else
        parent->right_ = z;
    z->left_ = rbt->nil_;
    z->right_ = rbt->nil_;
    z->color_ = RbtNode_Const.Color_.RED_;

    Insert_Fixup(rbt, z);
}

static long long int 
Rbt_Size(const Rbt* rbt){
    return rbt->size_;
}

static void 
Recursive_Inorder_Walk(const RbtNode* rbt_node, const RbtNode* nil, void (*op)(void* node)){
    if(rbt_node != nil){
        Recursive_Inorder_Walk(rbt_node->left_, nil, op);
        op(rbt_node->node_);
        Recursive_Inorder_Walk(rbt_node->right_, nil, op);
    }
}

static void 
Rbt_Inorder_Walk(const Rbt* rbt, void (*op)(void* node)){
    Recursive_Inorder_Walk(rbt->root_, rbt->nil_, op);
}

static RbtNode*
Find(RbtNode* rbt_node, const RbtNode* nil, const void* node, \
     int (*cmp)(const void* node1, const void* node2)){
    int cmp_result = 0;
    RbtNode* current = rbt_node;
    while(current != nil){
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
Rbt_Find(const Rbt* rbt, const void* node, \
         int (*cmp)(const void* node1, const void* node2)){
    RbtNode* found = Find(rbt->root_, rbt->nil_, node, cmp);
    return found == NULL ? NULL : found->node_;
}

static RbtNode*
Maximum(RbtNode* current, const RbtNode* nil){
    if(current == nil)
        return NULL;
    while(current->right_ != nil)
        current = current->right_;
    return current;
}
static void*
Rbt_Maximum(const Rbt *rbt){
    return Maximum(rbt->root_, rbt->nil_)->node_;
}

static RbtNode*
Minimum(RbtNode *current, const RbtNode* nil){
    if(current == nil)
        return NULL;
    while(current->left_ != nil)
        current = current->left_;
    return current;
}
static void*
Rbt_Minimum(const Rbt *rbt){
    return Minimum(rbt->root_, rbt->nil_)->node_;
}

static RbtNode*
Successor(const Rbt* rbt, RbtNode* rbt_node){
    if(rbt_node == rbt->nil_)
        return NULL;
    if(rbt_node->right_ != rbt->nil_)
        return Minimum(rbt_node->right_, rbt->nil_);
    RbtNode* parent = rbt_node->parent_;
    while(parent != rbt->nil_ && rbt_node == parent->right_){
        rbt_node = parent;
        parent = rbt_node -> parent_;
    }
    return parent == rbt->nil_ ? NULL : parent;
}

static void* 
Rbt_Successor(const Rbt* rbt, const void* node, \
              int (*cmp)(const void* node1, const void* node2)){
    RbtNode* successor = Successor(rbt, Find(rbt->root_, rbt->nil_, node, cmp));
    return successor != NULL ? successor->node_ : successor;
}

static void* 
Rbt_Predecessor(const Rbt* rbt, const void* node, \
                int (*cmp)(const void* node1, const void* node2)){
    RbtNode* rbt_node = Find(rbt->root_, rbt->nil_, node, cmp);
    if(rbt_node == NULL)
        return NULL;
    if(rbt_node -> left_ != rbt->nil_)
        return Maximum(rbt_node->left_, rbt->nil_)->node_;
    RbtNode* parent = rbt_node->parent_;
    while(parent != rbt->nil_ &&rbt_node == parent->left_){
        rbt_node = parent;
        parent = rbt_node -> parent_;
    }
    return parent == rbt->nil_ ? NULL : parent->node_;
}

static void 
Transplant(Rbt* rbt, RbtNode* old, RbtNode* new){
    if(old->parent_ == rbt->nil_)
        rbt->root_ = new;
    else if(old->parent_->left_ == old)
        old->parent_->left_ = new;
    else
        old->parent_->right_ = new;
    new->parent_ = old->parent_;
}

static void 
Erase_Fixup(Rbt* rbt, RbtNode* x){
    while(x != rbt->root_ && x->color_ == RbtNode_Const.Color_.BLACK_){
        if(x == x->parent_->left_){
            RbtNode* w = x->parent_->right_;
            if(w->color_ == RbtNode_Const.Color_.RED_){
                w->color_ = RbtNode_Const.Color_.BLACK_;
                x->parent_->color_ = RbtNode_Const.Color_.RED_;
                Left_Rotate(rbt, x->parent_);
                w = x->parent_->right_;
            }
            if(w->left_->color_ == RbtNode_Const.Color_.BLACK_ \
                && w->right_->color_ == RbtNode_Const.Color_.BLACK_){
                w->color_ = RbtNode_Const.Color_.RED_;
                x = x->parent_;
            }else{ 
                if(w->right_->color_ == RbtNode_Const.Color_.BLACK_){
                    w->left_->color_ = RbtNode_Const.Color_.BLACK_;
                    w->color_ = RbtNode_Const.Color_.RED_;
                    Right_Rotate(rbt, w);
                    w = x->parent_->right_;
                }
                w->color_ = x->parent_->color_;
                x->parent_->color_ = RbtNode_Const.Color_.BLACK_;
                w->right_->color_ = RbtNode_Const.Color_.BLACK_;
                Left_Rotate(rbt, x->parent_);
                x = rbt->root_;
            }
        }else{
            RbtNode* w = x->parent_->right_;
            if(w->color_ == RbtNode_Const.Color_.RED_){
                w->color_ = RbtNode_Const.Color_.BLACK_;
                x->parent_->color_ = RbtNode_Const.Color_.RED_;
                Right_Rotate(rbt, x->parent_);
                w = x->parent_->right_;
            }
            if(w->right_->color_ == RbtNode_Const.Color_.BLACK_\
                && w->left_->color_ == RbtNode_Const.Color_.BLACK_){
                w->color_ = RbtNode_Const.Color_.RED_;
                x = x->parent_;
            }else{
                if(w->left_->color_ == RbtNode_Const.Color_.BLACK_){
                    w->right_->color_ = RbtNode_Const.Color_.BLACK_;
                    w->color_ = RbtNode_Const.Color_.RED_;
                    Left_Rotate(rbt, w);
                    w = x->parent_->left_;
                }
                w->color_ = x->parent_->color_;
                x->parent_->color_ = RbtNode_Const.Color_.BLACK_;
                w->left_->color_ = RbtNode_Const.Color_.BLACK_;
                Right_Rotate(rbt, x->parent_);
                x = rbt->root_;
            }   
        }
    }
    x->color_ = RbtNode_Const.Color_.BLACK_;
}

static void
Rbt_Erase(Rbt* rbt, const void* node, \
           int (*cmp)(const void* node1, const void* node2), \
           void (*node_destructor)(void* node)){
    RbtNode* z = Find(rbt->root_, rbt->nil_, node, cmp), *deleted_node;
    deleted_node = z;
    if(z == NULL)
        return ;

    RbtNode *y = z, *x;
    int y_original_color = y->color_;
    if(z->left_ == rbt->nil_){
        x = z->right_;
        Transplant(rbt, z, z->right_);
    }else if(z->right_ == rbt->nil_){
        x = z->left_;
        Transplant(rbt, z, z->left_);
    }else{
        y = Minimum(z->right_, rbt->nil_);
        y_original_color = y->color_;
        x = y->right_;
        if(y->parent_ == z)
            x->parent_ = y;
        else{
            Transplant(rbt, y, y->right_);
            y->right_ = z->right_;
            y->right_->parent_ = y;
        }
        Transplant(rbt, z, y);
        y->left_ = z->left_;
        y->left_->parent_ = y;
        y->color_ = z->color_;
    }
    if(y_original_color == RbtNode_Const.Color_.BLACK_)
        Erase_Fixup(rbt, x);
    if(node_destructor != NULL)
        node_destructor(deleted_node->node_);
    RbtNode_Operations.destructor(deleted_node);
    rbt->size_ -= 1;
}

static void* 
Rbt_Inorder_Next(Rbt* rbt, int is_start_from_smallest){
    
    if(is_start_from_smallest > 0){
        rbt->ptr_ = Minimum(rbt->root_, rbt->nil_);
    }
    if(rbt->ptr_ == NULL)
        return NULL;
    void *return_value = rbt->ptr_->node_;
    rbt->ptr_ = Successor(rbt, rbt->ptr_);
    return return_value;
}










