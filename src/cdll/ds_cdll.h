#ifndef DS_CDLL_DS_CDLL_H_
#define DS_CDLL_DS_CDLL_H_

typedef struct CdllNode{
    void *node_;
    struct CdllNode *prev_;
    struct CdllNode *next_;
}CdllNode;

/*CDLL*/
typedef struct Cdll{
    CdllNode* nil_;
    CdllNode* ptr_;         /*used as iterator*/
    long long int size_;
}Cdll;

typedef struct CdllOperations{
    void (*init)(Cdll* cdll);
    void (*push_back)(Cdll* cdll, CdllNode* cdll_node);
    void* (*next)(Cdll* cdll, int is_start_from_head);
    long long int (*size)(const Cdll* cdll);
}CdllOperations;

extern const CdllOperations Cdll_Operations;


#endif