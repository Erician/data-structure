#ifndef DS_NODE_DS_NODE_H_
#define DS_NODE_DS_NODE_H_


/*an example struct of node*/
typedef struct Node{
    long long int key_;
}Node;

/*node operations struct*/
typedef struct NodeOperations{
    Node* (*constructor)(long long int key);
    void (*destructor)(void* node);
    int (*cmp)(const void* node1, const void* node2);
}NodeOperations;
extern NodeOperations Node_Operations;

#endif