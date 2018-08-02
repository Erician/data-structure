#ifndef DS_CDLL_DS_CDLL_H_
#include "cdll/ds_cdll.h"
#endif

#include <stdio.h>
#include <malloc.h>


/*All operations on CDLL*/
static void Cdll_Init(Cdll* cdll);
static void Cdll_Push_Back(Cdll* cdll, CdllNode* cdll_node);
static void* Cdll_Next(Cdll* cdll, int is_start_from_head);
static long long int Cdll_Size(const Cdll* cdll);

const CdllOperations Cdll_Operations = {
    .init = Cdll_Init,
    .push_back = Cdll_Push_Back,
    .next = Cdll_Next,
    .size = Cdll_Size
};

static void 
Cdll_Init(Cdll* cdll){
    cdll->nil_ = (CdllNode*)malloc(sizeof(CdllNode));
    cdll->nil_->next_ = cdll->nil_;
    cdll->nil_->prev_ = cdll->nil_;
    cdll->ptr_ = cdll->nil_;
    cdll->size_ = 0;
}

static void 
Cdll_Push_Back(Cdll* cdll, CdllNode* cdll_node){
    cdll->nil_->prev_->next_ = cdll_node;
    cdll_node -> prev_ = cdll->nil_->prev_;
    cdll_node -> next_ = cdll->nil_;
    cdll->nil_->prev_ = cdll_node;
    cdll->size_ += 1;
}

static void* 
Cdll_Next(Cdll* cdll, int is_start_from_head){
    if(is_start_from_head == 1)
        cdll->ptr_ = cdll->nil_;
    if(cdll->ptr_->next_ == cdll->nil_)
        return NULL;
    else{
        cdll->ptr_ = cdll->ptr_->next_;
        return cdll->ptr_->node_;
    }
}

static long long int
Cdll_Size(const Cdll *cdll){
    return cdll->size_;
}







