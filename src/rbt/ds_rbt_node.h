#ifndef DS_RBT_DS_RBT_NODE_H_
#define DS_RBT_DS_RBT_NODE_H_

typedef struct RbtNode{
    void *node_;                 
    struct RbtNode *left_;
    struct RbtNode *right_;
    struct RbtNode *parent_;
    int color_;
}RbtNode;

typedef struct RbtNodeOperations{
    RbtNode* (*constructor)(void *node);
    void (*destructor)(RbtNode* rbt_node);
}RbtNodeOperations;
extern const RbtNodeOperations RbtNode_Operations;



typedef struct RbtNodeConst{
    struct Color{
        int BLACK_;
        int RED_;
    }Color_;
}RbtNodeConst;
extern const RbtNodeConst RbtNode_Const;

#endif
